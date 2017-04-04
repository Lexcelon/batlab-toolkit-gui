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

signals:
    void emitReadResponse(int,int,int,int);
    void emitWriteResponse(int,int,int,int);
    void emitStream(int,int,int,float,int,int,int);
    void emitStreamExt(int,int,int,int,int);

    void emitStream(int,int,int,float,float,float);


public slots:
    void onRead();
    void onReadReg(int, int);
    void onWriteReg(int, int, int);
    //float getTemp(int val);

private:
    QSerialPort * port;
    QVector<uchar> data;
};

#endif // BATLABCOM_H
