#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include "batlabcom.h"

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

signals:
    void emitReadReg(int,int,vals);

private:
    Ui::test *ui;

};

#endif // TEST_H
