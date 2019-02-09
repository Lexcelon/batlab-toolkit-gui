#ifndef PLAYLISTSETTINGSWIDGET_H
#define PLAYLISTSETTINGSWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QListWidget>

#include "batlabcore/cellplaylist.h"

class PlaylistSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistSettingsWidget(QWidget *parent = nullptr);
    void updateBoundsBasedOnChemistryType();
    void enableOrDisableStorageDischargeField();
    void updateDynamicFieldBounds();
    void loadPlaylist(CellPlaylist playlist);

signals:

public slots:

private:
    QLabel *cellPlaylistNameLabel;
    QLineEdit *cellPlaylistNameLineEdit;

    QGroupBox *selectChemistryBox;
    QRadioButton *lipoRadioButton;
    QRadioButton *ironPhosphateRadioButton;
    QRadioButton *otherRadioButton;
    QLabel *sameTypeLabel;

    QLabel *cellNamesListLabel;
    QListWidget *cellNamesListWidget;

    QLabel *numWarmupCyclesLabel;
    QSpinBox *numWarmupCyclesSpinBox;

    QLabel *numMeasurementCyclesLabel;
    QSpinBox *numMeasurementCyclesSpinBox;

    QCheckBox *storageDischargeCheckBox;

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

    QLabel *prechargeRateLabel;
    QDoubleSpinBox *prechargeRateSpinBox;
    QLabel *prechargeRateUnit;

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
};

#endif // PLAYLISTSETTINGSWIDGET_H
