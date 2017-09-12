#include <QtWidgets>

#include "newcellplaylistwizard.h"

void NewCellPlaylistWizard::accept()
{

}

NewCellPlaylistWizard::NewCellPlaylistWizard(QWidget *parent) : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new BasicSetupPage);

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

BasicSetupPage::BasicSetupPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Basic Setup"));
    setSubTitle(tr("Provide basic setup information for a new cell playlist."));

    cellPlaylistNameLabel = new QLabel(tr("Playlist Name:"));
    cellPlaylistNameLineEdit = new QLineEdit;
    // TODO validate this is a valid filename

    groupBox = new QGroupBox(tr("Select a cell chemistry type"));
    lipoRadioButton = new QRadioButton(tr("Lithium Polymer (also called Lithium-Ion Polymer, LiPo, LIP or Li-poly)"));
    ironPhosphateRadioButton = new QRadioButton(tr(qPrintable(QString::fromUtf8("Lithium Iron Phosphate (also called LiFePO\u2084 or LFP)"))));
    otherRadioButton = new QRadioButton(tr("Other"));

    lipoRadioButton->setChecked(true);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(lipoRadioButton);
    groupBoxLayout->addWidget(ironPhosphateRadioButton);
    groupBoxLayout->addWidget(otherRadioButton);
    groupBox->setLayout(groupBoxLayout);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(cellPlaylistNameLabel, 0, 0);
    layout->addWidget(cellPlaylistNameLineEdit, 0, 1);
    layout->addWidget(groupBox, 1, 0, 1, 2);
    setLayout(layout);
}
