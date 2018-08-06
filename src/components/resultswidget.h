#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QMessageBox>
#include "batlabcore/batlablib.h"

class ResultsWidget : public QFrame
{
public:
    explicit ResultsWidget(QVector<cellResultsStatusInfo> infos, QFrame *parent = nullptr);

signals:

public slots:
    void showSelectCells();
    void hideSelectCells();
    void selectAllRows();
    void resetCellData();

private:
    QVBoxLayout *resultsLayout;

    QGridLayout *resultsButtonLayout;
    QTableWidget *resultsTableWidget;

    QPushButton *selectCellsButton;
    QPushButton *selectAllButton;
    QPushButton *resetCellDataButton;

    QVector<QTableWidgetItem*> checkBoxItems;

    QVector<cellResultsStatusInfo> infos;

};

#endif // RESULTSWIDGET_H
