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

ConfigPlaylistPage::ConfigPlaylistPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Configure Playlist Settings"));
    setSubTitle(tr("Configure the specific settings of your playlist."));

    QGridLayout *layout = new QGridLayout;
    setLayout(layout);
}
