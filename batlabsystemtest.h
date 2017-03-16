#ifndef BATLABSYSTEMTEST_H
#define BATLABSYSTEMTEST_H

#include <QObject>
#include "globals.h"



class batlabSystemTest : public QObject
{
    Q_OBJECT

public:
    explicit batlabSystemTest(QObject *parent = 0);
    ~batlabSystemTest();

public slots:
    void onRecieveTest(uchar, uchar);
    void onGetTests(uchar);
    void onNextTest(uchar);

signals:
    void emitTests(QVector<uchar>);
    void emitNextTest(uchar,uchar);

private:
    QMap<uchar, QVector<uchar>> testSchedule;

};

#endif // BATLABSYSTEMTEST_H
