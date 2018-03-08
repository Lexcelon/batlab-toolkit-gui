#include "batlablib.h"

void BatlabLib::debugResponsePacket(int serialnumber, uchar packetStartByte, uchar packetNamespace, uchar packetAddress, uchar packetLowByte, uchar packetHighByte)
{
    qDebug() << "Response Packet:" << "Batlab S/N:" << qPrintable(QString::number(serialnumber).leftJustified(6, ' '))
             << "Start Byte:"<< qPrintable("0x" + QString("%1").arg(packetStartByte, 0, 16).toUpper().rightJustified(2, '0'))
             << "Namespace:" << qPrintable(BatlabLib::namespaceIntToString[packetNamespace].leftJustified(12, ' '))
             << "Address:" << qPrintable("0x" + QString("%1").arg(packetAddress, 0, 16).toUpper().rightJustified(2, '0'))
             << "Value:" << qPrintable("0x" + QString("%1").arg(256*packetHighByte + packetLowByte, 0, 16).toUpper().rightJustified(4, '0'));
}

void BatlabLib::debugResponsePacket(int serialnumber, QVector<uchar> data)
{
    debugResponsePacket(serialnumber, data[0], data[1], data[2], data[3], data[4]);
}

void BatlabLib::debugCommandPacket(int serialnumber, uchar packetStartByte, uchar packetNamespace, uchar packetAddress, uchar packetLowByte, uchar packetHighByte)
{
    qDebug() << "Command Packet: " << "Batlab S/N:" << qPrintable(QString::number(serialnumber).leftJustified(6, ' '))
             << "Start Byte:"<< qPrintable("0x" + QString("%1").arg(packetStartByte, 0, 16).toUpper().rightJustified(2, '0'))
             << "Namespace:" << qPrintable(BatlabLib::namespaceIntToString[packetNamespace].leftJustified(12, ' '))
             << "Address:" << qPrintable("0x" + QString("%1").arg(packetAddress, 0, 16).toUpper().rightJustified(2, '0'))
             << "Value:" << qPrintable("0x" + QString("%1").arg(256*packetHighByte + packetLowByte, 0, 16).toUpper().rightJustified(4, '0'));
}

void BatlabLib::debugCommandPacket(int serialnumber, QVector<uchar> data)
{
    debugCommandPacket(serialnumber, data[0], data[1], data[2], data[3], data[4]);
}

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

cellResultsDisplayInfo BatlabLib::createInitializedcellResultsDisplayInfo() {
    cellResultsDisplayInfo newCellResult;
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
