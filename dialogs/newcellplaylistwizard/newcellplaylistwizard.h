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
    QLabel *storageDischargeLabel;
    QLabel *restPeriodLabel;
    QSpinBox *restPeriodSpinBox;
    QWidget *advancedExtension;
    QLabel *highVoltageCutoffLabel;
    QSpinBox *highVoltageCutoffSpinBox;
    QLabel *lowVoltageCutoffLabel;
    QSpinBox *lowVoltageCutoffSpinBox;
    QLabel *chargeTemperatureCutoffLabel;
    QSpinBox *chargeTemperatureCutoffSpinBox;
    QLabel *dischargeTemperatureCutoffLabel;
    QSpinBox *dischargeTemperatureCutoffSpinBox;
    QLabel *chargeCurrentSafetyCutoffLabel;
    QSpinBox *chargeCurrentSafetyCutoffSpinBox;
    QLabel *dischargeCurrentSafetyCutoffLabel;
    QSpinBox *dischargeCurrentSafetyCurrentSpinBox;
    QLabel *chargeRateLabel;
    QSpinBox *chargeRateSpinBox;
    QLabel *dischargeRateLabel;
    QSpinBox *dischargeRateSpinBox;
    QLabel *storageDischargeVoltageLabel;
    QSpinBox *storageDischargeVoltageSpinBox;
    QLabel *acceptableImpedanceThresholdLabel;
    QSpinBox *acceptableImpedanceThresholdSpinBox;
};

#endif // NEWCELLPLAYLISTWIZARD_H
