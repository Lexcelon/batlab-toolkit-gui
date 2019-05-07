#include "resultswidget.h"

ResultsWidget::ResultsWidget(QVector<cellResultsStatusInfo> infos,
                             QFrame *parent)
    : QFrame(parent) {

  this->infos = infos;

  // Total ResultsWidget Layout
  resultsLayout = new QVBoxLayout;

  // Layout for Buttons Only
  resultsButtonLayout = new QGridLayout;

  // Create Blank Labels to Replace Other Buttons
  QLabel *blankLabel = new QLabel("");
  for (int i = 0; i < 8; i++) {
    resultsButtonLayout->addWidget(blankLabel, 0, i);
  }

  // Create the Table Widget
  resultsTableWidget = new QTableWidget;
  resultsTableWidget->setRowCount(infos.length());
  resultsTableWidget->setColumnCount(8);

  // Create the Table Header
  resultsTableWidget->setHorizontalHeaderItem(
      0, new QTableWidgetItem(tr("Test Status")));
  resultsTableWidget->setHorizontalHeaderItem(
      1, new QTableWidgetItem(tr("Charge Capacity (C)")));
  resultsTableWidget->setHorizontalHeaderItem(
      2, new QTableWidgetItem(tr("Charge Capacity Range (C)")));
  resultsTableWidget->setHorizontalHeaderItem(
      3, new QTableWidgetItem(tr("Energy Capacity (J)")));
  resultsTableWidget->setHorizontalHeaderItem(
      4, new QTableWidgetItem(tr("Energy Capacity Range (J)")));
  resultsTableWidget->setHorizontalHeaderItem(
      5, new QTableWidgetItem(tr("Impedance (Ohm)")));
  resultsTableWidget->setHorizontalHeaderItem(
      6, new QTableWidgetItem(tr("Avg Voltage (V)")));
  resultsTableWidget->setHorizontalHeaderItem(
      7, new QTableWidgetItem(tr("Avg Current (A)")));

  // Stretch the Columns to Fill Available Space
  for (int c = 1; c < resultsTableWidget->horizontalHeader()->count(); c++) {
    resultsTableWidget->horizontalHeader()->setSectionResizeMode(
        c, QHeaderView::Stretch);
  }

  resultsTableWidget->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::Fixed);
  resultsTableWidget->horizontalHeader()->resizeSection(0, 90);

  // Add Cell Results Information to Table
  for (int i = 0; i < infos.length(); i++) {
    resultsTableWidget->setVerticalHeaderItem(
        i, new QTableWidgetItem(infos[i].cellName));

    // Tests currently running
    if (infos[i].testInProgress && !infos[i].hasCompleteResults) {
      QTableWidgetItem *inProgressItem =
          new QTableWidgetItem(tr("In Progress"));
      inProgressItem->setBackgroundColor(Qt::yellow);
      resultsTableWidget->setItem(i, 0, inProgressItem);
    }
    // Cell not yet assigned to Batlab
    else if (!infos[i].testInProgress && !infos[i].hasCompleteResults &&
             infos[i].batlabSerial == -1) {
      QTableWidgetItem *notStartedItem =
          new QTableWidgetItem(tr("Not Started"));
      notStartedItem->setBackgroundColor(Qt::darkGray);
      resultsTableWidget->setItem(i, 0, notStartedItem);
    }
    // Cell waiting to be put into Batlab
    else if (!infos[i].testInProgress && !infos[i].hasCompleteResults &&
             infos[i].batlabSerial != -1) {
      QTableWidgetItem *waitingForPlacementItem =
          new QTableWidgetItem(tr("Please place cell in Batlab %1, Channel %2")
                                   .arg(QString::number(infos[i].batlabSerial))
                                   .arg(QString::number(infos[i].channel)));
      waitingForPlacementItem->setBackgroundColor(Qt::lightGray);
      waitingForPlacementItem->setTextAlignment(Qt::AlignCenter);
      resultsTableWidget->setSpan(i, 0, 1, 8);
      resultsTableWidget->setItem(i, 0, waitingForPlacementItem);
    }
    // Results complete for this cell
    else {
      QTableWidgetItem *completedItem = new QTableWidgetItem(tr("Completed"));
      completedItem->setBackgroundColor(Qt::cyan);
      resultsTableWidget->setItem(i, 0, completedItem);
      resultsTableWidget->setItem(
          i, 1,
          new QTableWidgetItem(
              QString::number(static_cast<double>(infos[i].chargeCapacity))));
      resultsTableWidget->setItem(
          i, 2,
          new QTableWidgetItem(QString::number(
              static_cast<double>(infos[i].chargeCapacityRange))));
      resultsTableWidget->setItem(
          i, 3,
          new QTableWidgetItem(
              QString::number(static_cast<double>(infos[i].energyCapacity))));
      resultsTableWidget->setItem(
          i, 4,
          new QTableWidgetItem(QString::number(
              static_cast<double>(infos[i].energyCapacityRange))));
      resultsTableWidget->setItem(
          i, 5,
          new QTableWidgetItem(
              QString::number(static_cast<double>(infos[i].impedance))));
      resultsTableWidget->setItem(
          i, 6,
          new QTableWidgetItem(
              QString::number(static_cast<double>(infos[i].avgVoltage))));
      resultsTableWidget->setItem(
          i, 7,
          new QTableWidgetItem(
              QString::number(static_cast<double>(infos[i].avgCurrent))));
    }
  }

  //  resultsLayout->addLayout(resultsButtonLayout);  // Removed for now
  resultsLayout->addWidget(resultsTableWidget);
  this->setLayout(resultsLayout);
  this->setAutoFillBackground(true);
}
