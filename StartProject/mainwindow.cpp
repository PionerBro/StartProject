#include "mainwindow.h"
#include "mainmenu.h"
#include "registration.h"
#include "welcomepage.h"
#include "addwords.h"
#include <QApplication>
#include "start.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createWelcomePage();


}

void MainWindow::createWelcomePage(){
    QWidget* tmp = new WelcomePage;
    connect(tmp, SIGNAL(crMainApp()), SLOT(createMainApp()));
    tmp->show();
    delete wgt;
    wgt = tmp;
}

void MainWindow::createMainApp(){
    QWidget* tmp = new MainMenu;
    connect(tmp, SIGNAL(crAddWords()), SLOT(createAddWordsPage()));
    connect(tmp, SIGNAL(crStartPage()), SLOT(createExercisePage()));
    tmp->show();
    //wgt->hide();
    delete wgt;
    wgt = tmp;
}

void MainWindow::createAddWordsPage(){
    QWidget* tmp = new AddWords;
    connect(tmp, SIGNAL(returnToMenu()), SLOT(createMainApp()));
    tmp->show();
    delete wgt;
    wgt = tmp;
}

void MainWindow::createExercisePage(){
    QWidget* tmp = new Start;
    connect(tmp, SIGNAL(crMainMenu()), SLOT(createMainApp()));
    tmp->show();
    delete wgt;
    wgt = tmp;
}

/*void MainWindow::createRegApp(){
    QWidget* regbtn = new QPushButton("Quit");
    regbtn->setAttribute(Qt::WA_DeleteOnClose);
    connect(regbtn, SIGNAL(clicked()), regbtn, SLOT(close()));
    regbtn->show();
    wgt->hide();    

}*/
