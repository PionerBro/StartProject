#include "mainwindow.h"
#include "mainmenu.h"
#include "registration.h"
#include "welcomepage.h"
#include "addwords.h"
#include <QApplication>
#include "start.h"
#include <QLabel>
#include <work.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    createWelcomePage();
}

void MainWindow::createWelcomePage(){
    wlcm = new WelcomePage;
    connect(wlcm, SIGNAL(crMainApp()), SLOT(createMainApp()));
    connect(wlcm, SIGNAL(setId(QString&)), SLOT(addId(QString&)));
    wlcm->show();
    //delete wgt;
    //wgt = tmp;
}

void MainWindow::createMainApp(){
    mnmu = new MainMenu;
    connect(mnmu, SIGNAL(crAddWords()), SLOT(createAddWordsPage()));
    connect(mnmu, SIGNAL(crStartPage()), SLOT(createExercisePage()));
    mnmu->show();
    delete wlcm;
    //wgt->hide();
    //delete wgt;
    //wgt = tmp;
}

void MainWindow::createAddWordsPage(){
    adws = new AddWords;
    connect(adws, SIGNAL(returnToMenu()), SLOT(createMainAppAdd()));
    adws->show();
    delete mnmu;

    //delete wgt;
    //wgt = tmp;
}

void MainWindow::createExercisePage(){
    strt = new Start(m_id);
    connect(strt, SIGNAL(crMainMenu()), SLOT(createMainAppEx()));
    strt->show();
    delete mnmu;
    //delete wgt;
    //wgt = tmp;
}

void MainWindow::createMainAppAdd(){
    mnmu = new MainMenu;
    connect(mnmu, SIGNAL(crAddWords()), SLOT(createAddWordsPage()));
    connect(mnmu, SIGNAL(crStartPage()), SLOT(createExercisePage()));
    mnmu->show();
    delete adws;
}

void MainWindow::createMainAppEx(){
    mnmu = new MainMenu;
    connect(mnmu, SIGNAL(crAddWords()), SLOT(createAddWordsPage()));
    connect(mnmu, SIGNAL(crStartPage()), SLOT(createExercisePage()));
    mnmu->show();
    delete strt;
}
void MainWindow::addId(QString& str){
    m_id = str;
}

