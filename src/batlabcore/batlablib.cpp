#include "batlablib.h"

QVector<QString> L_TEST_STATE({"IDLE", "CHARGE", "PRECHARGE", "DISCHARGE",
                               "CHARGEREST", "DISCHARGEREST", "POSTDISCHARGE"});

QVector<QString> MODE_LIST({"NO CELL", "BACKWARDS", "IDLE", "CHARGE",
                            "DISCHARGE", "IMPEDANCE", "STOPPED"});

QString BatlabLib::cellName(QString designator, int numCells,
                            int startingCellNum, int cellId) {
  int maxCellNumber = numCells + startingCellNum;
  int digits = std::max(static_cast<int>(log10(maxCellNumber)) + 1, 3);
  QString numStr = QString("%1").arg(cellId, digits, 10, QChar('0'));

  return designator + "_" + numStr;
}

cellResultsStatusInfo BatlabLib::createInitializedCellResultsStatusInfo() {
  cellResultsStatusInfo newCellResult;

  newCellResult.cellName = "";

  newCellResult.hasSomeResults = false;
  newCellResult.hasCompleteResults = false;

  newCellResult.testInProgress = false;
  newCellResult.batlabSerial = -1;
  newCellResult.channel = -1;

  newCellResult.chargeCapacity = -1;
  newCellResult.chargeCapacityRange = -1;
  newCellResult.energyCapacity = -1;
  newCellResult.energyCapacityRange = -1;
  newCellResult.impedance = -1;
  newCellResult.avgVoltage = -1;
  newCellResult.avgCurrent = -1;

  return newCellResult;
}
