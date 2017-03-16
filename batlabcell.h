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
    batlabCell(QString designator, testParms parms, int cycles);


    ~batlabCell();

    QVector<int> * getModes() { return &modes; }
    QVector<float> * getTemperature() { return &temperature; }
    QVector<float> * getVoltage() { return &voltage; }
    QVector<float> * getCurrent() { return &current; }
    QVector<int> * getCurrentAmplitude() { return &currentAmplitude; }
    QVector<int> * getVoltageAmplitude() { return &voltageAmplitude; }
    QVector<int> * getVoltagePhase() { return &voltagePhase; }
    QVector<int> * getCharge() { return &charge; }
    QVector<testPacket> getTestData() { return tests; }
    QVector<int>* getTests() { return &testsToRun; }

    uchar getUnit(){ return unit;}
    uchar getCell(){ return cell;}
    int getStatus(){ return status;}
    QString getStatusString() { return statusString; }

    QString getDesignator() { return id; }

    void newTest(uchar testnum);

    void onUpdateParameters(int unit, int cell);
    testParms onGetParameters();

    int onGetNextTest();

signals:
    void testFinished(int cell, QString id, int testNum);
    void updateParameter(int unit, int cell, cellNamespace val, int num);

public slots:
//    void receiveStream(int,float,int,int,int);
//    void receiveStreamExt(int,int,int);

    void receiveStream(int, int, float, float, float);


private:
    uchar unit;
    uchar cell;
    QVector<testPacket> tests;
    QVector<float> temperature;
    QVector<float> voltage;
    QVector<float> current;
    QVector<int> currentAmplitude;
    QVector<int> voltageAmplitude;
    QVector<int> voltagePhase;
    QVector<int> charge;
    QVector<int> modes;
    int status;
    QString statusString;

    QVector<int> testsToRun;

    QString id;
    testParms testParameters;

};

#endif // BATLABCELL_H
