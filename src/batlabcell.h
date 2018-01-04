#ifndef BATLABCELL_H
#define BATLABCELL_H

#include <QObject>
#include <QVector>
#include <QString>
#include "batlabcore/globals.h"
#include <QTime>


class batlabCell : public QObject
{
    Q_OBJECT

public:
    batlabCell();
    batlabCell(QString designator, testParms parms, int cycles);


    ~batlabCell();

    QVector<int> * getModes() { return &modes; }
    QVector<float> * getTemperature() { return &temperature; }
    QVector<float> * getVoltage() { return &voltage; }
    QVector<float> * getCurrent() { return &current; }
    QVector<float> * getSoC() { return &soc; }
    QVector<testPacket> getTestData() { return tests; }
    QVector<int>* getTests() { return &testsToRun; }

    uchar getUnit(){ return unit;}
    uchar getCell(){ return cell;}
    int getStatus(){ return status;}
    QString getStatusString() { return statusString; }

    int getCurrentMode() { return currentMode; }

    QString getDesignator() { return id; }
    void setDesignator(QString name) { id = name; }


    void onUpdateParameters(int cell);
    testParms onGetParameters();

    int onGetNextTest();
    float getSineFreq() { return sineFreq; }
    void setSineFreq(float freq);
    void receiveReadResponse(int batlabRegister, int value);

    void onStartTimer() {timer.start(); timer.restart();}

signals:
    void testFinished(int cell);

    // THIS NEEDS TO CHANGE BECAUSE IT IS TERRIBLE
    void updateUI(QString designator, int testNumber);
    void updateParameter(int unit, int cell, int val);
    void emitError(QString);


public slots:
    void receiveStream(int, int, float, float, float);


private:
    uchar unit;
    uchar cell;
    QVector<testPacket> tests;
    QVector<int> time;
    QVector<float> temperature;
    QVector<float> voltage;
    QVector<float> current;
    QVector<float> soc;
    QVector<int> modes;
    QVector<QPair<float,QPair<int,float>>> voltagePP;
    QVector<QPair<float,QPair<int,float>>> currentPP;
    QVector<QPair<int,int>> chargeL;
    QVector<QPair<int,int>> chargeH;
    QTime timer;

    int currentMode;

    int status;
    float sineFreq;
    QString statusString;

    QVector<int> testsToRun;

    QString id;
    testParms testParameters;

};

#endif // BATLABCELL_H
