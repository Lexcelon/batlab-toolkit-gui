#include "batlablib.h"

void BatlabLib::bubblesort(QVector<float> data, QVector<int> &indices)
{
    for (int i = 0; i < data.size() - 1; ++i) {
        for (int j = 0; j < data.size() - 1 - i ; ++j) {
            if (data.at(j) > data.at(j+1)) {
                float swap = data[j];
                data[j] = data[j+1];
                data[j+1] = swap;

                swap = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = swap;
            }
        }
    }
}

QString BatlabLib::cellName(QString designator, int numCells, int startingCellNum, int cellId) {
    int maxCellNumber = numCells + startingCellNum;
    int digits = std::max((int) log10((double) maxCellNumber) + 1, 3);
    QString numStr = QString("%1").arg(cellId, digits, 10, QChar('0'));

    return designator + "_" + numStr;
}

float BatlabLib::getCharge(int val) {
    return (6. * (static_cast<float>(val) / pow(2,15) )) * 4.096 / 9.765625;
}

float BatlabLib::getTemp(int val)
{
    float R = 10000/ ((pow(2,15)/float(val))-1);
    float To = 25.0f + 273.15f;
    float Ro = 10000;
    float B = 3428;
    float Tinv = (1/To) + (log(R/Ro)/B);
    float T = (1/Tinv) - 273.15f;
    T = (T * 1.8f) +32;
    return T;
}


float BatlabLib::getTemp(int val, int tempCalibB, int tempCalibR)
{
    float Rtherm = static_cast<float>(tempCalibR) * 4.096 / (((pow(2,15) - 1)/ val) - 1);
    float Tinv = 1 / 298.15 + log(Rtherm / 10000.) / tempCalibB;


//    float R = 10000/ ((pow(2,15)/float(val))-1);
//    float To = 25.0f + 273.15f;
//    float Ro = 10000;
//    float B = 3428;
//    float Tinv = (1/To) + (log(R/Ro)/B);
    float T = (1/Tinv) - 273.15f;
    T = (T * 1.8f) +32;
    return T;
}


float BatlabLib::getTemperature(signed short val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<signed short>::max()) * 5.0f;

    return T;
}

float BatlabLib::getCurrent(signed short val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<signed short>::max()) * 4.096f;

    return T;
}

float BatlabLib::getVoltage(signed short val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<signed short>::max()) * 4.5f;

    return T;
}

float BatlabLib::getPhase(unsigned char val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<unsigned char>::max()) * 360.0f;

    return T;
}

int BatlabLib::sendTemperatureLimits()
{
//    int T;

//    T = static_cast<int>(val / 5.0f * static_cast<float>(std::numeric_limits<signed short>::max()));

//    return T;
//    return (int)std::numeric_limits<signed short>::max();
    return 0;
}

int BatlabLib::sendCurrentLimit(float val)
{
    int T;

    T = static_cast<int>(val / 4.096f * static_cast<float>(std::numeric_limits<signed short>::max()));

    return T;
}

int BatlabLib::sendVoltageLimit(float val)
{
    int T;

    T = static_cast<int>(val / 4.5f * static_cast<float>(std::numeric_limits<signed short>::max()));

    return T;
}

int BatlabLib::sendReportingFrequency(float val)
{
    int T;

    T = static_cast<int>(val / 0.1f);

    return T;
}

int BatlabLib::sendCurrentSetpoint(float val)
{
    int T;

    if (val > 10.0f) {
        val = 10.0f;
    }

    T = static_cast<int>(val*64);

    return T;
}

float BatlabLib::getSineFrequency(int val)
{
    float T;

    T = static_cast<float>(val) * static_cast<float>(10000.0f) / static_cast<float>(256.0f);

    return T;
}

int BatlabLib::sendSineFrequency(float val)
{
    int T;

    T = static_cast<int>(val * 256.0f / 10000.0f);

    return T;
}

cellResultsStatusInfo BatlabLib::createInitializedcellResultsDisplayInfo() {
    cellResultsStatusInfo newCellResult;
    newCellResult.cellName = "";
    newCellResult.testInProgress = false;
    newCellResult.testCompleted = false;
    newCellResult.chargeCapacity = -1;
    newCellResult.energyCapacity = -1;
    newCellResult.avgImpedance = -1;
    newCellResult.deltaTemperature = -1;
    newCellResult.avgCurrent = -1;
    newCellResult.avgVoltage = -1;
    newCellResult.runtime = -1;

    return newCellResult;
}
