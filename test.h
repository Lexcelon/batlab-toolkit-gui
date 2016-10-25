#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include "batlabcom.h"
#include "globals.h"

namespace Ui {
class test;
}

class batlabtest : public QDialog
{
    Q_OBJECT

public:
    explicit batlabtest(QWidget *parent = 0);
    ~batlabtest();

public slots:
    void onRegRead();
    void onRegWrite();

signals:
    void emitReadReg(int,int,vals);
    void emitWriteReg(int,int,writeVals,int);

private:
    Ui::test *ui;

};

#endif // TEST_H
