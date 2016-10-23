#ifndef BATLAB_H
#define BATLAB_H

#include <QMainWindow>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGraphicsView>
#include "batlabconfig.h"
#include "settings.h"
#include "bldataobject.h"
#include "test.h"
#include "batlabcom.h"

namespace Ui {
class Batlab;
}

class Batlab : public QMainWindow
{
    Q_OBJECT

public:
    explicit Batlab(QWidget *parent = 0);
    ~Batlab();

public slots:
    void onTest();
    void onReceiveResponse(int,int,QString,int);

private:
    Ui::Batlab *ui;
    QPushButton *test;
    QPushButton *settingsB;
    QPushButton *report;
    QPushButton *options;
    QPushButton *exit;
    QGraphicsView *glWidget;
    batlabtest *testObj = nullptr;
    batlabCom * com;

    settings * configSettings;

};

#endif // BATLAB_H
