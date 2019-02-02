#include "playlistsettingswidget.h"

PlaylistSettingsWidget::PlaylistSettingsWidget(QWidget *parent) : QWidget(parent)
{
    cellPlaylistNameLabel = new QLabel(tr("Playlist name:"));
    cellPlaylistNameLineEdit = new QLineEdit;
    // Only valid characters
    QRegExp cellPlaylistNameRx("^[ \\w\\-\\.]+$");
    QValidator *cellPlaylistNameValidator = new QRegExpValidator(cellPlaylistNameRx);
    cellPlaylistNameLineEdit->setValidator(cellPlaylistNameValidator);

    selectChemistryBox = new QGroupBox(tr("Cell chemistry type"));
    lipoRadioButton = new QRadioButton(tr("Lithium Polymer (also called Lithium-Ion Polymer, LiPo, LIP or Li-poly)"));
    ironPhosphateRadioButton = new QRadioButton(tr(qPrintable(QString::fromUtf8("Lithium Iron Phosphate (also called LiFePO\u2084 or LFP)"))));
    otherRadioButton = new QRadioButton(tr("Other"));

    lipoRadioButton->setChecked(true);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(lipoRadioButton);
    groupBoxLayout->addWidget(ironPhosphateRadioButton);
    groupBoxLayout->addWidget(otherRadioButton);
    selectChemistryBox->setLayout(groupBoxLayout);

    sameTypeLabel = new QLabel(tr("Please note that all cells in a playlist must be of the same type."));
    sameTypeLabel->setWordWrap(true);

    numCellsLabel = new QLabel(tr("Number of cells:"));
    numCellsSpinBox = new QSpinBox;
    numCellsSpinBox->setMinimum(MINIMUM_NUM_CELLS);
    numCellsSpinBox->setMaximum(MAXIMUM_NUM_CELLS);

    cellDesignatorLabel = new QLabel(tr("Cell designator:"));
    cellDesignatorLineEdit = new QLineEdit;
    cellDesignatorLineEdit->setText("CELL");
    // Only valid characters
    QRegExp cellDesignatorRx("^[\\w\\-\\.]+$");
    QValidator *cellDesignatorValidator = new QRegExpValidator(cellDesignatorRx);
    cellDesignatorLineEdit->setValidator(cellDesignatorValidator);

    startingCellNumberLabel = new QLabel(tr("Starting cell number:"));
    startingCellNumberSpinBox = new QSpinBox;
    startingCellNumberSpinBox->setMinimum(MINIMUM_STARTING_CELL_NUMBER);
    startingCellNumberSpinBox->setMaximum(MAXIMUM_STARTING_CELL_NUMBER);

    exampleCellNameLabel = new QLabel(tr("Example cell name:"));
    exampleCellName = new QLabel;

    // Update the example cell name when the designator or starting cell number are changed
    connect(cellDesignatorLineEdit, &QLineEdit::textChanged, this, &PlaylistSettingsWidget::updateExampleCellName);
    // The static_cast is needed for these because QSpinBox overloads valueChanged and we need to specify which one we want
    // https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
    connect(startingCellNumberSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PlaylistSettingsWidget::updateExampleCellName);
    connect(numCellsSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PlaylistSettingsWidget::updateExampleCellName);
    this->updateExampleCellName();

    numWarmupCyclesLabel = new QLabel(tr("Number of warmup cycles:"));
    numWarmupCyclesSpinBox = new QSpinBox;
    numWarmupCyclesSpinBox->setMinimum(NUM_WARMUP_CYCLES_MIN);
    numWarmupCyclesSpinBox->setMaximum(NUM_WARMUP_CYCLES_MAX);
    numWarmupCyclesSpinBox->setValue(NUM_WARMUP_CYCLES_DEFAULT);

    numMeasurementCyclesLabel = new QLabel(tr("Number of measurement cycles:"));
    numMeasurementCyclesSpinBox = new QSpinBox;
    numMeasurementCyclesSpinBox->setMinimum(NUM_MEASUREMENT_CYCLES_MIN);
    numMeasurementCyclesSpinBox->setMaximum(NUM_MEASUREMENT_CYCLES_MAX);
    numMeasurementCyclesSpinBox->setValue(NUM_MEASUREMENT_CYCLES_DEFAULT);

    storageDischargeCheckBox = new QCheckBox(tr("Discharge to storage voltage after testing"));
    storageDischargeCheckBox->setChecked(STORAGE_DISCHARGE_DEFAULT);
//    connect(storageDischargeCheckBox, &QCheckBox::toggled, this, &ConfigPlaylistPage::enableOrDisableStorageDischargeField);  // TODO

    advancedConfigButton = new QPushButton(tr("Advanced"));
    advancedConfigButton->setCheckable(true);
    advancedConfigButton->setChecked(false);
    advancedConfigButton->setMaximumWidth(80);

    advancedConfigExtensionWidget = new QWidget;
    // Do not show the advanced options by default
    advancedConfigExtensionWidget->hide();

    restPeriodLabel = new QLabel(tr("Rest period:"));
    restPeriodSpinBox = new QDoubleSpinBox;
    restPeriodUnit = new QLabel(tr("sec"));
    restPeriodSpinBox->setMinimum(REST_PERIOD_MIN);
    restPeriodSpinBox->setMaximum(REST_PERIOD_MAX);
    restPeriodSpinBox->setValue(REST_PERIOD_DEFAULT);

    highVoltageCutoffLabel = new QLabel(tr("High voltage cutoff:"));
    highVoltageCutoffSpinBox = new QDoubleSpinBox;
    highVoltageCutoffUnit = new QLabel(tr("V"));
    highVoltageCutoffSpinBox->setSingleStep(0.1);
    highVoltageCutoffSpinBox->setMinimum(HIGH_VOLTAGE_CUTOFF_MIN);
    highVoltageCutoffSpinBox->setMaximum(HIGH_VOLTAGE_CUTOFF_MAX);
    highVoltageCutoffSpinBox->setValue(HIGH_VOLTAGE_CUTOFF_DEFAULT);
//    connect(highVoltageCutoffSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ConfigPlaylistPage::updateDynamicFieldBounds);  // TODO

    lowVoltageCutoffLabel = new QLabel(tr("Low voltage cutoff:"));
    lowVoltageCutoffSpinBox = new QDoubleSpinBox;
    lowVoltageCutoffUnit = new QLabel(tr("V"));
    lowVoltageCutoffSpinBox->setSingleStep(0.1);
    lowVoltageCutoffSpinBox->setMinimum(LOW_VOLTAGE_CUTOFF_MIN);
    lowVoltageCutoffSpinBox->setMaximum(LOW_VOLTAGE_CUTOFF_MAX);
    lowVoltageCutoffSpinBox->setValue(LOW_VOLTAGE_CUTOFF_DEFAULT);

    chargeTemperatureCutoffLabel = new QLabel(tr("Charge temperature cutoff:"));
    chargeTemperatureCutoffSpinBox = new QDoubleSpinBox;
    chargeTemperatureCutoffUnit = new QLabel(tr("°C"));
    chargeTemperatureCutoffSpinBox->setMinimum(CHARGE_TEMP_CUTOFF_MIN);
    chargeTemperatureCutoffSpinBox->setMaximum(CHARGE_TEMP_CUTOFF_MAX);
    chargeTemperatureCutoffSpinBox->setValue(CHARGE_TEMP_CUTOFF_DEFAULT);

    dischargeTemperatureCutoffLabel = new QLabel(tr("Discharge temperature cutoff:"));
    dischargeTemperatureCutoffSpinBox = new QDoubleSpinBox;
    dischargeTemperatureCutoffUnit = new QLabel(tr("°C"));
    dischargeTemperatureCutoffSpinBox->setMinimum(DISCHARGE_TEMP_CUTOFF_MIN);
    dischargeTemperatureCutoffSpinBox->setMaximum(DISCHARGE_TEMP_CUTOFF_MAX);
    dischargeTemperatureCutoffSpinBox->setValue(DISCHARGE_TEMP_CUTOFF_DEFAULT);

    chargeCurrentSafetyCutoffLabel = new QLabel(tr("Charge current safety cutoff:"));
    chargeCurrentSafetyCutoffSpinBox = new QDoubleSpinBox;
    chargeCurrentSafetyCutoffUnit = new QLabel(tr("A"));
    chargeCurrentSafetyCutoffSpinBox->setSingleStep(0.1);
    chargeCurrentSafetyCutoffSpinBox->setDecimals(3);
    chargeCurrentSafetyCutoffSpinBox->setMinimum(CHARGE_CURRENT_SAFETY_CUTOFF_MIN);
    chargeCurrentSafetyCutoffSpinBox->setMaximum(CHARGE_CURRENT_SAFETY_CUTOFF_MAX);
    chargeCurrentSafetyCutoffSpinBox->setValue(CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT);
//    connect(chargeCurrentSafetyCutoffSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ConfigPlaylistPage::updateDynamicFieldBounds);  // TODO

    dischargeCurrentSafetyCutoffLabel = new QLabel(tr("Discharge current safety cutoff:"));
    dischargeCurrentSafetyCutoffSpinBox = new QDoubleSpinBox;
    dischargeCurrentSafetyCutoffUnit = new QLabel(tr("A"));
    dischargeCurrentSafetyCutoffSpinBox->setSingleStep(0.1);
    dischargeCurrentSafetyCutoffSpinBox->setDecimals(3);
    dischargeCurrentSafetyCutoffSpinBox->setMinimum(DISCHARGE_CURRENT_SAFETY_CUTOFF_MIN);
    dischargeCurrentSafetyCutoffSpinBox->setMaximum(DISCHARGE_CURRENT_SAFETY_CUTOFF_MAX);
    dischargeCurrentSafetyCutoffSpinBox->setValue(DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT);
//    connect(dischargeCurrentSafetyCutoffSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ConfigPlaylistPage::updateDynamicFieldBounds);  // TODO

    prechargeRateLabel = new QLabel(tr("Precharge rate:"));
    prechargeRateSpinBox = new QDoubleSpinBox;
    prechargeRateUnit = new QLabel(tr("A"));
    prechargeRateSpinBox->setSingleStep(0.1);
    prechargeRateSpinBox->setDecimals(3);
    prechargeRateSpinBox->setMinimum(PRECHARGE_RATE_MIN);
    prechargeRateSpinBox->setMaximum(CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT); // Max is updated dynamically to not be greater than the charge current safety cutoff
    prechargeRateSpinBox->setValue(PRECHARGE_RATE_DEFAULT);

    chargeRateLabel = new QLabel(tr("Charge rate:"));
    chargeRateSpinBox = new QDoubleSpinBox;
    chargeRateUnit = new QLabel(tr("A"));
    chargeRateSpinBox->setSingleStep(0.1);
    chargeRateSpinBox->setDecimals(3);
    chargeRateSpinBox->setMinimum(CHARGE_RATE_MIN);
    chargeRateSpinBox->setMaximum(CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT); // Max is updated dynamically to not be greater than the charge current safety cutoff
    chargeRateSpinBox->setValue(CHARGE_RATE_DEFAULT);

    dischargeRateLabel = new QLabel(tr("Discharge rate:"));
    dischargeRateSpinBox = new QDoubleSpinBox;
    dischargeRateUnit = new QLabel(tr("A"));
    dischargeRateSpinBox->setSingleStep(0.1);
    dischargeRateSpinBox->setDecimals(3);
    dischargeRateSpinBox->setMinimum(DISCHARGE_RATE_MIN);
    dischargeRateSpinBox->setMaximum(DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT); // Max is updated dynamically to not be greater than the discharge current safety cutoff
    dischargeRateSpinBox->setValue(DISCHARGE_RATE_DEFAULT);

    // TODO grey out if storage discharge not selected
    storageDischargeVoltageLabel = new QLabel(tr("Storage discharge voltage:"));
    storageDischargeVoltageSpinBox = new QDoubleSpinBox;
    storageDischargeVoltageUnit = new QLabel(tr("V"));
    storageDischargeVoltageSpinBox->setSingleStep(0.1);
    storageDischargeVoltageSpinBox->setRange(STORAGE_DISCHARGE_VOLTAGE_MIN, HIGH_VOLTAGE_CUTOFF_DEFAULT); // Max is updated dynamically to not be greater than the high voltage cutoff
    storageDischargeVoltageSpinBox->setValue(STORAGE_DISCHARGE_VOLTAGE_DEFAULT);

    acceptableCellImpedanceThresholdLabel = new QLabel(tr("Acceptable cell impedance threshold:"));
    acceptableCellImpedanceThresholdSpinBox = new QDoubleSpinBox;
    acceptableCellImpedanceThresholdUnit = new QLabel(tr("Ω"));
    acceptableCellImpedanceThresholdSpinBox->setSingleStep(0.1);
    acceptableCellImpedanceThresholdSpinBox->setRange(ACCEPTABLE_IMPEDANCE_THRESHOLD_MIN, ACCEPTABLE_IMPEDANCE_THRESHOLD_MAX);
    acceptableCellImpedanceThresholdSpinBox->setValue(ACCEPTABLE_IMPEDANCE_THRESHOLD_DEFAULT);

    // Show/hide the advanced options when the Advanced button is toggled
//    connect(advancedConfigButton, &QPushButton::toggled, advancedConfigExtensionWidget, &QWidget::setVisible);  // TODO

    QGridLayout *setupLayout = new QGridLayout;
    setupLayout->addWidget(cellPlaylistNameLabel, 0, 0);
    setupLayout->addWidget(cellPlaylistNameLineEdit, 0, 1);
    setupLayout->addWidget(selectChemistryBox, 1, 0, 1, 2);
    setupLayout->addWidget(sameTypeLabel, 2, 0, 1, 2);
    setupLayout->addWidget(numCellsLabel, 3, 0);
    setupLayout->addWidget(numCellsSpinBox, 3, 1);
    setupLayout->addWidget(cellDesignatorLabel, 4, 0);
    setupLayout->addWidget(cellDesignatorLineEdit, 4, 1);
    setupLayout->addWidget(startingCellNumberLabel, 5, 0);
    setupLayout->addWidget(startingCellNumberSpinBox, 5, 1);
    setupLayout->addWidget(exampleCellNameLabel, 6, 0);
    setupLayout->addWidget(exampleCellName, 6, 1);
    setupLayout->setRowStretch(7, 1);

    QGridLayout *basicConfigLayout = new QGridLayout;
    basicConfigLayout->addWidget(numWarmupCyclesLabel, 0, 0);
    basicConfigLayout->addWidget(numWarmupCyclesSpinBox, 0, 1);
    basicConfigLayout->addWidget(numMeasurementCyclesLabel, 1, 0);
    basicConfigLayout->addWidget(numMeasurementCyclesSpinBox, 1, 1);
    basicConfigLayout->addWidget(storageDischargeCheckBox, 2, 0);

    QGridLayout *advancedExtensionLayout = new QGridLayout;
    advancedExtensionLayout->addWidget(restPeriodLabel, 0, 0);
    advancedExtensionLayout->addWidget(restPeriodSpinBox, 0, 1);
    advancedExtensionLayout->addWidget(restPeriodUnit, 0, 2);
    advancedExtensionLayout->addWidget(highVoltageCutoffLabel, 1, 0);
    advancedExtensionLayout->addWidget(highVoltageCutoffSpinBox, 1, 1);
    advancedExtensionLayout->addWidget(highVoltageCutoffUnit, 1, 2);
    advancedExtensionLayout->addWidget(lowVoltageCutoffLabel, 2, 0);
    advancedExtensionLayout->addWidget(lowVoltageCutoffSpinBox, 2, 1);
    advancedExtensionLayout->addWidget(lowVoltageCutoffUnit, 2, 2);
    advancedExtensionLayout->addWidget(chargeTemperatureCutoffLabel, 3, 0);
    advancedExtensionLayout->addWidget(chargeTemperatureCutoffSpinBox, 3, 1);
    advancedExtensionLayout->addWidget(chargeTemperatureCutoffUnit, 3, 2);
    advancedExtensionLayout->addWidget(dischargeTemperatureCutoffLabel, 4, 0);
    advancedExtensionLayout->addWidget(dischargeTemperatureCutoffSpinBox, 4, 1);
    advancedExtensionLayout->addWidget(dischargeTemperatureCutoffUnit, 4, 2);
    advancedExtensionLayout->addWidget(chargeCurrentSafetyCutoffLabel, 5, 0);
    advancedExtensionLayout->addWidget(chargeCurrentSafetyCutoffSpinBox, 5, 1);
    advancedExtensionLayout->addWidget(chargeCurrentSafetyCutoffUnit, 5, 2);
    advancedExtensionLayout->addWidget(dischargeCurrentSafetyCutoffLabel, 6, 0);
    advancedExtensionLayout->addWidget(dischargeCurrentSafetyCutoffSpinBox, 6, 1);
    advancedExtensionLayout->addWidget(dischargeCurrentSafetyCutoffUnit, 6, 2);
    advancedExtensionLayout->addWidget(prechargeRateLabel, 7, 0);
    advancedExtensionLayout->addWidget(prechargeRateSpinBox, 7, 1);
    advancedExtensionLayout->addWidget(prechargeRateUnit, 7, 2);
    advancedExtensionLayout->addWidget(chargeRateLabel, 8, 0);
    advancedExtensionLayout->addWidget(chargeRateSpinBox, 8, 1);
    advancedExtensionLayout->addWidget(chargeRateUnit, 8, 2);
    advancedExtensionLayout->addWidget(dischargeRateLabel, 9, 0);
    advancedExtensionLayout->addWidget(dischargeRateSpinBox, 9, 1);
    advancedExtensionLayout->addWidget(dischargeRateUnit, 9, 2);
    advancedExtensionLayout->addWidget(storageDischargeVoltageLabel, 10, 0);
    advancedExtensionLayout->addWidget(storageDischargeVoltageSpinBox, 10, 1);
    advancedExtensionLayout->addWidget(storageDischargeVoltageUnit, 10, 2);
    advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdLabel, 11, 0);
    advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdSpinBox, 11, 1);
    advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdUnit, 11, 2);

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addLayout(basicConfigLayout);
    configLayout->addLayout(advancedExtensionLayout);
    configLayout->addStretch(1);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(setupLayout);
    layout->addStretch(1);
    layout->addLayout(configLayout);
    layout->addStretch(4);
    setLayout(layout);
}

void PlaylistSettingsWidget::loadPlaylist(CellPlaylist playlist)
{
    // TODO
}

void PlaylistSettingsWidget::updateExampleCellName()
{
    QString designator = this->cellDesignatorLineEdit->text();
    int numCells = this->numCellsSpinBox->value();
    int startingCellNumber = this->startingCellNumberSpinBox->value();

    QString cellStr = BatlabLib::cellName(designator, numCells, startingCellNumber, startingCellNumber);

    this->exampleCellName->setText(cellStr);
}
