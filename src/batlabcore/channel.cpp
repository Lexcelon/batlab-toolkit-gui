#include "channel.h"

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

//#TS_IDLE,TS_CHARGE,TS_PRECHARGE,TS_DISCHARGE,TS_CHARGEREST,TS_DISCHARGEREST,TS_POSTDISCHARGE
//self.test_state = TS_IDLE
//        self.state = 'IDLE'
}
