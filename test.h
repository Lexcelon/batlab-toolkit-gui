#ifndef TEST_H
#define TEST_H

#include <QDialog>

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
    void onGetTemp();
    void onGetCurr();
    void onGetCurrAmp();
    void onGetVol();
    void onGetVolAmp();
    void onGetVolPhase();
    void onGetEnergy();
    void onGetCharge();

signals:
    void emitGetTemp(int,int);
    void emitGetCurr(int,int);
    void emitGetCurrAmp(int,int);
    void emitGetVol(int,int);
    void emitGetVolAmp(int,int);
    void emitGetVolPhase(int,int);
    void emitGetEnergy(int,int);
    void emitGetCharge(int,int);

private:
    Ui::test *ui;
    QPushButton * temp;
    QPushButton * currAmp;
    QPushButton * curr;
    QPushButton * vol;
    QPushButton * volphase;
    QPushButton * volamp;
    QPushButton * energy;
    QPushButton * charge;


};

#endif // TEST_H
