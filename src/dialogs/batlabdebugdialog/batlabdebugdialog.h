#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include "batlabcom.h"

// TODO review this implementation

namespace Ui {
class debugDialog;
}

class BatlabDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatlabDebugDialog(QWidget *parent = 0, QVector<batlabCom*> com = QVector<batlabCom*>());
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
    QVector<batlabCom *> comObjects;

};

#endif // TEST_H
