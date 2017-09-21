#include <QtWidgets>

#include "newcellplaylistwizard.h"

void NewCellPlaylistWizard::accept()
{

}

NewCellPlaylistWizard::NewCellPlaylistWizard(QWidget *parent) : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new BasicSetupPage);
    addPage(new ConfigPlaylistPage);

    // TODO set this to good size once wizard implementation complete
    this->setMinimumSize(500, 600);

    setWindowTitle(tr("New Cell Playlist"));
}

IntroPage::IntroPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Introduction"));

    label = new QLabel(tr("This wizard will generate a playlist file, which is "
                          "a list of cells with information about the cells and"
                          " the tests you would like to run on the cells. Once "
                          "you create a playlist, you can load that playlist and"
                          " execute the tests on your battery cells to generate results."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void BasicSetupPage::updateExampleCellName()
{
    QString designator = this->cellDesignatorLineEdit->text();
    int numCells = this->numCellsSpinBox->value();
    int startingCellNumber = this->startingCellNumberSpinBox->value();
    int maxCellNumber = numCells + startingCellNumber;
    int digits = std::max((int) log10((double) maxCellNumber) + 1, 3);
    QString startingCellNumberStr = QString("%1").arg(startingCellNumber, digits, 10, QChar('0'));

    this->exampleCellName->setText(designator + "_" + startingCellNumberStr);
}

BasicSetupPage::BasicSetupPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Basic Setup"));
    setSubTitle(tr("Provide basic setup information for a new cell playlist. Please fill all fields."));

    cellPlaylistNameLabel = new QLabel(tr("Playlist name:"));
    cellPlaylistNameLineEdit = new QLineEdit;
    // Only valid characters
    QRegExp cellPlaylistNameRx("^[ \\w\\-\\.]+$");
    QValidator *cellPlaylistNameValidator = new QRegExpValidator(cellPlaylistNameRx);
    cellPlaylistNameLineEdit->setValidator(cellPlaylistNameValidator);
    // TODO trim whitespace from strings in fields with simplified()

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
    cellPlaylistNameLineEdit->setValidator(cellDesignatorValidator);
    // TODO trim whitespace from strings in fields with simplified()

    startingCellNumberLabel = new QLabel(tr("Starting cell number:"));
    startingCellNumberSpinBox = new QSpinBox;
    startingCellNumberSpinBox->setMinimum(MINIMUM_STARTING_CELL_NUMBER);
    startingCellNumberSpinBox->setMaximum(MAXIMUM_STARTING_CELL_NUMBER);

    exampleCellNameLabel = new QLabel(tr("Example cell name:"));
    exampleCellName = new QLabel;

    registerField("cellPlaylistName*", cellPlaylistNameLineEdit);
    registerField("lipoChemistry", lipoRadioButton);
    registerField("ironPhosphateChemistry", ironPhosphateRadioButton);
    registerField("otherChemistry", otherRadioButton);
    registerField("numCells", numCellsSpinBox);
    registerField("cellDesignator", cellDesignatorLineEdit);
    registerField("startingCellNumber", startingCellNumberSpinBox);

    // Update the example cell name when the designator or starting cell number are changed
    connect(cellDesignatorLineEdit, &QLineEdit::textChanged, this, &BasicSetupPage::updateExampleCellName);
    // The static_cast is needed for these because QSpinBox overloads valueChanged and we need to specify which one we want
    // https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
    connect(startingCellNumberSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &BasicSetupPage::updateExampleCellName);
    connect(numCellsSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &BasicSetupPage::updateExampleCellName);
    this->updateExampleCellName();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(cellPlaylistNameLabel, 0, 0);
    layout->addWidget(cellPlaylistNameLineEdit, 0, 1);
    layout->addWidget(selectChemistryBox, 1, 0, 1, 2);
    layout->addWidget(sameTypeLabel, 2, 0, 1, 2);
    layout->addWidget(numCellsLabel, 3, 0);
    layout->addWidget(numCellsSpinBox, 3, 1);
    layout->addWidget(cellDesignatorLabel, 4, 0);
    layout->addWidget(cellDesignatorLineEdit, 4, 1);
    layout->addWidget(startingCellNumberLabel, 5, 0);
    layout->addWidget(startingCellNumberSpinBox, 5, 1);
    layout->addWidget(exampleCellNameLabel, 6, 0);
    layout->addWidget(exampleCellName, 6, 1);
    setLayout(layout);
}

void ConfigPlaylistPage::initializePage()
{
    numWarmupCyclesSpinBox->setMinimum(NUM_WARMUP_CYCLES_MIN);
    numWarmupCyclesSpinBox->setMaximum(NUM_WARMUP_CYCLES_MAX);
    numWarmupCyclesSpinBox->setValue(NUM_WARMUP_CYCLES_DEFAULT);

    numMeasurementCyclesSpinBox->setMinimum(NUM_MEASUREMENT_CYCLES_MIN);
    numMeasurementCyclesSpinBox->setMaximum(NUM_MEASUREMENT_CYCLES_MAX);
    numMeasurementCyclesSpinBox->setValue(NUM_MEASUREMENT_CYCLES_DEFAULT);

    storageDischargeCheckBox->setChecked(STORAGE_DISCHARGE_DEFAULT);
}

ConfigPlaylistPage::ConfigPlaylistPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Configure Playlist Settings"));
    setSubTitle(tr("Configure the specific settings of your playlist."));

    basicConfigWidget = new QWidget;

    numWarmupCyclesLabel = new QLabel(tr("Number of warmup cycles:"));
    numWarmupCyclesSpinBox = new QSpinBox;

    numMeasurementCyclesLabel = new QLabel(tr("Number of measurement cycles:"));
    numMeasurementCyclesSpinBox = new QSpinBox;

    storageDischargeCheckBox = new QCheckBox(tr("Discharge to storage voltage after testing"));

    QGridLayout *basicConfigLayout = new QGridLayout;
    basicConfigLayout->addWidget(numWarmupCyclesLabel, 0, 0);
    basicConfigLayout->addWidget(numWarmupCyclesSpinBox, 0, 1);
    basicConfigLayout->addWidget(numMeasurementCyclesLabel, 1, 0);
    basicConfigLayout->addWidget(numMeasurementCyclesSpinBox, 1, 1);
    basicConfigLayout->addWidget(storageDischargeCheckBox, 2, 0);
    basicConfigWidget->setLayout(basicConfigLayout);

    advancedConfigButton = new QPushButton(tr("Advanced"));
    advancedConfigButton->setCheckable(true);
    advancedConfigButton->setChecked(false);
    advancedConfigButton->setMaximumWidth(80);

    advancedConfigExtensionWidget = new QWidget;
    // Do not show the advanced options by default
    advancedConfigExtensionWidget->hide();

    restPeriodLabel = new QLabel(tr("Rest period:"));
    restPeriodSpinBox = new QSpinBox;
    restPeriodUnit = new QLabel(tr("sec"));

    highVoltageCutoffLabel = new QLabel(tr("High voltage cutoff:"));
    highVoltageCutoffSpinBox = new QSpinBox;
    highVoltageCutoffUnit = new QLabel(tr("V"));

    lowVoltageCutoffLabel = new QLabel(tr("Low voltage cutoff:"));
    lowVoltageCutoffSpinBox = new QSpinBox;
    lowVoltageCutoffUnit = new QLabel(tr("V"));

    chargeTemperatureCutoffLabel = new QLabel(tr("Charge temperature cutoff:"));
    chargeTemperatureCutoffSpinBox = new QSpinBox;
    chargeTemperatureCutoffUnit = new QLabel(tr("°C"));

    dischargeTemperatureCutoffLabel = new QLabel(tr("Discharge temperature cutoff:"));
    dischargeTemperatureCutoffSpinBox = new QSpinBox;
    dischargeTemperatureCutoffUnit = new QLabel(tr("°C"));

    chargeCurrentSafetyCutoffLabel = new QLabel(tr("Charge current safety cutoff:"));
    chargeCurrentSafetyCutoffSpinBox = new QSpinBox;
    chargeCurrentSafetyCutoffUnit = new QLabel(tr("A"));

    dischargeCurrentSafetyCutoffLabel = new QLabel(tr("Discharge current safety cutoff:"));
    dischargeCurrentSafetyCutoffSpinBox = new QSpinBox;
    dischargeCurrentSafetyCutoffUnit = new QLabel(tr("A"));

    chargeRateLabel = new QLabel(tr("Charge rate:"));
    chargeRateSpinBox = new QSpinBox;
    chargeRateUnit = new QLabel(tr("A"));

    dischargeRateLabel = new QLabel(tr("Discharge rate:"));
    dischargeRateSpinBox = new QSpinBox;
    dischargeRateUnit = new QLabel(tr("A"));

    storageDischargeVoltageLabel = new QLabel(tr("Storage discharge voltage:"));
    storageDischargeVoltageSpinBox = new QSpinBox;
    storageDischargeVoltageUnit = new QLabel(tr("V"));

    acceptableCellImpedanceThresholdLabel = new QLabel(tr("Acceptable cell impedance threshold:"));
    acceptableCellImpedanceThresholdSpinBox = new QSpinBox;
    acceptableCellImpedanceThresholdUnit = new QLabel(tr("Ω"));

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
    advancedExtensionLayout->addWidget(chargeRateLabel, 7, 0);
    advancedExtensionLayout->addWidget(chargeRateSpinBox, 7, 1);
    advancedExtensionLayout->addWidget(chargeRateUnit, 7, 2);
    advancedExtensionLayout->addWidget(dischargeRateLabel, 8, 0);
    advancedExtensionLayout->addWidget(dischargeRateSpinBox, 8, 1);
    advancedExtensionLayout->addWidget(dischargeRateUnit, 8, 2);
    advancedExtensionLayout->addWidget(storageDischargeVoltageLabel, 9, 0);
    advancedExtensionLayout->addWidget(storageDischargeVoltageSpinBox, 9, 1);
    advancedExtensionLayout->addWidget(storageDischargeVoltageUnit, 9, 2);
    advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdLabel, 10, 0);
    advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdSpinBox, 10, 1);
    advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdUnit, 10, 2);
    advancedConfigExtensionWidget->setLayout(advancedExtensionLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(basicConfigWidget);
    layout->addWidget(advancedConfigButton);
    layout->addWidget(advancedConfigExtensionWidget);
    // Don't smash all the buttons and labels in this layout
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    setLayout(layout);

    // Show/hide the advanced options when the Advanced button is toggled
    connect(advancedConfigButton, &QPushButton::toggled, advancedConfigExtensionWidget, &QWidget::setVisible);
}
