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
    explicit batlabtest(QWidget *parent = 0, QVector<batlabCom*> com = QVector<batlabCom*>());
    ~batlabtest();

public slots:
    void onRegRead();
    void onRegWrite();
//    void onPrintCell();

signals:
    void emitReadReg(int,int);
    void emitWriteReg(int,int,int);
//    void emitPrint(uchar,properties);

private:
    Ui::test *ui;
    QVector<batlabCom *> comObjects;

};

#endif // TEST_H
