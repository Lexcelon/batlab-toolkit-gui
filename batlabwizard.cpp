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

    connect(page2,SIGNAL(emitCycles(int)),page3,SLOT(onNumCycles(int)));
    connect(page2,SIGNAL(emitParms(testParms)),page3,SLOT(onTestParms(testParms)));



    QWizardPage * first = new QWizardPage();
    first->setTitle("Welcome to WIZARD");
    first->setSubTitle("Wanna wizard?");


    QGridLayout * grid = new QGridLayout();
    grid->addWidget(page1);
    QWizardPage * two = new QWizardPage();
    two->setTitle("Page1");
    two->setLayout(grid);

    QGridLayout * grid1 = new QGridLayout();
    grid1->addWidget(page2);
    QWizardPage * three = new QWizardPage();
    three->setTitle("Page2");
    three->setLayout(grid1);

    QGridLayout * grid2 = new QGridLayout();
    grid2->addWidget(page3);
    QWizardPage * four = new QWizardPage();
    four->setTitle("Page3");
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
        if (wizForLambda->currentId()==3) {
            qDebug() << "Here";
            //page2->update();
            page3->onActivate();
        }
        if (wizForLambda->currentId()==2) {
            page2->onUpdate();
        }
    }
    );

    qDebug() << connect(wiz->button(QWizard::NextButton),SIGNAL(clicked()),this,SLOT(onIDChanged()));
 //  connect(wiz->button(QWizard::NextButton),SIGNAL(clicked()),this,SLOT(onIDChanged()));
    //wiz->show();
}

batlabWizard::~batlabWizard()
{

}

void batlabWizard::onShow() {
//    wiz->show();
    wiz->showFullScreen();
}

void batlabWizard::onIDChanged() {
    qDebug() << "val";
   // if (val == 3) {
        //page3->onActivate();
    //}
}
