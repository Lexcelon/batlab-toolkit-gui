#ifndef BATLAB_H
#define BATLAB_H

#include <QObject>

class Batlab : public QObject
{
    Q_OBJECT
public:
    explicit Batlab(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BATLAB_H