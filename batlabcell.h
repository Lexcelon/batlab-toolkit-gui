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
    batlabCell(QString designator, QString number);


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

    QVector<test> * getTests() { return &tests; }

    uchar getUnit(){ return unit;}
    uchar getCell(){ return cell;}
    int getStatus(){ return status;}
    QString getStatusString() { return statusString; }

    void newTest(uchar testnum);

signals:
    void testFinished(uchar);

private:
    uchar unit;
    uchar cell;
    QVector<test> tests;
    QVector<int> temperature;
    QVector<int> voltage;
    QVector<int> current;
    QVector<int> currentAmplitude;
    QVector<int> voltageAmplitude;
    QVector<int> voltagePhase;
    QVector<int> charge;
    int status;
    QString statusString;

    QString name;
    QString cellNumber;

};

#endif // BATLABCELL_H
