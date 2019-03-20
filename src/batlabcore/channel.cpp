#include "batlab.h"
#include "batlabmanager.h"

Channel::Channel(int slot, QObject *parent) : QObject(parent)
{
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

    m_channelPeriodicCheckTimer = new QTimer(this);
    connect(m_channelPeriodicCheckTimer, &QTimer::timeout, this, &Channel::periodicCheck);
    m_channelPeriodicCheckTimer->start(static_cast<int>(REPORTING_PERIOD_DEFAULT * 1000));
}

void Channel::handleSerialResponseBundleReady(batlabPacketBundle bundle)
{
    if (bundle.callback == "handleChannelPeriodicCheckResponse")
    {
        handlePeriodicCheckResponse(bundle.packets);
    }
}

Batlab* Channel::batlab()
{
    return dynamic_cast<Batlab*>(parent());
}

CellPlaylist Channel::playlist()
{
    return batlab()->playlist();
}

void Channel::startTest()
{
    m_channelPeriodicCheckTimer->setInterval(static_cast<int>(playlist().getReportingPeriod() * 1000));

    QVector<BatlabPacket> startTestPackets;

    startTestPackets.append(BatlabPacket(info.slot, VOLTAGE_LIMIT_CHG, Encoder(playlist().getHighVoltageCutoff()).asVoltage()));
    startTestPackets.append(BatlabPacket(info.slot, VOLTAGE_LIMIT_DCHG, Encoder(playlist().getLowVoltageCutoff()).asVoltage()));
    startTestPackets.append(BatlabPacket(info.slot, CURRENT_LIMIT_CHG, Encoder(playlist().getChargeCurrentSafetyCutoff()).asCurrent()));
    startTestPackets.append(BatlabPacket(info.slot, CURRENT_LIMIT_DCHG, Encoder(playlist().getDischargeCurrentSafetyCutoff()).asCurrent()));
    startTestPackets.append(BatlabPacket(info.slot, TEMP_LIMIT_CHG,
                                         Encoder(playlist().getChargeTempCutoff()).asTemperatureC(info.tempCalibR, info.tempCalibB)));
    startTestPackets.append(BatlabPacket(info.slot, TEMP_LIMIT_DCHG,
                                         Encoder(playlist().getDischargeTempCutoff()).asTemperatureC(info.tempCalibR, info.tempCalibB)));
    startTestPackets.append(BatlabPacket(info.slot, CHARGEH, 0));  // Only need to write to one of the charge registers to clear them

    if (playlist().getEnableConstantVoltage())
    {
        // If we're doing constant voltage charging, we need to have current resolution down to the small range
        startTestPackets.append(BatlabPacket(batlabNamespaces::UNIT, ZERO_AMP_THRESH, Encoder(0.05).asCurrent()));
    }

    startTestPackets.append(BatlabPacket(info.slot, TEMPERATURE));

    m_test_state = TS_PRECHARGE;

    batlabPacketBundle packetBundle;
    packetBundle.packets = startTestPackets;
    packetBundle.callback = "handleStartTestResponse";
    packetBundle.channel = info.slot;
    batlab()->sendPacketBundle(packetBundle);
}

void Channel::stateMachine()
{
    QVector<BatlabPacket> packets;
    if (m_test_state == TS_PRECHARGE)
    {
        if (info.cellName == "") { return; }
        if (m_mode == MODE_IDLE)
        {
            packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, 0));
            packets.append(BatlabPacket(info.slot, MODE, MODE_CHARGE).setSleepAfterTransaction_ms(10));
            packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, Encoder(playlist().getPrechargeRate()).asSetpoint()));

        }
        // Handle feature to trickle charge the cell if close to voltage limit
        if (playlist().getEnableTrickle() && !playlist().getEnableConstantVoltage())
        {
            if (!m_trickle_engaged && m_voltage_prev > static_cast<float>(playlist().getTrickleChargeEngageLimit()))
            {
                packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, Encoder(playlist().getTrickleChargeRate()).asSetpoint()));
                m_trickle_engaged = true;
            }
        }
        // Handle constant voltage charge
        else if (playlist().getEnableConstantVoltage())
        {
            float std_impedance;
            if (m_z_avg > 0.5f) { std_impedance = 0.5f / 128.0f; }
            else if (m_z_avg < 0.01f) { std_impedance = 0.01f / 128.0f; }
            else { std_impedance = m_z_avg / 128.0f; }
            std_impedance = std_impedance * static_cast<float>(playlist().getConstantVoltageSensitivity());
//          LEFT OFF figure out why getting 2 response packets when expecting 1 when charging
            // If voltage is getting close to the cutoff point and current is flowing at greater than a trickle
            if (m_voltage_prev > (static_cast<float>(playlist().getHighVoltageCutoff()) - (static_cast<float>(m_current_setpoint) * std_impedance))
                    && m_current_setpoint > playlist().getConstantVoltageStepSize())
            {
                // Scale down by 1/32th (default) of an amp
                packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, static_cast<quint16>(m_current_setpoint - playlist().getConstantVoltageStepSize())));
            }
        }

        if (m_mode == MODE_STOPPED)
        {
            // self.log_lvl2("PRECHARGE") TODO
            m_test_state = TS_CHARGEREST;
            m_rest_time = std::time(nullptr);
            // We should rarely hit this condition - it means you don't want to make any testing cycles, just charge up and stop, or charge up and equalize
            if (m_current_cycle >= (playlist().getNumWarmupCycles() + playlist().getNumMeasurementCycles()))
            {
                if (playlist().getStorageDischarge())
                {
                    m_test_state = TS_POSTDISCHARGE;
                    packets.append(BatlabPacket(info.slot, CURRENT_SETPOINT, Encoder(playlist().getDischargeRate()).asSetpoint()));
                    packets.append(BatlabPacket(info.slot, MODE, MODE_DISCHARGE));
                }
                else
                {
                    m_test_state = TS_IDLE;
//                    completeTest(); TODO
                }
            }
        }
    }
    else if (m_test_state == TS_CHARGEREST)
    {
//        if (std::time(nullptr) )
//        if (datetime.datetime.now() - self.rest_time).total_seconds() > self.settings.rest_time:
//                        self.log_lvl2("CHARGEREST")
//                        self.test_state = TS_DISCHARGE

//                        # reset pulse discharge variables
//                        self.pulse_state = True
//                        self.pulse_discharge_on_time = datetime.datetime.now()
//                        self.pulse_discharge_off_time = datetime.datetime.now()
//                        self.trickle_engaged = False

//                        self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.dischrg_rate).assetpoint())
//                        self.bat.write(self.slot,MODE,MODE_DISCHARGE)
//        self.current_cycle += 1
    }
    else if (m_test_state == TS_DISCHARGE)
    {
//# handle feature to end test after certain amount of time
//            if self.timeout_time is not None:
//                if self.timeout_time != 0:
//                    if(datetime.datetime.now() - self.start_time).total_seconds() > self.timeout_time:
//                        self.bat.write_verify(self.slot,MODE,MODE_STOPPED)
//            # handle feature to pulse discharge the cell
//            if self.settings.pulse_enable == 1:
//                if self.pulse_state == True:
//                    if self.pulse_discharge_on_time == 0:
//                        self.pulse_discharge_on_time = datetime.datetime.now()
//                    if (datetime.datetime.now() - self.pulse_discharge_on_time).total_seconds() > self.settings.pulse_discharge_on_time and self.settings.pulse_discharge_on_time > 0:
//                        self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.pulse_discharge_off_rate).assetpoint())
//                        self.pulse_state = False
//                        self.pulse_discharge_off_time = datetime.datetime.now()
//                else:
//                    if self.pulse_discharge_off_time == 0:
//                        self.pulse_discharge_off_time = datetime.datetime.now()
//                    if (datetime.datetime.now() - self.pulse_discharge_off_time).total_seconds() > self.settings.pulse_discharge_off_time and self.settings.pulse_discharge_off_time > 0:
//                        if self.trickle_engaged == True:
//                            self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.trickle_dischrg_rate).assetpoint())
//                        else:
//                            self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.dischrg_rate).assetpoint())
//                        self.pulse_state = True
//                        self.pulse_discharge_on_time = datetime.datetime.now()

//            elif self.settings.constant_voltage_enable == True: # handle constant voltage discharge
//                stdimpedance = 0.050 / 128.0
//                try:
//                    stdimpedance = self.zavg / 128.0
//                    if(self.zavg > 0.5):
//                        stdimpedance = 0.5 / 128.0
//                    if(self.zavg < 0.01):
//                        stdimpedance = 0.01 / 128.0
//                    if(self.zavg == 0 or math.isnan(self.zavg)):
//                        stdimpedance = 0.050 / 128.0
//                except:
//                    stdimpedance = 0.050 / 128.0
//                stdimpedance = stdimpedance * self.settings.constant_voltage_sensitivity
//                if v < (self.settings.low_volt_cutoff + (self.bat.setpoints[self.slot] * stdimpedance)) and self.bat.setpoints[self.slot] > self.settings.constant_voltage_stepsize: # if voltage is getting close to the cutoff point and current is flowing at greater than a trickle
//                    self.bat.write_verify(self.slot,CURRENT_SETPOINT,self.bat.setpoints[self.slot] - self.settings.constant_voltage_stepsize ) # scale down by 1/32th of an amp
//            # handle feature to trickle charge the cell if close to voltage limit
//            if self.settings.trickle_enable == 1 and self.settings.constant_voltage_enable == False:
//                if v < self.settings.trickle_discharge_engage_limit and self.trickle_engaged == False:
//                    self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.trickle_dischrg_rate).assetpoint())
//                    self.trickle_engaged = True

//            if mode == MODE_STOPPED:
//                if self.test_type == TT_CYCLE:
//                    self.log_lvl2("DISCHARGE")
//                    self.test_state = TS_DISCHARGEREST
//                    self.rest_time = datetime.datetime.now()

//                if self.test_type == TT_DISCHARGE:
//                    self.log_lvl2("DISCHARGE")
//                    self.test_state = TS_IDLE
//print('Test Completed: Batlab',self.bat.sn,', Channel',self.slot)
    }
    else if (m_test_state == TS_DISCHARGEREST)
    {
//        if (datetime.datetime.now() - self.rest_time).total_seconds() > self.settings.rest_time:
//                        self.log_lvl2("DISCHARGEREST")
//                        self.test_state = TS_CHARGE

//                        # reset pulse charge variables
//                        self.pulse_state = True
//                        self.pulse_charge_on_time = datetime.datetime.now()
//                        self.pulse_charge_off_time = datetime.datetime.now()
//                        self.trickle_engaged = False

//                        self.bat.write_verify(self.slot,CURRENT_SETPOINT,0)
//                        self.bat.write(self.slot,MODE,MODE_CHARGE)
//                        sleep(0.010)
//        self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.chrg_rate).assetpoint())
    }
    else if (m_test_state == TS_CHARGE)
    {
//# handle feature to pulse charge the cell
//           if self.settings.pulse_enable == 1:
//               if self.pulse_state == True:
//                   if self.pulse_charge_on_time == 0:
//                       self.pulse_charge_on_time = datetime.datetime.now()
//                   if (datetime.datetime.now() - self.pulse_charge_on_time).total_seconds() > self.settings.pulse_charge_on_time and self.settings.pulse_charge_on_time > 0:
//                       self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.pulse_charge_off_rate).assetpoint())
//                       self.pulse_state = False
//                       self.pulse_charge_off_time = datetime.datetime.now()
//               else:
//                   if self.pulse_charge_off_time == 0:
//                       self.pulse_charge_off_time = datetime.datetime.now()
//                   if (datetime.datetime.now() - self.pulse_charge_off_time).total_seconds() > self.settings.pulse_charge_off_time and self.settings.pulse_charge_off_time > 0:
//                       if self.trickle_engaged == True:
//                           self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.trickle_chrg_rate).assetpoint())
//                       else:
//                           self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.chrg_rate).assetpoint())
//                       self.pulse_state = True
//                       self.pulse_charge_on_time = datetime.datetime.now()

//           elif self.settings.constant_voltage_discharge_enable == True: # handle constant voltage discharge
//               stdimpedance = 0.050 / 128.0
//               try:
//                   stdimpedance = self.zavg / 128.0
//                   if(self.zavg > 0.5):
//                       stdimpedance = 0.5 / 128.0
//                   if(self.zavg < 0.01):
//                       stdimpedance = 0.01 / 128.0
//                   if(self.zavg == 0 or math.isnan(self.zavg)):
//                       stdimpedance = 0.050 / 128.0
//               except:
//                   stdimpedance = 0.050 / 128.0
//               stdimpedance = stdimpedance * self.settings.constant_voltage_sensitivity
//               if v > (self.settings.high_volt_cutoff - (self.bat.setpoints[self.slot] * stdimpedance)) and self.bat.setpoints[self.slot] > self.settings.constant_voltage_stepsize: # if voltage is getting close to the cutoff point and current is flowing at greater than a trickle
//                   self.bat.write_verify(self.slot,CURRENT_SETPOINT,self.bat.setpoints[self.slot] - self.settings.constant_voltage_stepsize ) # scale down by 1/32th of an amp

//           # handle feature to trickle charge the cell if close to voltage limit
//           if self.settings.trickle_enable == 1 and self.settings.constant_voltage_enable == False:
//               if v > self.settings.trickle_charge_engage_limit and self.trickle_engaged == False:
//                   self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.trickle_chrg_rate).assetpoint())
//                   self.trickle_engaged = True

//           if mode == MODE_STOPPED:
//               self.log_lvl2("CHARGE")
//               self.test_state = TS_CHARGEREST
//               self.rest_time = datetime.datetime.now()
//               if self.current_cycle >= (self.settings.num_meas_cyc + self.settings.num_warm_up_cyc):
//                   if self.settings.bool_storage_dischrg:
//                       self.test_state = TS_POSTDISCHARGE
//                       self.bat.write_verify(self.slot,CURRENT_SETPOINT,batlab.encoder.Encoder(self.settings.dischrg_rate).assetpoint())
//                       self.bat.write(self.slot,MODE,MODE_DISCHARGE)
//                   else:
//                       self.test_state = TS_IDLE
//print('Test Completed: Batlab',self.bat.sn,', Channel',self.slot,', Time:',datetime.datetime.now())
    }
    else if (m_test_state == TS_POSTDISCHARGE)
    {
        //if mode == MODE_STOPPED or v < self.settings.storage_dischrg_volt:
        //    self.log_lvl2("POSTDISCHARGE")
        //    self.bat.write_verify(self.slot,MODE,MODE_IDLE)
        //    self.test_state = TS_IDLE
        //print('Test Completed: Batlab',self.bat.sn,', Channel',self.slot,', Time:',datetime.datetime.now())
    }
    else
    {
        qWarning() << tr("Test state %1 not implemented").arg(m_test_state);
    }
    batlabPacketBundle packetBundle;
    packetBundle.packets = packets;
    packetBundle.callback = "handleStateMachineResponse";
    packetBundle.channel = info.slot;
    batlab()->sendPacketBundle(packetBundle);
}

void Channel::handleStartTestResponse(QVector<BatlabPacket> response)
{
    // Initialize the control variables
    m_start_time = std::time(nullptr);
    m_last_lvl2_time = std::time(nullptr);
    m_last_impedance_time = std::time(nullptr);
    m_rest_time = std::time(nullptr);

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
    while (response[responseCounter].getAddress() != TEMPERATURE && response[responseCounter].getNamespace() != info.slot)
    {
        responseCounter++;
    }
    m_temperature0 = response[responseCounter].asTemperatureC(info.tempCalibR, info.tempCalibB);
    m_delta_t = 0;

    m_q = 0;
    m_e = 0;

    m_vcc = 5.0;

    m_current_cycle = 0;

    // Control variables for pulse discharge test
    m_pulse_charge_on_time = 0;
    m_pulse_charge_off_time = 0;
    m_pulse_discharge_on_time = 0;
    m_pulse_discharge_off_time = 0;
    m_pulse_state = true;

    // Control variables for trickle charge/discharge at voltage limits
    m_trickle_engaged = false;
}

void Channel::handlePeriodicCheckResponse(QVector<BatlabPacket> response)
{
    int responseCounter = 0;

    m_mode = static_cast<ChannelMode>(response[responseCounter++].getValue());
    m_current_setpoint = response[responseCounter++].getValue();

    if (response.length() == 2) { return; }

    float voltage = response[responseCounter++].asVoltage();
    float current = response[responseCounter++].asCurrent();
    float temperature = response[responseCounter++].asTemperatureC(info.tempCalibR, info.tempCalibB);

    float charge;
    auto set = response[responseCounter++].getValue();
    float multiplier = 6.0;
    if (!(set & SET_CH0_HI_RES)) { multiplier = 1.0; }
    auto chargeh1 = response[responseCounter++].getValue();
    auto chargel1 = response[responseCounter++].getValue();
    auto chargeh2 = response[responseCounter++].getValue();
    auto chargel2 = response[responseCounter++].getValue();
    if (std::isnan(static_cast<float>(chargeh1))
            || std::isnan(static_cast<float>(chargel1))
            || std::isnan(static_cast<float>(chargeh2)))
    {
        charge = NAN;
    }
    else if (chargeh2 == chargeh1)
    {
        auto data = (chargeh1 << 16) + chargel1;
        charge = ((multiplier * data / powf(2, 15)) * 4.096f / 9.765625f);
    }
    else
    {
        auto data = (chargeh2 << 16) + chargel2;
        charge = ((multiplier * data / powf(2, 15)) * 4.096f / 9.765625f);
    }

    auto duty = response[responseCounter++].getValue();
    auto vc = response[responseCounter++].asVcc();

    if (!std::isnan(static_cast<float>(vc)))
    {
        if (vc < 4.35f)
        {
            qWarning() << tr("VCC on Batlab %1 is dangerously low - consider using more robust powered hub")
                          .arg(batlab()->getSerialNumber());
        }
        if (vc < 4.1f && m_vcc < 4.1f)
        {
            abortTest();
            qWarning() << tr("Test aborted due to low VCC: Batlab %1, Channel %2")
                          .arg(batlab()->getSerialNumber())
                          .arg(info.slot);
        }
        m_vcc = vc;
    }

    // Detect voltage measurement inconsistency hardware problem that was found on a couple of batlabs
    if (!std::isnan(voltage) && !std::isnan(current))
    {
        if (m_current_prev > 0.05f && m_voltage_prev > 0.5f)
        {
            if (std::abs(current - m_current_prev) < 0.05f)
            {
                if (m_voltage_prev - voltage > 0.2f)
                {
                    m_voltage_error_count++;
                    qWarning() << tr("Unexpected voltage jump detected on Batlab %1, Channel %2")
                                  .arg(batlab()->getSerialNumber())
                                  .arg(info.slot);
                    if (m_voltage_error_count > 5)
                    {
                        abortTest();
                        qWarning() << tr("Test aborted due to voltage measurement inconsistency. Possible hardware problem with: Batlab %1, Channel %2")
                                      .arg(batlab()->getSerialNumber())
                                      .arg(info.slot);
                    }
                }
            }
        }
        m_voltage_prev = voltage;
        m_current_prev = current;
        m_voltage_avg = m_voltage_avg + (voltage - m_voltage_avg) / ++m_voltage_count;
        m_current_avg = m_current_avg + (current - m_current_avg) / ++m_current_count;
        m_e = charge * m_voltage_avg;
    }

    if (m_temperature0 < 0) { m_temperature0 = temperature; }
    m_delta_t = temperature - m_temperature0;

    //      # log the results TODO
    //      if (ts - self.last_lvl1_time).total_seconds() > self.settings.reporting_period:
    //          self.last_lvl1_time = datetime.datetime.now()
    //          if (ts - self.last_impedance_time).total_seconds() > self.settings.impedance_period and self.settings.impedance_period > 0 and self.trickle_engaged == False:
    //              z = self.bat.impedance(self.slot)
    //              if math.isnan(z):
    //                  z = self.zavg
    //                  print("error in impedance measurement...using previous result")
    //              self.last_impedance_time = datetime.datetime.now()
    //              self.zcnt += 1
    //              self.zavg += (z - self.zavg) / self.zcnt
    //              logstr = str(self.name) + ',' + str(self.bat.sn) + ',' + str(self.slot) + ',' + str(ts) + ',' + '{:.4f}'.format(self.vprev) + ',' + '{:.4f}'.format(self.iprev) + ',' + '{:.4f}'.format(t) + ',' + '{:.4f}'.format(z) + ',' + '{:.4f}'.format(e) + ',' + '{:.4f}'.format(q) + ',' + state + ',,,,,,,' + ',,' + '{:.4f}'.format(self.vcc) + ',' + str(op_raw) + ',' + str(sp_raw) + ',' + str(dty)
    //          else:
    //              logstr = str(self.name) + ',' + str(self.bat.sn) + ',' + str(self.slot) + ',' + str(ts) + ',' + '{:.4f}'.format(v) + ',' + '{:.4f}'.format(i) + ',' + '{:.4f}'.format(t) + ',,' + '{:.4f}'.format(e) + ',' + '{:.4f}'.format(q) + ',' + state + ',,,,,,,' + ',,' + '{:.4f}'.format(self.vcc) + ',' + str(op_raw) + ',' + str(sp_raw) + ',' + str(dty)

    //          if self.settings.individual_cell_logs == 0:
    //              self.bat.logger.log(logstr,self.settings.logfile)
    //          else:
    //              self.bat.logger.log(logstr,self.settings.cell_logfile + self.name + '.csv')


    // Run the test state machine - decides what to do next
    stateMachine();
}

void Channel::periodicCheck()
{
    if (batlab()->inBootloader()) { return; }

    m_ts = std::time(nullptr);

    QVector<BatlabPacket> checkPackets;
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::MODE));
    checkPackets.append(BatlabPacket(info.slot, cellNamespace::CURRENT_SETPOINT));

    if (m_test_state != TS_IDLE)
    {
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::VOLTAGE));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CURRENT));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::TEMPERATURE));

        checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::SETTINGS));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEH));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEL));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEH));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEL));

        checkPackets.append(BatlabPacket(info.slot, cellNamespace::DUTY));
        checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::VCC));
    }
    batlabPacketBundle packetBundle;
    packetBundle.packets = checkPackets;
    packetBundle.callback = "handleChannelPeriodicCheckResponse";
    packetBundle.channel = info.slot;
    batlab()->sendPacketBundle(packetBundle);
}

void Channel::abortTest()
{
    QVector<BatlabPacket> abortPackets;
    abortPackets.append(BatlabPacket(info.slot, cellNamespace::MODE, MODE_STOPPED));
    batlabPacketBundle packetBundle;
    packetBundle.packets = abortPackets;
    packetBundle.callback = "handleAbortTestResponse";
    packetBundle.channel = info.slot;
    batlab()->sendPacketBundle(packetBundle);
    m_test_state = TS_IDLE;
}
