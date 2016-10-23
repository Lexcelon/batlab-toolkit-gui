#ifndef BATLABCOM_H
#define BATLABCOM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QDebug>
#include <iostream>

class batlabCom : public QObject
{
    Q_OBJECT
public:
    explicit batlabCom(QObject *parent = 0);
    ~batlabCom();

signals:
    void emitResponse(int,int,QString,int);

public slots:
    void onRead();
    void parseData();

    void onGetTemp(int unit, int cell);
    void onGetCurr(int unit, int cell);
    void onGetCurrAmp(int unit, int cell);
    void onGetVol(int unit, int cell);
    void onGetVolPhase(int unit, int cell);
    void onGetVolAmp(int unit, int cell);
    void onGetEnergy(int unit, int cell);
    void onGetCharge(int unit, int cell);

private:
    QSerialPort * port;
    QVector<uchar> data;
};

#endif // BATLABCOM_H
