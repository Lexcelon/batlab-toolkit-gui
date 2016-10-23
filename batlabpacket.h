#ifndef BATLABPACKET_H
#define BATLABPACKET_H

#include <QObject>
#include <QVector>
#include <QString>
#include <iostream>
#include <QDebug>

class batlabPacket : public QObject
{
    Q_OBJECT
public:
    batlabPacket();
    ~batlabPacket();

private:
    QString type = "RESPONSE";
    int unit=0;
    int cell=0;
    int addr=0;
    int data=0;
    bool write = false;
    int status=0;
    int temp=0;
    int current=0;
    int voltage=0;
    int charge=0;
    int current_amp=0;
    int voltage_amp=0;
    int voltage_phase=0;
    int timestamp=0;

public slots:
    void setUnit(int val) {unit = val;}
    void setCell(int val) {cell = val;}
    void setAddr(int val) {addr = val;}
    void setData(int val) {data = val;}
    void setStatus(int val) {status = val;}
    void setTemp(int val) {temp = val;}
    void setCurrent(int val) { current = val;}
    void setVoltage(int val) {voltage = val;}
    void setCharge(int val) {charge = val;}
    void setCurrent_Amp(int val) {current_amp= val;}
    void setVoltage_Amp(int val) {voltage_amp = val;}
    void setVoltage_Phase(int val) {voltage_phase = val;}
    void setTimestamp(int val) {timestamp = val;}
    void setWrite(bool val) { write = val;}

    void printThis();

signals:
    void emitError(QString);
    void emitStatusH(uchar);
    void emitStatusL(uchar);
    void emitTempH(uchar);
    void emitTempL(uchar);
    void emitCurrentH(uchar);
    void emitCurrentL(uchar);
    void emitVoltageH(uchar);
    void emitVoltageL(uchar);
    void emitEnergyH(uchar);
    void emitEnergyL(uchar);

};

#endif // BATLABPACKET_H
