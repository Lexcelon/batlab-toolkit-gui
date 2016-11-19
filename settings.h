#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include "batlabconfig.h"
#include "globals.h"

namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = 0);
    ~settings();

public slots:
    void saveSettings();

private:
    Ui::settings *ui;

    float hvc,lvc,hvcv,lvcv,htc,ltc,ccsc,dcsc,rf,ccs,sf;
    int pont,poft;

    batlabConfig * batlabSettings;
};

#endif // SETTINGS_H
