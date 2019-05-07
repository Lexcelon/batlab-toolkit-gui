#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include "batlabcore/batlablib.h"
#include <QCheckBox>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidget>
#include <QWidget>

class ResultsWidget : public QFrame {
public:
  explicit ResultsWidget(QVector<cellResultsStatusInfo> infos,
                         QFrame *parent = nullptr);

signals:

public slots:

private:
  QVBoxLayout *resultsLayout;

  QGridLayout *resultsButtonLayout;
  QTableWidget *resultsTableWidget;

  QVector<cellResultsStatusInfo> infos;
};

#endif // RESULTSWIDGET_H
