#ifndef CELLTESTSTATUSWIDGET_H
#define CELLTESTSTATUSWIDGET_H

#include <QWidget>
#include <QLabel>
#include "batlabcore/batlablib.h"

class CellTestStatusWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CellTestStatusWidget(channelStatusInfo info, QFrame *parent = nullptr);
    void updatePhaseColors(channelStatusInfo info);

signals:

public slots:

private:

    QHBoxLayout *statusBarLayout;

    QLabel *prechargeLabel;

    QLabel *warmupLabel;

    QLabel *measurementLabel;

    QLabel *storageLabel;

};

#endif // CELLTESTSTATUSWIDGET_H
