#ifndef BATLABCOM_H
#define BATLABCOM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QDebug>
#include <iostream>
#include "globals.h"

class batlabCom : public QObject
{
    Q_OBJECT
public:
    explicit batlabCom(QObject *parent = 0);
    ~batlabCom();

signals:
    void emitResponse(int,int,QString,int);
    void emitStream(int,int,int,float,int,int,int);
    void emitStreamExt(int,int,int,int,int);


public slots:
    void onRead();
    void onReadReg(int,int,vals);
    void onWriteReg(int,int,writeVals,int);
    float getTemp(int val);

private:
    QSerialPort * port;
    QVector<uchar> data;
};

#endif // BATLABCOM_H
