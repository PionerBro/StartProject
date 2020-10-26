#include "mainwindow.h"
#include "menu.h"
#include "registration.h"
#include "welcomepage.h"
#include <QApplication>

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
    QWidget* tmp = new Menu;
    tmp->show();
    //wgt->hide();
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
