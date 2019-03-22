#include "batlab.h"
#include "batlabmanager.h"

Channel::Channel(int slot, QObject *parent) : QObject(parent) {
  info.slot = slot;

  info.cellName = "";
  info.testInProgress = false;
  info.preChargeComplete = false;
  info.preChargeError = false;
  info.numWarmupCycles = -1;
  info.numWarmupCyclesCompleted = -1;
  info.warmupCyclesError = false;
  info.numMeasurementCycles = -1;
  info.numMeasurementCyclesCompleted = -1;
  info.measurementCyclesError = false;
  info.storageDischarge = false;
  info.storageDischargeComplete = false;
  info.storageDischargeError = false;
  info.tempCalibB = -1;
  info.tempCalibR = -1;

  m_mode = MODE_NO_CELL;
  m_test_state = TS_IDLE;

  m_voltage_count = 0;
  m_voltage_avg = 0;
  m_voltage_prev = 0;
  m_voltage_error_count = 0;

  m_z_avg = 0;
  m_z_count = 0;

  m_current_count = 0;
  m_current_avg = 0;
  m_current_prev = 0;
  m_current_setpoint = 256;

  m_vcc = 5.0;

  m_e = 0;
  m_q = 0;

  m_temperature0 = -1;

  m_timeout_time = std::chrono::seconds(0);

  QTimer *getStartedTimer = new QTimer(this);
  getStartedTimer->setSingleShot(true);
  connect(getStartedTimer, &QTimer::timeout, this, &Channel::startTimer);
  getStartedTimer->start((info.slot + 1) * 100);
  // Have the channels not start at the exact same
  // time to try to stagger communication
}

void Channel::startTimer() {
  m_channelPeriodicCheckTimer = new QTimer(this);
  connect(m_channelPeriodicCheckTimer, &QTimer::timeout, this,
          &Channel::periodicCheck);
  m_channelPeriodicCheckTimer->start(
      static_cast<int>(REPORTING_PERIOD_DEFAULT * 1000));
}

void Channel::handleSerialResponseBundleReady(batlabPacketBundle bundle) {
  if (bundle.callback == "handleChannelPeriodicCheckResponse") {
    handlePeriodicCheckResponse(bundle.packets);
  } else if (bundle.callback == "handleCurrentCompensateResponse") {
    handleCurrentCompensateResponse();
  } else if (bundle.callback == "handleStartTestResponse") {
    handleStartTestResponse(bundle.packets);
  } else {
    qWarning() << tr("%1 not implemented in Channel implementation")
                      .arg(bundle.callback);
  }
}

Batlab *Channel::batlab() { return dynamic_cast<Batlab *>(parent()); }

CellPlaylist Channel::playlist() { return batlab()->playlist(); }

void Channel::startTest() {
  m_channelPeriodicCheckTimer->setInterval(
      static_cast<int>(playlist().getReportingPeriod() * 1000));

  QVector<BatlabPacket> startTestPackets;

  startTestPackets.append(
      BatlabPacket(info.slot, VOLTAGE_LIMIT_CHG,
                   Encoder(playlist().getHighVoltageCutoff()).asVoltage()));
  startTestPackets.append(
      BatlabPacket(info.slot, VOLTAGE_LIMIT_DCHG,
                   Encoder(playlist().getLowVoltageCutoff()).asVoltage()));
  startTestPackets.append(BatlabPacket(
      info.slot, CURRENT_LIMIT_CHG,
      Encoder(playlist().getChargeCurrentSafetyCutoff()).asCurrent()));
  startTestPackets.append(BatlabPacket(
      info.slot, CURRENT_LIMIT_DCHG,
      Encoder(playlist().getDischargeCurrentSafetyCutoff()).asCurrent()));
  startTestPackets.append(
      BatlabPacket(info.slot, TEMP_LIMIT_CHG,
                   Encoder(playlist().getChargeTempCutoff())
                       .asTemperatureC(info.tempCalibR, info.tempCalibB)));
  startTestPackets.append(
      BatlabPacket(info.slot, TEMP_LIMIT_DCHG,
                   Encoder(playlist().getDischargeTempCutoff())
                       .asTemperatureC(info.tempCalibR, info.tempCalibB)));
  startTestPackets.append(BatlabPacket(
      info.slot, CHARGEH,
      0)); // Only need to write to one of the charge registers to clear them

  if (playlist().getEnableConstantVoltage()) {
    // If we're doing constant voltage charging, we need to have current
    // resolution down to the small range
    startTestPackets.append(BatlabPacket(
        batlabNamespaces::UNIT, ZERO_AMP_THRESH, Encoder(0.05).asCurrent()));
  }

  startTestPackets.append(BatlabPacket(info.slot, TEMPERATURE));

  m_test_state = TS_PRECHARGE;

  batlabPacketBundle packetBundle;
  packetBundle.packets = startTestPackets;
  packetBundle.callback = "handleStartTestResponse";
  packetBundle.channel = info.slot;
  batlab()->sendPacketBundle(packetBundle);
}

void Channel::stateMachine() {
  QVector<BatlabPacket> packets;
  if (m_test_state == TS_PRECHARGE && info.cellName != "") {
    if (m_mode == MODE_IDLE) {
      packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, 0));
      packets.append(BatlabPacket(info.slot, MODE, MODE_CHARGE)
                         .setSleepAfterTransaction_ms(10));
      packets.append(
          BatlabPacket(info.slot, CURRENT_SETPOINT,
                       Encoder(playlist().getPrechargeRate()).asSetpoint()));
    }
    // Handle feature to trickle charge the cell if close to voltage limit
    if (playlist().getEnableTrickle() &&
        !playlist().getEnableConstantVoltage()) {
      if (!m_trickle_engaged &&
          m_voltage_prev >
              static_cast<float>(playlist().getTrickleChargeEngageLimit())) {
        packets.append(BatlabPacket(
            info.slot, CURRENT_SETPOINT,
            Encoder(playlist().getTrickleChargeRate()).asSetpoint()));
        m_trickle_engaged = true;
      }
    }
    // Handle constant voltage charge
    else if (playlist().getEnableConstantVoltage()) {
      float std_impedance;
      if (m_z_avg > 0.5f) {
        std_impedance = 0.5f / 128.0f;
      } else if (m_z_avg < 0.01f) {
        std_impedance = 0.01f / 128.0f;
      } else {
        std_impedance = m_z_avg / 128.0f;
      }
      std_impedance =
          std_impedance *
          static_cast<float>(playlist().getConstantVoltageSensitivity());
      // LEFT OFF figure out why getting 2 response packets when
      // expecting 1 when charging

      // If voltage is getting close to the cutoff point and current is flowing
      // at greater than a trickle
      if (m_voltage_prev >
              (static_cast<float>(playlist().getHighVoltageCutoff()) -
               (static_cast<float>(m_current_setpoint) * std_impedance)) &&
          m_current_setpoint > playlist().getConstantVoltageStepSize()) {
        // Scale down by 1/32th (default) of an amp
        packets.append(BatlabPacket(
            info.slot, CURRENT_SETPOINT,
            static_cast<quint16>(m_current_setpoint -
                                 playlist().getConstantVoltageStepSize())));
      }
    }

    if (m_mode == MODE_STOPPED) {
      // self.log_lvl2("PRECHARGE") TODO
      m_test_state = TS_CHARGEREST;
      m_rest_time = std::chrono::system_clock::now();
      // We should rarely hit this condition - it means you don't want to make
      // any testing cycles, just charge up and stop, or charge up and equalize
      if (m_current_cycle >= (playlist().getNumWarmupCycles() +
                              playlist().getNumMeasurementCycles())) {
        if (playlist().getStorageDischarge()) {
          m_test_state = TS_POSTDISCHARGE;
          packets.append(BatlabPacket(
              info.slot, CURRENT_SETPOINT,
              Encoder(playlist().getDischargeRate()).asSetpoint()));
          packets.append(BatlabPacket(info.slot, MODE, MODE_DISCHARGE)
                             .setSleepAfterTransaction_ms(10));
        } else {
          m_test_state = TS_IDLE;
          // completeTest(); TODO
        }
      }
    }
  } else if (m_test_state == TS_CHARGEREST && info.cellName != "") {
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    std::chrono::duration<double> difference = now - m_rest_time;
    if (difference.count() > playlist().getRestPeriod()) {
      // self.log_lvl2("CHARGEREST") TODO
      m_test_state = TS_DISCHARGE;

      // Reset pulse discharge variables
      m_pulse_state = true;
      m_pulse_discharge_on_time = std::chrono::system_clock::now();
      m_pulse_discharge_off_time = std::chrono::system_clock::now();
      m_trickle_engaged = false;

      packets.append(
          BatlabPacket(info.slot, CURRENT_SETPOINT,
                       Encoder(playlist().getDischargeRate()).asSetpoint()));
      packets.append(BatlabPacket(info.slot, MODE, MODE_DISCHARGE)
                         .setSleepAfterTransaction_ms(10));
      m_current_cycle++;
    }
  } else if (m_test_state == TS_DISCHARGE && info.cellName != "") {
    // Handle feature to end test after certain amount of time
    if (m_timeout_time != std::chrono::seconds(0)) {
      std::chrono::time_point<std::chrono::system_clock> now =
          std::chrono::system_clock::now();
      std::chrono::duration<double> difference = now - m_start_time;
      if (difference.count() > m_timeout_time.count()) {
        abortTest();
      }
    }
    // Handle feature to pulse discharge the cell
    if (playlist().getEnablePulse()) {
      if (m_pulse_state) {
        if (m_pulse_discharge_on_time ==
            std::chrono::system_clock::from_time_t(0)) {
          m_pulse_discharge_on_time = std::chrono::system_clock::now();
        }
        std::chrono::time_point<std::chrono::system_clock> now =
            std::chrono::system_clock::now();
        std::chrono::duration<double> difference =
            now - m_pulse_discharge_on_time;
        if (difference.count() > playlist().getPulseDischargeOnTime() &&
            playlist().getPulseDischargeOnTime() > 0) {
          packets.append(BatlabPacket(
              info.slot, CURRENT_SETPOINT,
              Encoder(playlist().getPulseDischargeOffRate()).asSetpoint()));
          m_pulse_state = false;
          m_pulse_discharge_off_time = std::chrono::system_clock::now();
        }
      } else {
        if (m_pulse_discharge_off_time ==
            std::chrono::system_clock::from_time_t(0)) {
          m_pulse_discharge_off_time = std::chrono::system_clock::now();
        }
        std::chrono::time_point<std::chrono::system_clock> now =
            std::chrono::system_clock::now();
        std::chrono::duration<double> difference =
            now - m_pulse_discharge_off_time;
        if (difference.count() > playlist().getPulseDischargeOffTime() &&
            playlist().getPulseDischargeOffTime() > 0) {
          if (m_trickle_engaged) {
            packets.append(BatlabPacket(
                info.slot, CURRENT_SETPOINT,
                Encoder(playlist().getTrickleDischargeRate()).asSetpoint()));
          } else {
            packets.append(BatlabPacket(
                info.slot, CURRENT_SETPOINT,
                Encoder(playlist().getDischargeRate()).asSetpoint()));
          }
          m_pulse_state = true;
          m_pulse_discharge_on_time = std::chrono::system_clock::now();
        }
      }
    } else if (playlist().getEnableConstantVoltage()) {
      float std_impedance;
      if (m_z_avg > 0.5f) {
        std_impedance = 0.5f / 128.0f;
      } else if (m_z_avg < 0.01f) {
        std_impedance = 0.01f / 128.0f;
      } else {
        std_impedance = m_z_avg / 128.0f;
      }
      std_impedance =
          std_impedance *
          static_cast<float>(playlist().getConstantVoltageSensitivity());
      if (m_voltage_prev <
              (static_cast<float>(playlist().getLowVoltageCutoff()) +
               (static_cast<float>(m_current_setpoint) * std_impedance)) &&
          m_current_setpoint > playlist().getConstantVoltageStepSize()) {
        packets.append(BatlabPacket(
            info.slot, CURRENT_SETPOINT,
            static_cast<quint16>(m_current_setpoint -
                                 playlist().getConstantVoltageStepSize())));
      }
    }
    if (playlist().getEnableTrickle() &&
        !playlist().getEnableConstantVoltage()) {
      if (m_voltage_prev <
              static_cast<float>(playlist().getTrickleDischargeEngageLimit()) &&
          !m_trickle_engaged) {
        packets.append(BatlabPacket(
            info.slot, CURRENT_SETPOINT,
            Encoder(playlist().getTrickleDischargeRate()).asSetpoint()));
        m_trickle_engaged = true;
      }
    }
    if (m_mode == MODE_STOPPED) {
      // self.log_lvl2("DISCHARGE") TODO
      m_test_state = TS_DISCHARGEREST;
      m_rest_time = std::chrono::system_clock::now();
    }
  } else if (m_test_state == TS_DISCHARGEREST) {
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    std::chrono::duration<double> difference = now - m_rest_time;
    if (difference.count() > playlist().getRestPeriod()) {
      // self.log_lvl2("DISCHARGEREST") TODO
      m_test_state = TS_CHARGE;

      // Reset pulse discharge variables
      m_pulse_state = true;
      m_pulse_charge_on_time = std::chrono::system_clock::now();
      m_pulse_charge_off_time = std::chrono::system_clock::now();
      m_trickle_engaged = false;

      packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, 0));
      packets.append(BatlabPacket(info.slot, MODE, MODE_CHARGE)
                         .setSleepAfterTransaction_ms(10));
      packets.append(
          BatlabPacket(info.slot, CURRENT_SETPOINT,
                       Encoder(playlist().getChargeRate()).asSetpoint()));
    }
  } else if (m_test_state == TS_CHARGE) {
    // Handle feature to pulse charge the cell
    if (playlist().getEnablePulse()) {
      if (m_pulse_state) {
        if (m_pulse_charge_on_time ==
            std::chrono::system_clock::from_time_t(0)) {
          m_pulse_charge_on_time = std::chrono::system_clock::now();
        }
        std::chrono::time_point<std::chrono::system_clock> now =
            std::chrono::system_clock::now();
        std::chrono::duration<double> difference = now - m_pulse_charge_on_time;
        if (difference.count() > playlist().getPulseChargeOnTime() &&
            playlist().getPulseChargeOnTime() > 0) {
          packets.append(BatlabPacket(
              info.slot, CURRENT_SETPOINT,
              Encoder(playlist().getPulseChargeOffRate()).asSetpoint()));
          m_pulse_state = false;
          m_pulse_charge_off_time = std::chrono::system_clock::now();
        }
      } else {
        if (m_pulse_charge_off_time ==
            std::chrono::system_clock::from_time_t(0)) {
          m_pulse_charge_off_time = std::chrono::system_clock::now();
        }
        std::chrono::time_point<std::chrono::system_clock> now =
            std::chrono::system_clock::now();
        std::chrono::duration<double> difference =
            now - m_pulse_charge_off_time;
        if (difference.count() > playlist().getPulseChargeOffTime() &&
            playlist().getPulseChargeOffTime() > 0) {
          if (m_trickle_engaged) {
            packets.append(BatlabPacket(
                info.slot, CURRENT_SETPOINT,
                Encoder(playlist().getTrickleChargeRate()).asSetpoint()));
          } else {
            packets.append(
                BatlabPacket(info.slot, CURRENT_SETPOINT,
                             Encoder(playlist().getChargeRate()).asSetpoint()));
          }
          m_pulse_state = true;
          m_pulse_charge_on_time = std::chrono::system_clock::now();
        }
      }
    } else if (playlist().getEnableConstantVoltage()) {
      float std_impedance;
      if (m_z_avg > 0.5f) {
        std_impedance = 0.5f / 128.0f;
      } else if (m_z_avg < 0.01f) {
        std_impedance = 0.01f / 128.0f;
      } else {
        std_impedance = m_z_avg / 128.0f;
      }
      std_impedance =
          std_impedance *
          static_cast<float>(playlist().getConstantVoltageSensitivity());
      if (m_voltage_prev >
              (static_cast<float>(playlist().getHighVoltageCutoff()) -
               (static_cast<float>(m_current_setpoint) * std_impedance)) &&
          m_current_setpoint > playlist().getConstantVoltageStepSize()) {
        packets.append(BatlabPacket(
            info.slot, CURRENT_SETPOINT,
            static_cast<quint16>(m_current_setpoint -
                                 playlist().getConstantVoltageStepSize())));
      }
    }
    if (playlist().getEnableTrickle() &&
        !playlist().getEnableConstantVoltage()) {
      if (m_voltage_prev >
              static_cast<float>(playlist().getTrickleChargeEngageLimit()) &&
          !m_trickle_engaged) {
        packets.append(BatlabPacket(
            info.slot, CURRENT_SETPOINT,
            Encoder(playlist().getTrickleChargeRate()).asSetpoint()));
        m_trickle_engaged = true;
      }
    }
    if (m_mode == MODE_STOPPED) {
      // self.log_lvl2("CHARGE") TODO
      m_test_state = TS_CHARGEREST;
      m_rest_time = std::chrono::system_clock::now();
      if (m_current_cycle >= (playlist().getNumWarmupCycles() +
                              playlist().getNumMeasurementCycles())) {
        if (playlist().getStorageDischarge()) {
          m_test_state = TS_POSTDISCHARGE;
          packets.append(BatlabPacket(
              info.slot, CURRENT_SETPOINT,
              Encoder(playlist().getDischargeRate()).asSetpoint()));
          packets.append(BatlabPacket(info.slot, MODE, MODE_DISCHARGE)
                             .setSleepAfterTransaction_ms(10));
        } else {
          m_test_state = TS_IDLE;
          // completeTest(); TODO
        }
      }
    }
  } else if (m_test_state == TS_POSTDISCHARGE) {
    if (m_mode == MODE_STOPPED ||
        m_voltage_prev <
            static_cast<float>(playlist().getStorageDischargeVoltage())) {
      // self.log_lvl2("POSTDISCHARGE") TODO
      packets.append(BatlabPacket(info.slot, MODE, MODE_IDLE)
                         .setSleepAfterTransaction_ms(10));
      m_test_state = TS_IDLE;
      // completeTest(); TODO
    }
  } else {
    qWarning() << tr("Test state %1 not implemented").arg(m_test_state);
  }
  batlabPacketBundle packetBundle;
  packetBundle.packets = packets;
  packetBundle.callback = "handleStateMachineResponse";
  packetBundle.channel = info.slot;
  batlab()->sendPacketBundle(packetBundle);
}

void Channel::handleStartTestResponse(QVector<BatlabPacket> response) {
  // Initialize the control variables
  m_start_time = std::chrono::system_clock::now();
  m_last_lvl2_time = std::chrono::system_clock::now();
  m_last_impedance_time = std::chrono::system_clock::now();
  m_rest_time = std::chrono::system_clock::now();

  m_voltage_avg = 0;
  m_voltage_count = 0;
  m_voltage_error_count = 0;
  m_voltage_prev = 0;

  m_z_avg = 0;
  m_z_count = 0;

  m_current_avg = 0;
  m_current_count = 0;
  m_current_prev = 0;

  int responseCounter = 0;
  while (response[responseCounter].getAddress() != TEMPERATURE &&
         response[responseCounter].getNamespace() != info.slot) {
    responseCounter++;
  }
  m_temperature0 = response[responseCounter].asTemperatureC(info.tempCalibR,
                                                            info.tempCalibB);
  m_delta_t = 0;

  m_q = 0;
  m_e = 0;

  m_vcc = 5.0;

  m_current_cycle = 0;

  // Control variables for pulse discharge test
  m_pulse_charge_on_time = std::chrono::system_clock::from_time_t(0);
  m_pulse_charge_off_time = std::chrono::system_clock::from_time_t(0);
  m_pulse_discharge_on_time = std::chrono::system_clock::from_time_t(0);
  m_pulse_discharge_off_time = std::chrono::system_clock::from_time_t(0);
  m_pulse_state = true;

  // Control variables for trickle charge/discharge at voltage limits
  m_trickle_engaged = false;
}

void Channel::currentCompensate(quint16 op_raw, quint16 sp_raw) {
  m_sp_raw = sp_raw;
  QVector<BatlabPacket> packets;
  packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, op_raw));
  batlabPacketBundle packetBundle;
  packetBundle.packets = packets;
  packetBundle.callback = "handleCurrentCompensateResponse";
  packetBundle.channel = info.slot;
  batlab()->sendPacketBundle(packetBundle);
}

void Channel::handleCurrentCompensateResponse() {
  m_current_setpoint = m_sp_raw;
}

void Channel::handlePeriodicCheckResponse(QVector<BatlabPacket> response) {
  int responseCounter = 0;

  m_mode = static_cast<ChannelMode>(response[responseCounter++].getValue());
  auto p = response[responseCounter++];

  if (response.length() == 2) {
    return;
  }

  float voltage = response[responseCounter++].asVoltage();
  float current = response[responseCounter++].asCurrent();
  float temperature = response[responseCounter++].asTemperatureC(
      info.tempCalibR, info.tempCalibB);

  // Patch for current compensation problem in firmware versions <= 3.
  // Fix is to move the current compensation control loop to software and turn
  // it off in hardware.
  auto op_raw = p.getValue();       // Actual operating point
  auto sp_raw = m_current_setpoint; // Current setpoint
  auto sp = sp_raw / 128.0;
  if (m_mode == MODE_CHARGE || m_mode == MODE_DISCHARGE) {
    if (current > 0 && (sp >= 0.35 || current < 0.37f)) {
      if (current < (static_cast<float>(sp) - 0.01f)) {
        op_raw++;
      } else if (current > (static_cast<float>(sp) + 0.01f)) {
        op_raw--;
      }
    }
    if (current > 4.02f) {
      op_raw--;
    }
    if (sp > 4.5) {
      op_raw = 575;
    }
    if (op_raw < playlist().getConstantVoltageStepSize() &&
        sp_raw > 0) { // Make sure that some amount of trickle current is
                      // flowing even if our setpoint is close to 0
      op_raw = static_cast<quint16>(playlist().getConstantVoltageStepSize());
    }
    if (op_raw > 575 &&
        sp_raw <= 575) { // If for some reason we read a garbage op_raw, then
                         // don't make that our new setpoint
      op_raw = sp_raw;
    }
    currentCompensate(op_raw, sp_raw);
  }

  float charge;
  auto set = response[responseCounter++].getValue();
  float multiplier = 6.0;
  if (!(set & SET_CH0_HI_RES)) {
    multiplier = 1.0;
  }
  auto chargeh1 = response[responseCounter++].getValue();
  auto chargel1 = response[responseCounter++].getValue();
  auto chargeh2 = response[responseCounter++].getValue();
  auto chargel2 = response[responseCounter++].getValue();
  if (std::isnan(static_cast<float>(chargeh1)) ||
      std::isnan(static_cast<float>(chargel1)) ||
      std::isnan(static_cast<float>(chargeh2))) {
    charge = NAN;
  } else if (chargeh2 == chargeh1) {
    auto data = (chargeh1 << 16) + chargel1;
    charge = ((multiplier * data / powf(2, 15)) * 4.096f / 9.765625f);
  } else {
    auto data = (chargeh2 << 16) + chargel2;
    charge = ((multiplier * data / powf(2, 15)) * 4.096f / 9.765625f);
  }

  auto duty = response[responseCounter++].getValue();
  auto vc = response[responseCounter++].asVcc();

  if (!std::isnan(static_cast<float>(vc))) {
    if (vc < 4.35f) {
      qWarning() << tr("VCC on Batlab %1 is dangerously low - consider using "
                       "more robust powered hub")
                        .arg(batlab()->getSerialNumber());
    }
    if (vc < 4.1f && m_vcc < 4.1f) {
      abortTest();
      qWarning() << tr("Test aborted due to low VCC: Batlab %1, Channel %2")
                        .arg(batlab()->getSerialNumber())
                        .arg(info.slot);
    }
    m_vcc = vc;
  }

  // Detect voltage measurement inconsistency hardware problem that was found
  // on a couple of batlabs
  if (!std::isnan(voltage) && !std::isnan(current)) {
    if (m_current_prev > 0.05f && m_voltage_prev > 0.5f) {
      if (std::abs(current - m_current_prev) < 0.05f) {
        if (m_voltage_prev - voltage > 0.2f) {
          m_voltage_error_count++;
          qWarning() << tr("Unexpected voltage jump detected on Batlab %1, "
                           "Channel %2")
                            .arg(batlab()->getSerialNumber())
                            .arg(info.slot);
          if (m_voltage_error_count > 5) {
            abortTest();
            qWarning()
                << tr("Test aborted due to voltage measurement "
                      "inconsistency. "
                      "Possible hardware problem with: Batlab %1, Channel %2")
                       .arg(batlab()->getSerialNumber())
                       .arg(info.slot);
          }
        }
      }
    }
    m_voltage_prev = voltage;
    m_current_prev = current;
    m_voltage_avg =
        m_voltage_avg + (voltage - m_voltage_avg) / ++m_voltage_count;
    m_current_avg =
        m_current_avg + (current - m_current_avg) / ++m_current_count;
    m_e = charge * m_voltage_avg;
  }

  if (m_temperature0 < 0) {
    m_temperature0 = temperature;
  }
  m_delta_t = temperature - m_temperature0;

  //      # log the results TODO
  //      if (ts - self.last_lvl1_time).total_seconds() >
  //      self.settings.reporting_period:
  //          self.last_lvl1_time = datetime.datetime.now()
  //          if (ts - self.last_impedance_time).total_seconds() >
  //          self.settings.impedance_period and
  //          self.settings.impedance_period > 0 and self.trickle_engaged ==
  //          False:
  //              z = self.bat.impedance(self.slot)
  //              if math.isnan(z):
  //                  z = self.zavg
  //                  print("error in impedance measurement...using previous
  //                  result")
  //              self.last_impedance_time = datetime.datetime.now()
  //              self.zcnt += 1
  //              self.zavg += (z - self.zavg) / self.zcnt
  //              logstr = str(self.name) + ',' + str(self.bat.sn) + ',' +
  //              str(self.slot) + ',' + str(ts) + ',' +
  //              '{:.4f}'.format(self.vprev) + ',' +
  //              '{:.4f}'.format(self.iprev) + ',' + '{:.4f}'.format(t) + ','
  //              +
  //              '{:.4f}'.format(z) + ',' + '{:.4f}'.format(e) + ',' +
  //              '{:.4f}'.format(q) + ',' + state + ',,,,,,,' + ',,' +
  //              '{:.4f}'.format(self.vcc) + ',' + str(op_raw) + ',' +
  //              str(sp_raw) + ',' + str(dty)
  //          else:
  //              logstr = str(self.name) + ',' + str(self.bat.sn) + ',' +
  //              str(self.slot) + ',' + str(ts) + ',' + '{:.4f}'.format(v) +
  //              ',' + '{:.4f}'.format(i) + ',' + '{:.4f}'.format(t) + ',,' +
  //              '{:.4f}'.format(e) + ',' + '{:.4f}'.format(q) + ',' + state
  //              +
  //              ',,,,,,,' + ',,' + '{:.4f}'.format(self.vcc) + ',' +
  //              str(op_raw) + ',' + str(sp_raw) + ',' + str(dty)

  //          if self.settings.individual_cell_logs == 0:
  //              self.bat.logger.log(logstr,self.settings.logfile)
  //          else:
  //              self.bat.logger.log(logstr,self.settings.cell_logfile +
  //              self.name + '.csv')

  // Run the test state machine - decides what to do next
  stateMachine();
}

void Channel::periodicCheck() {
  if (batlab()->inBootloader()) {
    return;
  }

  m_ts = std::chrono::system_clock::now();

  QVector<BatlabPacket> checkPackets;
  checkPackets.append(BatlabPacket(info.slot, cellNamespace::MODE)
                          .setSleepAfterTransaction_ms(10));
  checkPackets.append(BatlabPacket(info.slot, cellNamespace::CURRENT_SETPOINT));

  if (m_test_state != TS_IDLE) {
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::VOLTAGE));
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::CURRENT));
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::TEMPERATURE));

    checkPackets.append(
        BatlabPacket(batlabNamespaces::UNIT, unitNamespace::SETTINGS));
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEH));
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEL));
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEH));
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEL));

    checkPackets.append(BatlabPacket(info.slot, cellNamespace::DUTY));
    checkPackets.append(
        BatlabPacket(batlabNamespaces::UNIT, unitNamespace::VCC));
  }
  batlabPacketBundle packetBundle;
  packetBundle.packets = checkPackets;
  packetBundle.callback = "handleChannelPeriodicCheckResponse";
  packetBundle.channel = info.slot;
  batlab()->sendPacketBundle(packetBundle);
}

void Channel::abortTest() {
  QVector<BatlabPacket> abortPackets;
  abortPackets.append(BatlabPacket(info.slot, cellNamespace::MODE, MODE_STOPPED)
                          .setSleepAfterTransaction_ms(10));
  batlabPacketBundle packetBundle;
  packetBundle.packets = abortPackets;
  packetBundle.callback = "handleAbortTestResponse";
  packetBundle.channel = info.slot;
  batlab()->sendPacketBundle(packetBundle);
  m_test_state = TS_IDLE;
}
