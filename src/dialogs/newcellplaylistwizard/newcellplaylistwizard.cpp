#include "newcellplaylistwizard.h"

void NewCellPlaylistWizard::accept() {
  QDialog::accept();
  emit finished(m_playlist);
  // TODO load changes even if they were not saved to a file
}

NewCellPlaylistWizard::NewCellPlaylistWizard(QWidget *parent)
    : QWizard(parent) {
  skipped = false;
  m_playlist = CellPlaylist();
  // The fields registered do not by default know what value and signal
  // to use with a QDoubleSpinBox, so we have to tell it how to handle those.
  // Why it does not have the same default behavior as QSpinBox I do not know.
  this->setDefaultProperty("QDoubleSpinBox", "value", "valueChanged");

  addPage(new IntroPage);
  addPage(new BasicSetupPage);
  addPage(new ConfigPlaylistPage);
  addPage(new PlaylistDirectoryPage);

  // Feels hackish, but I don't have access to internet and couldn't get child
  // page to connect to parent wizard So, I am doing it in reverse
  SavePlaylistPage *savePlaylistPage = new SavePlaylistPage;
  addPage(savePlaylistPage);
  connect(savePlaylistPage->skipButton, &QPushButton::clicked, this,
          &NewCellPlaylistWizard::skipToNextPage);

  addPage(new FinishPlaylistPage);

  // TODO set this to good size once wizard implementation complete
  this->setMinimumSize(500, 650);

  setWindowTitle(tr("New Cell Playlist"));

  connect(this, &QWizard::currentIdChanged, this,
          &NewCellPlaylistWizard::savePlaylist);
}

IntroPage::IntroPage(QWidget *parent) : QWizardPage(parent) {
  setTitle(tr("Introduction"));

  label = new QLabel(
      tr("This wizard will generate a playlist file, which is "
         "a list of cells with information about the cells and"
         " the tests you would like to run on them. Once "
         "you create a playlist, you can load that playlist and"
         " execute the tests on your battery cells to generate results."));
  label->setWordWrap(true);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(label);
  setLayout(layout);
}

void BasicSetupPage::updateExampleCellName() {
  QString designator = this->cellDesignatorLineEdit->text();
  int numCells = this->numCellsSpinBox->value();
  int startingCellNumber = this->startingCellNumberSpinBox->value();

  QString cellStr = BatlabLib::cellName(designator, numCells,
                                        startingCellNumber, startingCellNumber);

  this->exampleCellName->setText(cellStr);
}

BasicSetupPage::BasicSetupPage(QWidget *parent) : QWizardPage(parent) {
  setTitle(tr("Basic Setup"));
  setSubTitle(tr("Provide basic setup information for a new cell playlist."));

  cellPlaylistNameLabel = new QLabel(tr("Playlist name:"));
  cellPlaylistNameLineEdit = new QLineEdit;
  // Only valid characters
  QRegExp cellPlaylistNameRx("^[ \\w\\-\\.]+$");
  QValidator *cellPlaylistNameValidator =
      new QRegExpValidator(cellPlaylistNameRx);
  cellPlaylistNameLineEdit->setValidator(cellPlaylistNameValidator);
  registerField(QString(CELL_PLAYLIST_NAME_FIELDSTR) + "*",
                cellPlaylistNameLineEdit);
  // TODO trim whitespace from strings in fields with simplified()

  selectChemistryBox = new QGroupBox(tr("Cell chemistry type"));
  lipoRadioButton = new QRadioButton(
      tr("3.7V Lithium-Ion (LiPo, LIP ,Li-Poly, LCO, LMO, NMC, NCA)"));
  registerField(LIPO_CHEMISTRY_FIELDSTR, lipoRadioButton);
  ironPhosphateRadioButton =
      new QRadioButton(tr("3.2V Lithium Iron Phosphate (LiFePO4, LFP)"));
  registerField(IRON_PHOSPHATE_CHEMISTRY_FIELDSTR, ironPhosphateRadioButton);
  otherRadioButton = new QRadioButton(tr("Other"));
  registerField(OTHER_CHEMISTRY_FIELDSTR, otherRadioButton);

  lipoRadioButton->setChecked(true);

  QVBoxLayout *groupBoxLayout = new QVBoxLayout;
  groupBoxLayout->addWidget(lipoRadioButton);
  groupBoxLayout->addWidget(ironPhosphateRadioButton);
  groupBoxLayout->addWidget(otherRadioButton);
  selectChemistryBox->setLayout(groupBoxLayout);

  sameTypeLabel = new QLabel(
      tr("Please note that all cells in a playlist must be of the same type."));
  sameTypeLabel->setWordWrap(true);

  numCellsLabel = new QLabel(tr("Number of cells:"));
  numCellsSpinBox = new QSpinBox;
  numCellsSpinBox->setMinimum(MINIMUM_NUM_CELLS);
  numCellsSpinBox->setMaximum(MAXIMUM_NUM_CELLS);
  registerField(NUM_CELLS_FIELDSTR, numCellsSpinBox);

  cellDesignatorLabel = new QLabel(tr("Cell designator:"));
  cellDesignatorLineEdit = new QLineEdit;
  cellDesignatorLineEdit->setText("CELL");
  // Only valid characters
  QRegExp cellDesignatorRx("^[\\w\\-\\.]+$");
  QValidator *cellDesignatorValidator = new QRegExpValidator(cellDesignatorRx);
  cellDesignatorLineEdit->setValidator(cellDesignatorValidator);
  registerField(CELL_DESIGNATOR_FIELDSTR, cellDesignatorLineEdit);
  // TODO trim whitespace from strings in fields with simplified()

  startingCellNumberLabel = new QLabel(tr("Starting cell number:"));
  startingCellNumberSpinBox = new QSpinBox;
  startingCellNumberSpinBox->setMinimum(MINIMUM_STARTING_CELL_NUMBER);
  startingCellNumberSpinBox->setMaximum(MAXIMUM_STARTING_CELL_NUMBER);
  registerField(STARTING_CELL_NUMBER_FIELDSTR, startingCellNumberSpinBox);

  exampleCellNameLabel = new QLabel(tr("Example cell name:"));
  exampleCellName = new QLabel;

  // Update the example cell name when the designator or starting cell number
  // are changed
  connect(cellDesignatorLineEdit, &QLineEdit::textChanged, this,
          &BasicSetupPage::updateExampleCellName);
  // The static_cast is needed for these because QSpinBox overloads valueChanged
  // and we need to specify which one we want
  // https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
  connect(startingCellNumberSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
          &BasicSetupPage::updateExampleCellName);
  connect(numCellsSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
          &BasicSetupPage::updateExampleCellName);
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

void ConfigPlaylistPage::initializePage() {
  static QString cellChemistryTypeStr = "";

  QString newCellChemistryTypeStr = "";
  if (field(LIPO_CHEMISTRY_FIELDSTR) == true) {
    newCellChemistryTypeStr = "lipo";
  } else if (field(IRON_PHOSPHATE_CHEMISTRY_FIELDSTR) == true) {
    newCellChemistryTypeStr = "ironPhosphate";
  } else if (field(OTHER_CHEMISTRY_FIELDSTR) == true) {
    newCellChemistryTypeStr = "other";
  }

  // If they have changed the cell chemistry type, update the defaults and
  // bounds for those values which are affected We do this check, so that if
  // someone keeps the same cell chemistry, changes an advanced value, goes back
  // to the basic setup page and then again to the advanced page, as long as
  // they have not changed the cell chemistry type we don't want to rewrite
  // their settings.
  if (newCellChemistryTypeStr != cellChemistryTypeStr) {
    cellChemistryTypeStr = newCellChemistryTypeStr;
    if (cellChemistryTypeStr == "ironPhosphate") {
      highVoltageCutoffSpinBox->setMaximum(
          IRON_PHOSPHATE_HIGH_VOLTAGE_CUTOFF_MAX);
      highVoltageCutoffSpinBox->setValue(
          IRON_PHOSPHATE_HIGH_VOLTAGE_CUTOFF_DEFAULT);
      lowVoltageCutoffSpinBox->setValue(
          IRON_PHOSPHATE_LOW_VOLTAGE_CUTOFF_DEFAULT);
      storageDischargeVoltageSpinBox->setValue(
          IRON_PHOSPHATE_STORAGE_DISCHARGE_VOLTAGE_DEFAULT);
    } else {
      highVoltageCutoffSpinBox->setMaximum(HIGH_VOLTAGE_CUTOFF_MAX);
      highVoltageCutoffSpinBox->setValue(HIGH_VOLTAGE_CUTOFF_DEFAULT);
      lowVoltageCutoffSpinBox->setValue(LOW_VOLTAGE_CUTOFF_DEFAULT);
      storageDischargeVoltageSpinBox->setValue(
          STORAGE_DISCHARGE_VOLTAGE_DEFAULT);
    }
  }
}

// Only allow the storage discharge voltage to be set if a storage discharge
// phase is desired
void ConfigPlaylistPage::enableOrDisableStorageDischargeField() {
  if (storageDischargeCheckBox->isChecked()) {
    storageDischargeVoltageSpinBox->setEnabled(true);
  } else {
    storageDischargeVoltageSpinBox->setEnabled(false);
  }
}

// When certain values are updated, other fields need to have their limits
// adjusted to keep some values from being greater than others
void ConfigPlaylistPage::updateDynamicFieldBounds() {
  prechargeRateSpinBox->setMaximum(chargeCurrentSafetyCutoffSpinBox->value());
  chargeRateSpinBox->setMaximum(chargeCurrentSafetyCutoffSpinBox->value());
  dischargeRateSpinBox->setMaximum(
      dischargeCurrentSafetyCutoffSpinBox->value());
  storageDischargeVoltageSpinBox->setMaximum(highVoltageCutoffSpinBox->value());
}

ConfigPlaylistPage::ConfigPlaylistPage(QWidget *parent) : QWizardPage(parent) {
  setTitle(tr("Configure Playlist Settings"));
  setSubTitle(tr("Configure the specific settings of your playlist."));

  basicConfigWidget = new QWidget;

  numWarmupCyclesLabel = new QLabel(tr("Number of warmup cycles:"));
  numWarmupCyclesSpinBox = new QSpinBox;
  numWarmupCyclesSpinBox->setMinimum(NUM_WARMUP_CYCLES_MIN);
  numWarmupCyclesSpinBox->setMaximum(NUM_WARMUP_CYCLES_MAX);
  numWarmupCyclesSpinBox->setValue(NUM_WARMUP_CYCLES_DEFAULT);
  registerField(NUM_WARMUP_CYCLES_FIELDSTR, numWarmupCyclesSpinBox);

  numMeasurementCyclesLabel = new QLabel(tr("Number of measurement cycles:"));
  numMeasurementCyclesSpinBox = new QSpinBox;
  numMeasurementCyclesSpinBox->setMinimum(NUM_MEASUREMENT_CYCLES_MIN);
  numMeasurementCyclesSpinBox->setMaximum(NUM_MEASUREMENT_CYCLES_MAX);
  numMeasurementCyclesSpinBox->setValue(NUM_MEASUREMENT_CYCLES_DEFAULT);
  registerField(NUM_MEASUREMENT_CYCLES_FIELDSTR, numMeasurementCyclesSpinBox);

  storageDischargeCheckBox =
      new QCheckBox(tr("Discharge to storage voltage after testing"));
  storageDischargeCheckBox->setChecked(STORAGE_DISCHARGE_DEFAULT);
  connect(storageDischargeCheckBox, &QCheckBox::toggled, this,
          &ConfigPlaylistPage::enableOrDisableStorageDischargeField);
  registerField(STORAGE_DISCHARGE_FIELDSTR, storageDischargeCheckBox);

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
  restPeriodSpinBox = new QDoubleSpinBox;
  restPeriodUnit = new QLabel(tr("sec"));
  restPeriodSpinBox->setMinimum(REST_PERIOD_MIN);
  restPeriodSpinBox->setMaximum(REST_PERIOD_MAX);
  restPeriodSpinBox->setValue(REST_PERIOD_DEFAULT);
  registerField(REST_PERIOD_FIELDSTR, restPeriodSpinBox);

  highVoltageCutoffLabel = new QLabel(tr("High voltage cutoff:"));
  highVoltageCutoffSpinBox = new QDoubleSpinBox;
  highVoltageCutoffUnit = new QLabel(tr("V"));
  highVoltageCutoffSpinBox->setSingleStep(0.1);
  highVoltageCutoffSpinBox->setMinimum(HIGH_VOLTAGE_CUTOFF_MIN);
  highVoltageCutoffSpinBox->setMaximum(HIGH_VOLTAGE_CUTOFF_MAX);
  highVoltageCutoffSpinBox->setValue(HIGH_VOLTAGE_CUTOFF_DEFAULT);
  connect(highVoltageCutoffSpinBox,
          static_cast<void (QDoubleSpinBox::*)(double)>(
              &QDoubleSpinBox::valueChanged),
          this, &ConfigPlaylistPage::updateDynamicFieldBounds);
  registerField(HIGH_VOLTAGE_CUTOFF_FIELDSTR, highVoltageCutoffSpinBox);

  lowVoltageCutoffLabel = new QLabel(tr("Low voltage cutoff:"));
  lowVoltageCutoffSpinBox = new QDoubleSpinBox;
  lowVoltageCutoffUnit = new QLabel(tr("V"));
  lowVoltageCutoffSpinBox->setSingleStep(0.1);
  lowVoltageCutoffSpinBox->setMinimum(LOW_VOLTAGE_CUTOFF_MIN);
  lowVoltageCutoffSpinBox->setMaximum(LOW_VOLTAGE_CUTOFF_MAX);
  lowVoltageCutoffSpinBox->setValue(LOW_VOLTAGE_CUTOFF_DEFAULT);
  registerField(LOW_VOLTAGE_CUTOFF_FIELDSTR, lowVoltageCutoffSpinBox);

  chargeTemperatureCutoffLabel = new QLabel(tr("Charge temperature cutoff:"));
  chargeTemperatureCutoffSpinBox = new QDoubleSpinBox;
  chargeTemperatureCutoffUnit = new QLabel(tr("°C"));
  chargeTemperatureCutoffSpinBox->setMinimum(CHARGE_TEMP_CUTOFF_MIN);
  chargeTemperatureCutoffSpinBox->setMaximum(CHARGE_TEMP_CUTOFF_MAX);
  chargeTemperatureCutoffSpinBox->setValue(CHARGE_TEMP_CUTOFF_DEFAULT);
  registerField(CHARGE_TEMP_CUTOFF_FIELDSTR, chargeTemperatureCutoffSpinBox);

  dischargeTemperatureCutoffLabel =
      new QLabel(tr("Discharge temperature cutoff:"));
  dischargeTemperatureCutoffSpinBox = new QDoubleSpinBox;
  dischargeTemperatureCutoffUnit = new QLabel(tr("°C"));
  dischargeTemperatureCutoffSpinBox->setMinimum(DISCHARGE_TEMP_CUTOFF_MIN);
  dischargeTemperatureCutoffSpinBox->setMaximum(DISCHARGE_TEMP_CUTOFF_MAX);
  dischargeTemperatureCutoffSpinBox->setValue(DISCHARGE_TEMP_CUTOFF_DEFAULT);
  registerField(DISCHARGE_TEMP_CUTOFF_FIELDSTR,
                dischargeTemperatureCutoffSpinBox);

  chargeCurrentSafetyCutoffLabel =
      new QLabel(tr("Charge current safety cutoff:"));
  chargeCurrentSafetyCutoffSpinBox = new QDoubleSpinBox;
  chargeCurrentSafetyCutoffUnit = new QLabel(tr("A"));
  chargeCurrentSafetyCutoffSpinBox->setSingleStep(0.1);
  chargeCurrentSafetyCutoffSpinBox->setDecimals(3);
  chargeCurrentSafetyCutoffSpinBox->setMinimum(
      CHARGE_CURRENT_SAFETY_CUTOFF_MIN);
  chargeCurrentSafetyCutoffSpinBox->setMaximum(
      CHARGE_CURRENT_SAFETY_CUTOFF_MAX);
  chargeCurrentSafetyCutoffSpinBox->setValue(
      CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT);
  connect(chargeCurrentSafetyCutoffSpinBox,
          static_cast<void (QDoubleSpinBox::*)(double)>(
              &QDoubleSpinBox::valueChanged),
          this, &ConfigPlaylistPage::updateDynamicFieldBounds);
  registerField(CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR,
                chargeCurrentSafetyCutoffSpinBox);

  dischargeCurrentSafetyCutoffLabel =
      new QLabel(tr("Discharge current safety cutoff:"));
  dischargeCurrentSafetyCutoffSpinBox = new QDoubleSpinBox;
  dischargeCurrentSafetyCutoffUnit = new QLabel(tr("A"));
  dischargeCurrentSafetyCutoffSpinBox->setSingleStep(0.1);
  dischargeCurrentSafetyCutoffSpinBox->setDecimals(3);
  dischargeCurrentSafetyCutoffSpinBox->setMinimum(
      DISCHARGE_CURRENT_SAFETY_CUTOFF_MIN);
  dischargeCurrentSafetyCutoffSpinBox->setMaximum(
      DISCHARGE_CURRENT_SAFETY_CUTOFF_MAX);
  dischargeCurrentSafetyCutoffSpinBox->setValue(
      DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT);
  connect(dischargeCurrentSafetyCutoffSpinBox,
          static_cast<void (QDoubleSpinBox::*)(double)>(
              &QDoubleSpinBox::valueChanged),
          this, &ConfigPlaylistPage::updateDynamicFieldBounds);
  registerField(DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR,
                dischargeCurrentSafetyCutoffSpinBox);

  prechargeRateLabel = new QLabel(tr("Precharge rate:"));
  prechargeRateSpinBox = new QDoubleSpinBox;
  prechargeRateUnit = new QLabel(tr("A"));
  prechargeRateSpinBox->setSingleStep(0.1);
  prechargeRateSpinBox->setDecimals(3);
  prechargeRateSpinBox->setMinimum(PRECHARGE_RATE_MIN);
  prechargeRateSpinBox->setMaximum(
      CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT); // Max is updated dynamically to
                                             // not be greater than the charge
                                             // current safety cutoff
  prechargeRateSpinBox->setValue(PRECHARGE_RATE_DEFAULT);
  registerField(PRECHARGE_RATE_FIELDSTR, prechargeRateSpinBox);

  chargeRateLabel = new QLabel(tr("Charge rate:"));
  chargeRateSpinBox = new QDoubleSpinBox;
  chargeRateUnit = new QLabel(tr("A"));
  chargeRateSpinBox->setSingleStep(0.1);
  chargeRateSpinBox->setDecimals(3);
  chargeRateSpinBox->setMinimum(CHARGE_RATE_MIN);
  chargeRateSpinBox->setMaximum(
      CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT); // Max is updated dynamically to
                                             // not be greater than the charge
                                             // current safety cutoff
  chargeRateSpinBox->setValue(CHARGE_RATE_DEFAULT);
  registerField(CHARGE_RATE_FIELDSTR, chargeRateSpinBox);

  dischargeRateLabel = new QLabel(tr("Discharge rate:"));
  dischargeRateSpinBox = new QDoubleSpinBox;
  dischargeRateUnit = new QLabel(tr("A"));
  dischargeRateSpinBox->setSingleStep(0.1);
  dischargeRateSpinBox->setDecimals(3);
  dischargeRateSpinBox->setMinimum(DISCHARGE_RATE_MIN);
  dischargeRateSpinBox->setMaximum(
      DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT); // Max is updated dynamically to
                                                // not be greater than the
                                                // discharge current safety
                                                // cutoff
  dischargeRateSpinBox->setValue(DISCHARGE_RATE_DEFAULT);
  registerField(DISCHARGE_RATE_FIELDSTR, dischargeRateSpinBox);

  // TODO grey out if storage discharge not selected
  storageDischargeVoltageLabel = new QLabel(tr("Storage discharge voltage:"));
  storageDischargeVoltageSpinBox = new QDoubleSpinBox;
  storageDischargeVoltageUnit = new QLabel(tr("V"));
  storageDischargeVoltageSpinBox->setSingleStep(0.1);
  storageDischargeVoltageSpinBox->setRange(
      STORAGE_DISCHARGE_VOLTAGE_MIN,
      HIGH_VOLTAGE_CUTOFF_DEFAULT); // Max is updated dynamically to not be
                                    // greater than the high voltage cutoff
  storageDischargeVoltageSpinBox->setValue(STORAGE_DISCHARGE_VOLTAGE_DEFAULT);
  registerField(STORAGE_DISCHARGE_VOLTAGE_FIELDSTR,
                storageDischargeVoltageSpinBox);

  acceptableCellImpedanceThresholdLabel =
      new QLabel(tr("Acceptable cell impedance threshold:"));
  acceptableCellImpedanceThresholdSpinBox = new QDoubleSpinBox;
  acceptableCellImpedanceThresholdUnit = new QLabel(tr("Ω"));
  acceptableCellImpedanceThresholdSpinBox->setSingleStep(0.1);
  acceptableCellImpedanceThresholdSpinBox->setRange(
      ACCEPTABLE_IMPEDANCE_THRESHOLD_MIN, ACCEPTABLE_IMPEDANCE_THRESHOLD_MAX);
  acceptableCellImpedanceThresholdSpinBox->setValue(
      ACCEPTABLE_IMPEDANCE_THRESHOLD_DEFAULT);
  registerField(ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR,
                acceptableCellImpedanceThresholdSpinBox);

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
  advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdLabel, 11,
                                     0);
  advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdSpinBox,
                                     11, 1);
  advancedExtensionLayout->addWidget(acceptableCellImpedanceThresholdUnit, 11,
                                     2);
  advancedConfigExtensionWidget->setLayout(advancedExtensionLayout);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(basicConfigWidget);
  layout->addWidget(advancedConfigButton);
  layout->addWidget(advancedConfigExtensionWidget);
  // Don't smash all the buttons and labels in this layout
  layout->setSizeConstraint(QLayout::SetMinimumSize);
  setLayout(layout);

  // Show/hide the advanced options when the Advanced button is toggled
  connect(advancedConfigButton, &QPushButton::toggled,
          advancedConfigExtensionWidget, &QWidget::setVisible);
}

PlaylistDirectoryPage::PlaylistDirectoryPage(QWidget *parent)
    : QWizardPage(parent) {
  setTitle(tr("Choose Playlist Directory"));
  setSubTitle(tr("Choose the directory that will hold the output data files "
                 "from your tests using this playlist."));

  playlistDirectoryLineEdit = new QLineEdit();
  playlistDirectoryLineEdit->setReadOnly(true);
  registerField(PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR, playlistDirectoryLineEdit);
  playlistDirectoryBrowseButton = new QPushButton(tr("Browse..."));
  connect(playlistDirectoryBrowseButton, &QPushButton::clicked, this,
          &PlaylistDirectoryPage::browseForPlaylistDirectory);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(playlistDirectoryLineEdit);
  layout->addWidget(playlistDirectoryBrowseButton);
  setLayout(layout);
}

void PlaylistDirectoryPage::initializePage() {
  QString homeDirPath =
      QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
  QString playlistDirectoryPathString =
      homeDirPath + "/" +
      field(CELL_PLAYLIST_NAME_FIELDSTR).toString().simplified();

  playlistDirectoryLineEdit->setText(playlistDirectoryPathString);
}

void PlaylistDirectoryPage::browseForPlaylistDirectory() {
  QString homeDirPath =
      QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
  QString playlistDirectoryPathString =
      homeDirPath + "/" +
      field(CELL_PLAYLIST_NAME_FIELDSTR).toString().simplified();
  // TODO fix this so that if none is selected it leaves the path instead of
  // setting it to ""
  playlistDirectoryPathString = QFileDialog::getExistingDirectory(
      this, tr("Choose playlist directory for output files:"),
      playlistDirectoryPathString);
  playlistDirectoryLineEdit->setText(playlistDirectoryPathString);
}

bool PlaylistDirectoryPage::validatePage() {
  QDir dir;
  dir.setPath(playlistDirectoryLineEdit->text());
  if (dir.exists()) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Directory Exists",
        "This Playlist directory already exists. Continue?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
      return false;
    }
  }
  if (!dir.mkpath(playlistDirectoryLineEdit->text())) {
    qWarning() << "Unable to find/make default playlist directory.";
    return false;
  }
  return true;
}

void SavePlaylistPage::initializePage() {
  QString directoryPath =
      field(PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR).toString().simplified();
  QString defaultSaveFilename = directoryPath + "/playlist.json";
  saveFilenameLineEdit->setText(defaultSaveFilename);
}

bool SavePlaylistPage::validatePage() {
  QFileInfo fileInfo(
      field(PLAYLIST_SAVE_FILENAME_FIELDSTR).toString().simplified());
  if (fileInfo.exists()) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "File Exists", "This Playlist file already exists. Continue?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
      return false;
    }
  }
  return true;
}

SavePlaylistPage::SavePlaylistPage(QWidget *parent) : QWizardPage(parent) {
  setTitle(tr("Save Playlist"));
  setSubTitle(tr("Save your playlist to a file."));

  setButtonText(QWizard::NextButton, tr("Save >"));

  saveFilenameLineEdit = new QLineEdit();
  saveFilenameLineEdit->setReadOnly(true);
  registerField(PLAYLIST_SAVE_FILENAME_FIELDSTR, saveFilenameLineEdit);
  saveFilenameBrowseButton = new QPushButton(tr("Browse..."));
  connect(saveFilenameBrowseButton, &QPushButton::clicked, this,
          &SavePlaylistPage::browseForSaveFilename);

  skipButton = new QPushButton(tr("Skip >"));

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(saveFilenameLineEdit, 0, 0);
  layout->addWidget(saveFilenameBrowseButton, 0, 1);
  layout->addWidget(skipButton, 1, 1);
  setLayout(layout);
}

void SavePlaylistPage::browseForSaveFilename() {
  QString directoryPath =
      field(PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR).toString().simplified();
  QString defaultSaveFilename = directoryPath + "/playlist.json";
  QString saveFilename = QFileDialog::getSaveFileName(
      this, tr("Save cell playlist as:"), defaultSaveFilename,
      "Batlab Cell Playlist Files (*.json);;All Files (*)");
  saveFilenameLineEdit->setText(saveFilename);
}

void NewCellPlaylistWizard::savePlaylist() {
  // This is called every time the user hits next, so we only want this when on
  // the correct page
  if (currentId() == 5) {
    // We also don't want to do this if they got to the next page with the
    // "Skip" button
    if (skipped == false) {

      m_playlist.setCellPlaylistName(
          field(CELL_PLAYLIST_NAME_FIELDSTR).toString().simplified());

      if (field(LIPO_CHEMISTRY_FIELDSTR).toBool()) {
        m_playlist.setCellChemistryType(LIPO_CHEMISTRY_FIELDSTR);
      } else if (field(IRON_PHOSPHATE_CHEMISTRY_FIELDSTR).toBool()) {
        m_playlist.setCellChemistryType(IRON_PHOSPHATE_CHEMISTRY_FIELDSTR);
      } else if (field(OTHER_CHEMISTRY_FIELDSTR).toBool()) {
        m_playlist.setCellChemistryType(OTHER_CHEMISTRY_FIELDSTR);
      }

      m_playlist.setNumWarmupCycles(field(NUM_WARMUP_CYCLES_FIELDSTR).toInt());
      m_playlist.setNumMeasurementCycles(
          field(NUM_MEASUREMENT_CYCLES_FIELDSTR).toInt());
      m_playlist.setHighVoltageCutoff(
          field(HIGH_VOLTAGE_CUTOFF_FIELDSTR).toDouble());
      m_playlist.setLowVoltageCutoff(
          field(LOW_VOLTAGE_CUTOFF_FIELDSTR).toDouble());
      m_playlist.setStorageDischarge(
          field(STORAGE_DISCHARGE_FIELDSTR).toBool());
      m_playlist.setStorageDischargeVoltage(
          field(STORAGE_DISCHARGE_VOLTAGE_FIELDSTR).toDouble());
      m_playlist.setRestPeriod(field(REST_PERIOD_FIELDSTR).toInt());
      m_playlist.setChargeTempCutoff(
          field(CHARGE_TEMP_CUTOFF_FIELDSTR).toDouble());
      m_playlist.setDischargeTempCutoff(
          field(DISCHARGE_TEMP_CUTOFF_FIELDSTR).toDouble());
      m_playlist.setChargeCurrentSafetyCutoff(
          field(CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR).toDouble());
      m_playlist.setDischargeCurrentSafetyCutoff(
          field(DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR).toDouble());
      m_playlist.setPrechargeRate(field(PRECHARGE_RATE_FIELDSTR).toDouble());
      m_playlist.setChargeRate(field(CHARGE_RATE_FIELDSTR).toDouble());
      m_playlist.setDischargeRate(field(DISCHARGE_RATE_FIELDSTR).toDouble());
      m_playlist.setAcceptableImpedanceThreshold(
          field(ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR).toDouble());
      m_playlist.setPlaylistOutputDirectory(
          field(PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR).toString());
      m_playlist.setPlaylistSaveFilename(
          field(PLAYLIST_SAVE_FILENAME_FIELDSTR).toString());

      QVector<QString> names;
      int numCells = field(NUM_CELLS_FIELDSTR).toInt();
      int startingCellNumber = field(STARTING_CELL_NUMBER_FIELDSTR).toInt();
      QString designator = field(CELL_DESIGNATOR_FIELDSTR).toString();
      for (int cellId = startingCellNumber;
           cellId < startingCellNumber + numCells; cellId++) {
        QString cellStr = BatlabLib::cellName(designator, numCells,
                                              startingCellNumber, cellId);
        names.append(cellStr);
      }
      m_playlist.setCellNames(names);

      QString saveFileName =
          field(PLAYLIST_SAVE_FILENAME_FIELDSTR).toString().simplified();
      m_playlist.write(saveFileName);
    }
    skipped = false;
  }
}

void NewCellPlaylistWizard::skipToNextPage() {
  skipped = true;
  next();
}

FinishPlaylistPage::FinishPlaylistPage(QWidget *parent) : QWizardPage(parent) {
  setTitle(tr("Finish New Playlist"));
  setSubTitle(tr("The new playlist will be loaded and can be executed on your "
                 "battery cells."));
}
