#ifndef BLDATAOBJECT_H
#define BLDATAOBJECT_H

#include <QObject>
#include <QString>
#include <QDebug>
#include "globals.h"

class batlabDataObject : public QObject
{
    Q_OBJECT
public:
    batlabDataObject();
    batlabDataObject(float * loc, int estSize);
    ~batlabDataObject();

public slots:
    void setRate(float val) { rate = val; }
    void setName(QString str) { name = str; }
    void setXUnits(QString str) { xunits = str; }
    void setYUnits(QString str) { yunits = str; }

    void getRate() { emit sendRate(rate); }
    void getName() { emit sendName(name); }
    void getXUnits() { emit sendXUnits(xunits); }
    void getYUnits() { emit sendYUnits(yunits); }
    void getData() { emit sendData(data); }


signals:
    void sendData(float*);
    void sendName(QString str);
    void sendXUnits(QString str);
    void sendYUnits(QString str);
    void sendRate(float);

private:
    float * data;
    float rate;
    int size;
    QString name, xunits, yunits;

};

#endif // BLDATAOBJECT_H
