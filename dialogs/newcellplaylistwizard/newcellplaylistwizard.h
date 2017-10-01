#ifndef NEWCELLPLAYLISTWIZARD_H
#define NEWCELLPLAYLISTWIZARD_H

#include <QWizard>
#include "globals.h"

class QLabel;
class QLineEdit;
class QGroupBox;
class QRadioButton;
class QSpinBox;
class QCheckBox;

class NewCellPlaylistWizard : public QWizard
{
    Q_OBJECT

public:
    NewCellPlaylistWizard(QWidget *parent = 0);

    void accept() override;
};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};

class BasicSetupPage : public QWizardPage
{
    Q_OBJECT

public:
    BasicSetupPage(QWidget *parent = 0);
    void updateExampleCellName();

private:
    QLabel *cellPlaylistNameLabel;
    QLineEdit *cellPlaylistNameLineEdit;
    QGroupBox *selectChemistryBox;
    QRadioButton *lipoRadioButton;
    QRadioButton *ironPhosphateRadioButton;
    QRadioButton *otherRadioButton;
    QLabel *sameTypeLabel;
    QLabel *numCellsLabel;
    QSpinBox *numCellsSpinBox;
    QLabel *cellDesignatorLabel;
    QLineEdit *cellDesignatorLineEdit;
    QLabel *startingCellNumberLabel;
    QSpinBox *startingCellNumberSpinBox;
    QLabel *exampleCellNameLabel;
    QLabel *exampleCellName;
};

class ConfigPlaylistPage : public QWizardPage
{
    Q_OBJECT

public:
    ConfigPlaylistPage(QWidget *parent = 0);
    void initializePage();

private:
    QLabel *numWarmupCyclesLabel;
    QSpinBox *numWarmupCyclesSpinBox;

    QLabel *numMeasurementCyclesLabel;
    QSpinBox *numMeasurementCyclesSpinBox;

    QCheckBox *storageDischargeCheckBox;

    QWidget *advancedConfigExtensionWidget;

    QLabel *restPeriodLabel;
    QDoubleSpinBox *restPeriodSpinBox;
    QLabel *restPeriodUnit;

    QLabel *highVoltageCutoffLabel;
    QDoubleSpinBox *highVoltageCutoffSpinBox;
    QLabel *highVoltageCutoffUnit;

    QLabel *lowVoltageCutoffLabel;
    QDoubleSpinBox *lowVoltageCutoffSpinBox;
    QLabel *lowVoltageCutoffUnit;

    QLabel *chargeTemperatureCutoffLabel;
    QDoubleSpinBox *chargeTemperatureCutoffSpinBox;
    QLabel *chargeTemperatureCutoffUnit;

    QLabel *dischargeTemperatureCutoffLabel;
    QDoubleSpinBox *dischargeTemperatureCutoffSpinBox;
    QLabel *dischargeTemperatureCutoffUnit;

    QLabel *chargeCurrentSafetyCutoffLabel;
    QDoubleSpinBox *chargeCurrentSafetyCutoffSpinBox;
    QLabel *chargeCurrentSafetyCutoffUnit;

    QLabel *dischargeCurrentSafetyCutoffLabel;
    QDoubleSpinBox *dischargeCurrentSafetyCutoffSpinBox;
    QLabel *dischargeCurrentSafetyCutoffUnit;

    QLabel *chargeRateLabel;
    QDoubleSpinBox *chargeRateSpinBox;
    QLabel *chargeRateUnit;

    QLabel *dischargeRateLabel;
    QDoubleSpinBox *dischargeRateSpinBox;
    QLabel *dischargeRateUnit;

    QLabel *storageDischargeVoltageLabel;
    QDoubleSpinBox *storageDischargeVoltageSpinBox;
    QLabel *storageDischargeVoltageUnit;

    QLabel *acceptableCellImpedanceThresholdLabel;
    QDoubleSpinBox *acceptableCellImpedanceThresholdSpinBox;
    QLabel *acceptableCellImpedanceThresholdUnit;

    QWidget *basicConfigWidget;
    QPushButton *advancedConfigButton;
};

#endif // NEWCELLPLAYLISTWIZARD_H
