#include "resultswidget.h"

ResultsWidget::ResultsWidget(QVector<cellResultsDisplayInfo> infos, QFrame *parent) : QFrame(parent)
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
    resultsTableWidget->setColumnCount(8);

    // Create the Table Header
    resultsTableWidget->setHorizontalHeaderItem(0,  new QTableWidgetItem(tr("Test Status")));
    resultsTableWidget->setHorizontalHeaderItem(1,  new QTableWidgetItem(tr("Charge Capacity (C)")));
    resultsTableWidget->setHorizontalHeaderItem(2,  new QTableWidgetItem(tr("Energy Capacity (J)")));
    resultsTableWidget->setHorizontalHeaderItem(3,  new QTableWidgetItem(tr("Avg Impedance (Ohm)")));
    resultsTableWidget->setHorizontalHeaderItem(4,  new QTableWidgetItem(tr("Delta Temperature (C)")));
    resultsTableWidget->setHorizontalHeaderItem(5,  new QTableWidgetItem(tr("Avg Current (A)")));
    resultsTableWidget->setHorizontalHeaderItem(6,  new QTableWidgetItem(tr("Avg Voltage (V)")));
    resultsTableWidget->setHorizontalHeaderItem(7,  new QTableWidgetItem(tr("Runtime (s)")));

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

        // Add Test Phase
        if (infos[i].testInProgress && !infos[i].testCompleted) {
            QTableWidgetItem *inProgressItem = new QTableWidgetItem(tr("In Progress"));
            inProgressItem->setBackgroundColor(Qt::yellow);
            resultsTableWidget->setItem(i, 0, inProgressItem);
        }
        else if (!infos[i].testInProgress && !infos[i].testCompleted) {
            QTableWidgetItem *notStartedItem = new QTableWidgetItem(tr("Not Started"));
            notStartedItem->setBackgroundColor(Qt::red);
            resultsTableWidget->setItem(i, 0, notStartedItem);
        }
        else {
            QTableWidgetItem *completedItem = new QTableWidgetItem(tr("Completed"));
            completedItem->setBackgroundColor(Qt::cyan);
            resultsTableWidget->setItem(i, 0, completedItem);
        }

        resultsTableWidget->setItem(i, 1, new QTableWidgetItem(infos[i].chargeCapacity == -1 ? "--" : QString::number(infos[i].chargeCapacity)));
        resultsTableWidget->setItem(i, 2, new QTableWidgetItem(infos[i].energyCapacity == -1 ? "--" : QString::number(infos[i].energyCapacity)));
        resultsTableWidget->setItem(i, 3, new QTableWidgetItem(infos[i].avgImpedance == -1 ? "--" : QString::number(infos[i].avgImpedance)));
        resultsTableWidget->setItem(i, 4, new QTableWidgetItem(infos[i].deltaTemperature == -1 ? "--" : QString::number(infos[i].deltaTemperature)));
        resultsTableWidget->setItem(i, 5, new QTableWidgetItem(infos[i].avgCurrent == -1 ? "--" : QString::number(infos[i].avgCurrent)));
        resultsTableWidget->setItem(i, 6, new QTableWidgetItem(infos[i].avgVoltage == -1 ? "--" : QString::number(infos[i].avgVoltage)));
        resultsTableWidget->setItem(i, 7, new QTableWidgetItem(infos[i].runtime == -1 ? "--" : QString::number(infos[i].runtime)));
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
