#ifndef BATLABMANAGER_H
#define BATLABMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QSerialPortInfo>
#include <QList>
#include <QStringList>
#include "batlab.h"

// There is one BatlabManager for the entire program. It keeps track of connected Batlabs, manages their communication, and handles test state and execution.
class BatlabManager : public QObject
{
    Q_OBJECT
public:
    explicit BatlabManager(QObject *parent = nullptr);

signals:

public slots:
    void updateConnectedBatlabs();

private:
    bool cellPlaylistLoaded;
    bool testsInProgress;

    QMap<int, Batlab> connectedBatlabsBySerialNumber;


};

#endif // BATLABMANAGER_H
