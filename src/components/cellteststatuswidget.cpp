#include "cellteststatuswidget.h"

CellTestStatusWidget::CellTestStatusWidget(channelInfo info, QFrame *parent) : QFrame(parent)
{

    statusBarLayout = new QHBoxLayout;
    statusBarLayout->setSpacing(0);

    // Create the Warmup Label Text
    char warmupLabelText[100];
    strcpy(warmupLabelText, "Warmup (");
    strcat(warmupLabelText, std::to_string(info.numWarmupCyclesCompleted).c_str());
    strcat(warmupLabelText, " of ");
    strcat(warmupLabelText, std::to_string(info.numWarmupCycles).c_str());
    strcat(warmupLabelText, ")");

    // Create the Measurement Label Text
    char measurementLabelText[100];
    strcpy(measurementLabelText, "Measurement (");
    strcat(measurementLabelText, std::to_string(info.numMeasurementCyclesCompleted).c_str());
    strcat(measurementLabelText, " of ");
    strcat(measurementLabelText, std::to_string(info.numMeasurementCycles).c_str());
    strcat(measurementLabelText, ")");

    // Create the Test Phases Labels
    prechargeLabel = new QLabel(tr("Precharge"));
    warmupLabel = new QLabel(tr(warmupLabelText));
    measurementLabel = new QLabel(tr(measurementLabelText));
    storageLabel = new QLabel(tr("Storage Discharge"));

    prechargeLabel->setAlignment(Qt::AlignCenter);
    warmupLabel->setAlignment(Qt::AlignCenter);
    measurementLabel->setAlignment(Qt::AlignCenter);
    storageLabel->setAlignment(Qt::AlignCenter);

    // Add Lines Between Test Phases
    prechargeLabel->setStyleSheet("QLabel { border-right: 1px solid black; border-radius: 0px; }");
    warmupLabel->setStyleSheet("QLabel { border-right: 1px solid black; border-left: 1px solid black; border-radius: 0px; }");
    measurementLabel->setStyleSheet("QLabel { border-right: 1px solid black; border-left: 1px solid black; border-radius: 0px; }");
    storageLabel->setStyleSheet("QLabel { border-left: 1px solid black; border-radius: 0px; }");

    // Update the Phase Colors
    updatePhaseColors(info);

    // Add the Valid Test Phases to Status Bar
    statusBarLayout->addWidget(prechargeLabel);
    if (info.numWarmupCycles > 0 || !info.testInProgress) {
        statusBarLayout->addWidget(warmupLabel);
    }
    if (info.numMeasurementCycles > 0 || !info.testInProgress) {
        statusBarLayout->addWidget(measurementLabel);
    }
    if (info.storageDischarge || !info.testInProgress) {
        statusBarLayout->addWidget(storageLabel);
    }

    this->setLayout(statusBarLayout);
    this->setAutoFillBackground(true);
    this->setFixedHeight(38);
    this->setStyleSheet("QFrame CellTestStatusWidget { border: 2px solid black; border-radius:5px; padding:-10px; }");
}

void CellTestStatusWidget::updatePhaseColors(channelInfo info) {

    const QString VALID_TEST_PHASE_COLOR = "QLabel { background-color : White; }";
    const QString SKIPPED_TEST_PHASE_COLOR = "QLabel { text-decoration: line-through;}";
    const QString IN_PROGRESS_TEST_PHASE_COLOR = "QLabel { background-color : #FFD104; }"; // Yellow
    const QString DONE_TEST_CASE_COLOR = "QLabel { background-color : #25B920; }"; // Green
    const QString ERROR_TEST_CASE_COLOR = "QLabel { background-color : Red; }";

    // Begin With All Phases the Default Color
    prechargeLabel->setStyleSheet(prechargeLabel->styleSheet().append(VALID_TEST_PHASE_COLOR));
    warmupLabel->setStyleSheet(warmupLabel->styleSheet().append(VALID_TEST_PHASE_COLOR));
    measurementLabel->setStyleSheet(measurementLabel->styleSheet().append(VALID_TEST_PHASE_COLOR));
    storageLabel->setStyleSheet(storageLabel->styleSheet().append(VALID_TEST_PHASE_COLOR));

    if (info.testInProgress) {

        // Update Precharge Cycle Color
        if (!info.preChargeComplete) {
            prechargeLabel->setStyleSheet(prechargeLabel->styleSheet().append(IN_PROGRESS_TEST_PHASE_COLOR));
        }
        else if (info.preChargeError) {
            prechargeLabel->setStyleSheet(prechargeLabel->styleSheet().append(ERROR_TEST_CASE_COLOR));
        }
        else {
            prechargeLabel->setStyleSheet(prechargeLabel->styleSheet().append(DONE_TEST_CASE_COLOR));
        }

        // Update Warmup Cycle Color
        if (info.numWarmupCycles <= 0) {
            warmupLabel->setStyleSheet(warmupLabel->styleSheet().append(SKIPPED_TEST_PHASE_COLOR));
        }
        else if (info.warmupCyclesError) {
            warmupLabel->setStyleSheet(warmupLabel->styleSheet().append(ERROR_TEST_CASE_COLOR));
        }
        else if (info.numWarmupCyclesCompleted < info.numWarmupCycles && info.numWarmupCyclesCompleted != 0) {
            warmupLabel->setStyleSheet(warmupLabel->styleSheet().append(IN_PROGRESS_TEST_PHASE_COLOR));
        }
        else if (info.numWarmupCyclesCompleted >= info.numWarmupCycles) {
            warmupLabel->setStyleSheet(warmupLabel->styleSheet().append(DONE_TEST_CASE_COLOR));
        }

        // Update MeasurementCycle Color
        if (info.numMeasurementCycles <= 0) {
            measurementLabel->setStyleSheet(measurementLabel->styleSheet().append(SKIPPED_TEST_PHASE_COLOR));
        }
        else if (info.measurementCyclesError) {
            measurementLabel->setStyleSheet(measurementLabel->styleSheet().append(ERROR_TEST_CASE_COLOR));
        }
        else if (info.numMeasurementCyclesCompleted < info.numMeasurementCycles && info.numMeasurementCyclesCompleted != 0) {
            measurementLabel->setStyleSheet(measurementLabel->styleSheet().append(IN_PROGRESS_TEST_PHASE_COLOR));
        }
        else if (info.numMeasurementCyclesCompleted >= info.numMeasurementCycles) {
            measurementLabel->setStyleSheet(measurementLabel->styleSheet().append(DONE_TEST_CASE_COLOR));
        }

        // Update Storage Discharge Color
        if (!info.storageDischarge) {
            storageLabel->setStyleSheet(storageLabel->styleSheet().append(SKIPPED_TEST_PHASE_COLOR));
        }
        else if (info.storageDischargeError) {
            storageLabel->setStyleSheet(storageLabel->styleSheet().append(ERROR_TEST_CASE_COLOR));
        }
        else if (info.storageDischargeComplete) {
            storageLabel->setStyleSheet(storageLabel->styleSheet().append(DONE_TEST_CASE_COLOR));
        }
        else if (info.preChargeComplete
                 && info.numWarmupCyclesCompleted >= info.numWarmupCycles
                 && info.numMeasurementCyclesCompleted >= info.numMeasurementCycles) {
            storageLabel->setStyleSheet(storageLabel->styleSheet().append(IN_PROGRESS_TEST_PHASE_COLOR));
        }
    }
}
