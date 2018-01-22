#ifndef BATLABWIDGET_H
#define BATLABWIDGET_H

#include <QWidget>
#include <QLabel>
#include "batlabcore/batlablib.h"

class BatlabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BatlabWidget(batlabInfo info, QWidget *parent = nullptr);

signals:

public slots:

private:
    QGridLayout *batlabWidgetLayout;

    QLabel *serialNumberLabel;
    QLabel *serialNumberValueLabel;

    QLabel *portNameLabel;
    QLabel *portNameValueLabel;

    QLabel *externalPowerConnectedLabel;
    QLabel *externalPowerConnectedValueLabel;

    QLabel *firmwareVersionLabel;
    QLabel *firmwareVersionValueLabel;

};

#endif // BATLABWIDGET_H
