#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include "batlabcore/batlab.h"

// TODO review this implementation

namespace Ui {
class debugDialog;
}

class BatlabDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatlabDebugDialog(QWidget *parent = 0, QVector<Batlab*> com = QVector<Batlab*>());
    ~BatlabDebugDialog();

public slots:
    void onRegRead();
    void onRegWrite();
//    void onPrintCell();

signals:
    void emitReadReg(int,int);
    void emitWriteReg(int,int,int);
//    void emitPrint(uchar,properties);

private:
    Ui::debugDialog *ui;
    QVector<Batlab *> comObjects;

};

#endif // TEST_H
