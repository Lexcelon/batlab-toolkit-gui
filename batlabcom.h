#ifndef BATLABCOM_H
#define BATLABCOM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QDebug>
#include <QComboBox>
#include <QInputDialog>
#include <qmath.h>
#include "globals.h"

class batlabCom : public QObject
{
    Q_OBJECT
public:
    explicit batlabCom(QObject *parent = 0);
    batlabCom(QString item, QObject *parent = 0);
    ~batlabCom();

    void setAllIdle();

signals:
    void emitReadResponse(int,int,int,int);
    void emitWriteResponse(int,int,int,int);    
    void emitStream(int,int,int,float,float,float);
    void emitReadCommand (int, int, int);
    void emitWriteCommand(int, int, int, int);


public slots:
    void onRead();
    void onReadReg(int, int);
    void onWriteReg(int, int, int);
    QString getName() { return portName; }
    int getSerialNumber() {return serialNumber; }

private:
    QSerialPort * port;
    QVector<uchar> data;
    int serialNumber = -1;
    QString portName;
};

#endif // BATLABCOM_H
