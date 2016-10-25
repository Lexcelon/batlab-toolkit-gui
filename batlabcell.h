#ifndef BATLABCELL_H
#define BATLABCELL_H

#include <QObject>
#include <QVector>
#include <QString>
#include "globals.h"

class batlabCell : public QObject
{
    Q_OBJECT

public:
    batlabCell();
    batlabCell(uchar key);
    ~batlabCell();

    void receiveStream(int,float,int,int,int);
    void receiveStreamExt(int,int,int);

    QVector<int> * getTemperature() { return &temperature; }
    QVector<int> * getVoltage() { return &voltage; }
    QVector<int> * getCurrent() { return &current; }
    QVector<int> * getCurrentAmplitude() { return &currentAmplitude; }
    QVector<int> * getVoltageAmplitude() { return &voltageAmplitude; }
    QVector<int> * getVoltagePhase() { return &voltagePhase; }
    QVector<int> * getCharge() { return &charge; }
    uchar getUnit(){ return unit;}
    uchar getCell(){ return cell;}
    int getStatus(){ return status;}
    QString getStatusString() { return statusString; }

private:
    uchar unit;
    uchar cell;
    QVector<int> temperature;
    QVector<int> voltage;
    QVector<int> current;
    QVector<int> currentAmplitude;
    QVector<int> voltageAmplitude;
    QVector<int> voltagePhase;
    QVector<int> charge;
    int status;
    QString statusString;

};

#endif // BATLABCELL_H
