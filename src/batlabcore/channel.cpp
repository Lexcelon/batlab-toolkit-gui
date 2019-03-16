#include "channel.h"
#include "batlab.h"

Channel::Channel(Batlab *batlab, int slot, QObject *parent) : QObject(parent)
{
    m_batlab = batlab;
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

    m_current_count = 0;
    m_current_avg = 0;
    m_current_prev = 0;

    m_vcc = 5.0;

    m_e = 0;

    m_temperature0 = -1;

    QTimer *channelPeriodicCheckTimer = new QTimer(this);
    connect(channelPeriodicCheckTimer, &QTimer::timeout, this, &Channel::periodicCheck);
    channelPeriodicCheckTimer->start(static_cast<int>(REPORTING_PERIOD_DEFAULT * 1000));
}

void Channel::handleSerialResponseBundleReady(batlabPacketBundle bundle)
{
    if (bundle.callback == "handleChannelPeriodicCheckResponse")
    {
        handlePeriodicCheckResponse(bundle.packets);
    }
}

void Channel::startTest()
{
    // TODO
}

void Channel::handlePeriodicCheckResponse(QVector<BatlabPacket> response)
{
    int responseCounter = 0;
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

    m_mode = static_cast<ChannelMode>(response[responseCounter++].getValue());
    auto duty = response[responseCounter++].getValue();
    auto vc = response[responseCounter++].asVcc();

    if (!std::isnan(static_cast<float>(vc)))
    {
        if (vc < 4.35f)
        {
            qWarning() << tr("VCC on Batlab %1 is dangerously low - consider using more robust powered hub").arg(m_batlab->getSerialNumber());
        }
        if (vc < 4.1f && m_vcc < 4.1f)
        {
            abortTest();
            qWarning() << tr("Test aborted due to low VCC: Batlab %1, Channel %2").arg(m_batlab->getSerialNumber(), info.slot);
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
                    qWarning() << tr("Unexpected voltage jump detected on Batlab %1, Channel %2").arg(m_batlab->getSerialNumber(), info.slot);
                    if (m_voltage_error_count > 5)
                    {
                        abortTest();
                        qWarning() << tr("Test aborted due to voltage measurement inconsistency. Possible hardware problem with: Batlab %1, Channel %2")
                                      .arg(m_batlab->getSerialNumber(), info.slot);
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


    //      # actually run the test state machine - decides what to do next
    //      self.state_machine_cycletest(mode,v)
}

void Channel::periodicCheck()
{
    if (m_batlab->inBootloader()) { return; }

    m_ts = std::time(nullptr);

    if (m_test_state != TS_IDLE)
    {
        QVector<BatlabPacket> checkPackets;
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::VOLTAGE));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CURRENT));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::TEMPERATURE));

        checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::SETTINGS));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEH));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEL));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEH));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::CHARGEL));

        checkPackets.append(BatlabPacket(info.slot, cellNamespace::MODE));
        checkPackets.append(BatlabPacket(info.slot, cellNamespace::DUTY));
        checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::VCC));

        batlabPacketBundle packetBundle;
        packetBundle.packets = checkPackets;
        packetBundle.callback = "handleChannelPeriodicCheckResponse";
        packetBundle.channel = info.slot;
        m_batlab->sendPacketBundle(packetBundle);
    }
}

void Channel::abortTest()
{
    QVector<BatlabPacket> abortPackets;
    abortPackets.append(BatlabPacket(info.slot, cellNamespace::MODE, MODE_STOPPED));
    batlabPacketBundle packetBundle;
    packetBundle.packets = abortPackets;
    packetBundle.callback = "handleAbortTestResponse";
    packetBundle.channel = info.slot;
    m_batlab->sendPacketBundle(packetBundle);
    m_test_state = TS_IDLE;
}
