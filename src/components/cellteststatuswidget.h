#ifndef CELLTESTSTATUSWIDGET_H
#define CELLTESTSTATUSWIDGET_H

#include "batlabcore/batlablib.h"
#include <QLabel>
#include <QWidget>

class CellTestStatusWidget : public QFrame {
  Q_OBJECT
public:
  explicit CellTestStatusWidget(channelStatusInfo info,
                                QFrame *parent = nullptr);
  void updatePhaseColors(channelStatusInfo info);

signals:

public slots:

private:
  QHBoxLayout *statusBarLayout;

  QLabel *prechargeLabel;
  QLabel *warmupLabel;
  QLabel *measurementLabel;
  QLabel *storageLabel;
  QLabel *placeCellLabel;
};

#endif // CELLTESTSTATUSWIDGET_H
