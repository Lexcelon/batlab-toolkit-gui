#include "batlabwizard.h"

batlabWizard::batlabWizard(QObject *parent) : QObject(parent)
{
    wiz = new QWizard();

    wizardPageOne * page1 = new wizardPageOne();
    wizardPageTwo * page2 = new wizardPageTwo();
    wizardPageThree * page3 = new wizardPageThree();

    connect(page1,SIGNAL(emitDesignator(QString)),page3,SLOT(onDesignator(QString)));
    connect(page1,SIGNAL(emitNumCells(int)),page3,SLOT(onNumCells(int)));
    connect(page1,SIGNAL(emitStartInd(QString)),page3,SLOT(onStartInd(QString)));
    connect(page1,SIGNAL(emitProjectName(QString)),page3,SLOT(onProjectName(QString)));

    connect(page2,SIGNAL(emitCycles(int)),page3,SLOT(onNumCycles(int)));
    connect(page2,SIGNAL(emitParms(testParms)),page3,SLOT(onTestParms(testParms)));
    connect(page2,SIGNAL(emitCapacity(double)),page3,SLOT(onCapacity(double)));
    connect(page2,SIGNAL(emitRestTime(int)),page3,SLOT(onRestTime(int)));

    QWizardPage * first = new QWizardPage();
    first->setTitle("Welcome to WIZARD");
    first->setSubTitle("Wanna wizard?");

    QGridLayout * grid = new QGridLayout();
    grid->addWidget(page1);
    QWizardPage * two = new QWizardPage();
    two->setTitle("Project Setup");
    two->setLayout(grid);

    QGridLayout * grid1 = new QGridLayout();
    grid1->addWidget(page2);
    QWizardPage * three = new QWizardPage();
    three->setTitle("Test Parameters");
    three->setLayout(grid1);

    QGridLayout * grid2 = new QGridLayout();
    grid2->addWidget(page3);
    QWizardPage * four = new QWizardPage();
    four->setTitle("Finalize");
    four->setLayout(grid2);

    wiz->addPage(first);
    wiz->addPage(two);
    wiz->addPage(three);
    wiz->addPage(four);

    QWizard * wizForLambda = wiz;
    connect(wiz,&QWizard::currentIdChanged,[&,wizForLambda,page3,page2,page1]() {
        if (wizForLambda->currentId()==1) {
            page1->onUpdate();
        }
        if (wizForLambda->currentId()==2) {
            page2->onUpdate();
        }
        if (wizForLambda->currentId()==3) {
            page3->onActivate();
        }
    }
    );

    connect(wiz,&QWizard::finished, [&,wizForLambda,page3]() {
        if (wizForLambda->currentId()==3) {
            page3->onSaveProject();
            emit emitFinished(page3->onGetProjectName());
        }
    }
    );

    wiz->setWizardStyle(QWizard::ModernStyle);
}

batlabWizard::~batlabWizard()
{

}

void batlabWizard::onShow() {
    wiz->show();
//    wiz->showFullScreen();
}

void batlabWizard::onIDChanged() {
}
