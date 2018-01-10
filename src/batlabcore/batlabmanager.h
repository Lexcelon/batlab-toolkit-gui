#ifndef BATLABMANAGER_H
#define BATLABMANAGER_H

#include <QObject>

class BatlabManager : public QObject
{
    Q_OBJECT
public:
    explicit BatlabManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BATLABMANAGER_H