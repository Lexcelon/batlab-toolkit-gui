#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include "batlablib.h"

class Batlab;

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(Batlab *batlab, int slot, QObject *parent = nullptr);
    channelStatusInfo info;

signals:

public slots:

private:
    Batlab *m_batlab;
};

#endif // CHANNEL_H
