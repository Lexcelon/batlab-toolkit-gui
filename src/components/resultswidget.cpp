#include "resultswidget.h"

ResultsWidget::ResultsWidget(QVector<cellResultsStatusInfo> infos, QFrame *parent) : QFrame(parent)
{

    this->infos = infos;

    // Total ResultsWidget Layout
    resultsLayout = new QVBoxLayout;

    // Layout for Buttons Only
    resultsButtonLayout = new QGridLayout;

    // Create Blank Labels to Replace Other Buttons
    QLabel *blankLabel = new QLabel("");
    for (int i = 0; i < 6; i++) {
        resultsButtonLayout->addWidget(blankLabel, 0, i);
    }

    // Create Reset Cell Data Button
    resetCellDataButton = new QPushButton(tr("Reset Selected Cell Data"));
    resultsButtonLayout->addWidget(resetCellDataButton, 0, 4);
    resetCellDataButton->hide();
    connect(resetCellDataButton, &QPushButton::released, this, &ResultsWidget::resetCellData);

    // Create Select All Button
    selectAllButton = new QPushButton(tr("Select All"));
    resultsButtonLayout->addWidget(selectAllButton, 0, 5);
    selectAllButton->hide();
    connect(selectAllButton, &QPushButton::released, this, &ResultsWidget::selectAllRows);

    // Create Select Cells Button
    selectCellsButton = new QPushButton(tr("Select Cells"));
    resultsButtonLayout->addWidget(selectCellsButton, 0, 6);
    connect(selectCellsButton, &QPushButton::released, this, &ResultsWidget::showSelectCells);

    // Create the Table Widget
    resultsTableWidget = new QTableWidget;
    resultsTableWidget->setRowCount(infos.length());
    resultsTableWidget->setColumnCount(6);

    // Create the Table Header
    resultsTableWidget->setHorizontalHeaderItem(0,  new QTableWidgetItem(tr("Test Status")));
    resultsTableWidget->setHorizontalHeaderItem(1,  new QTableWidgetItem(tr("Capacity (C)")));
    resultsTableWidget->setHorizontalHeaderItem(2,  new QTableWidgetItem(tr("Capacity Range (C)")));
    resultsTableWidget->setHorizontalHeaderItem(3,  new QTableWidgetItem(tr("Impedance (Ohm)")));
    resultsTableWidget->setHorizontalHeaderItem(4,  new QTableWidgetItem(tr("Avg Voltage (V)")));
    resultsTableWidget->setHorizontalHeaderItem(5,  new QTableWidgetItem(tr("Avg Current (A)")));

    // Stretch the Columns to Fill Available Space
    for (int c = 1; c < resultsTableWidget->horizontalHeader()->count(); c++)
    {
        resultsTableWidget->horizontalHeader()->setSectionResizeMode(
                    c, QHeaderView::Stretch);
    }

    resultsTableWidget->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::Fixed);
    resultsTableWidget->horizontalHeader()->resizeSection(0, 90);

    // Add Cell Results Information to Table
    for (int i = 0; i < infos.length(); i++) {
        resultsTableWidget->setVerticalHeaderItem(i,  new QTableWidgetItem(infos[i].cellName));

        // Tests currently running
        if (infos[i].testInProgress && !infos[i].hasCompleteResults) {
            QTableWidgetItem *inProgressItem = new QTableWidgetItem(tr("In Progress"));
            inProgressItem->setBackgroundColor(Qt::yellow);
            resultsTableWidget->setItem(i, 0, inProgressItem);
        }
        // Cell not yet assigned to Batlab
        else if (!infos[i].testInProgress && !infos[i].hasCompleteResults && infos[i].batlabSerial == -1) {
            QTableWidgetItem *notStartedItem = new QTableWidgetItem(tr("Not Started"));
            notStartedItem->setBackgroundColor(Qt::darkGray);
            resultsTableWidget->setItem(i, 0, notStartedItem);
        }
        // Cell waiting to be put into Batlab
        else if (!infos[i].testInProgress && !infos[i].hasCompleteResults && infos[i].batlabSerial != -1) {
            QTableWidgetItem *waitingForPlacementItem = new QTableWidgetItem(tr("Please place cell in Batlab %1, Channel %2")
                                                                             .arg(QString::number(infos[i].batlabSerial))
                                                                             .arg(QString::number(infos[i].channel)));
            waitingForPlacementItem->setBackgroundColor(Qt::lightGray);
            waitingForPlacementItem->setTextAlignment(Qt::AlignCenter);
            resultsTableWidget->setSpan(i, 0, 1, 6);
            resultsTableWidget->setItem(i, 0, waitingForPlacementItem);
        }
        // Results complete for this cell
        else {
            QTableWidgetItem *completedItem = new QTableWidgetItem(tr("Completed"));
            completedItem->setBackgroundColor(Qt::cyan);
            resultsTableWidget->setItem(i, 0, completedItem);
            resultsTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(static_cast<double>(infos[i].capacity))));
            resultsTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(static_cast<double>(infos[i].capacityRange))));
            resultsTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(static_cast<double>(infos[i].impedance))));
            resultsTableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(static_cast<double>(infos[i].avgVoltage))));
            resultsTableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(static_cast<double>(infos[i].avgCurrent))));
        }


    }

    resultsLayout->addLayout(resultsButtonLayout);
    resultsLayout->addWidget(resultsTableWidget);
    this->setLayout(resultsLayout);
    this->setAutoFillBackground(true);

}

void ResultsWidget::showSelectCells() {
    resultsTableWidget->setColumnCount(9);
    selectCellsButton->setText(tr("Cancel"));

    // Resize the Selection Column
    resultsTableWidget->horizontalHeader()->setSectionResizeMode(
        8, QHeaderView::Fixed);
    resultsTableWidget->horizontalHeader()->resizeSection(8, 30);
    resultsTableWidget->setHorizontalHeaderItem(8,  new QTableWidgetItem(tr("X")));

    // Reset Button Connections
    disconnect(selectCellsButton, &QAbstractButton::released, this, &ResultsWidget::showSelectCells);
    connect(selectCellsButton, &QAbstractButton::released, this, &ResultsWidget::hideSelectCells);

    // Show Relevant Buttons
    selectAllButton->show();
    resetCellDataButton->show();

    for (int i = 0; i < infos.length(); i++) {

        // Create Check Box Item
        QTableWidgetItem *checkBoxItem = new QTableWidgetItem;
        checkBoxItem->setCheckState(Qt::Unchecked);

        // Add Check Box Item to Vector
        checkBoxItems.push_back(checkBoxItem);

        // Add Check Box to Row
        resultsTableWidget->setItem(i, 8, checkBoxItem);
    }

}

void ResultsWidget::hideSelectCells() {
    resultsTableWidget->setColumnCount(8);
    selectCellsButton->setText(tr("Select Cells"));

    // Stretch the Columns to Fill Available Space
    for (int c = 1; c < resultsTableWidget->horizontalHeader()->count(); ++c)
    {
        resultsTableWidget->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }

    // Reset Button Connections
    disconnect(selectCellsButton, &QAbstractButton::released, this, &ResultsWidget::hideSelectCells);
    connect(selectCellsButton, &QAbstractButton::released, this, &ResultsWidget::showSelectCells);

    // Hide Relevant Buttons
    selectAllButton->hide();
    resetCellDataButton->hide();

    // Delete All CheckBoxes From the Array
    checkBoxItems.clear();
}

void ResultsWidget::selectAllRows() {

    // Select All CheckBoxes
    for (int i = 0; i < checkBoxItems.length(); i++) {
        checkBoxItems[i]->setCheckState(Qt::Checked);
    }
}

void ResultsWidget::resetCellData() {

    // Create a List of Cell Names to Reset
    QVector<QString> cellsToReset;
    for (int i = 0; i < infos.length(); i++) {
        if (i < checkBoxItems.length() && checkBoxItems[i]->checkState() == Qt::Checked) {
            cellsToReset.push_back(infos[i].cellName);
        }
    }

    if (cellsToReset.length() > 0) {

        // Ask the User to Confirm
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Reset Selected Cell Data", "Are you sure you want to reset the selected cell data?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {

            // TODO: Reset the Cell Data using the list of Cell Names in cellsToReset

            hideSelectCells();
        }
    }
    else {

        // Warn the User that No Cells Were Selected
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "No Cells Selected", "You did not select any cells.",
                                      QMessageBox::Ok);
    }
}
