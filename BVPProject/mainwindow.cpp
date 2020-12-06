#include "mainwindow.h"
#include "mainwidget.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MainWidget* mainWgt = new MainWidget(this);
    setCentralWidget(mainWgt);
    /*QMenu* menuFile;
    menuFile = menuBar()->addMenu("&Файл");
    QAction* newFile = new QAction("&Создать документ", this);
    newFile->setShortcut(tr("CTRL+A"));
    connect(newFile, SIGNAL(triggered()), SLOT(slotNewFile()));
    QAction* readFile = new QAction("&Просмотр документа", this);
    connect(readFile, SIGNAL(triggered()), SLOT(slotReadFile()));
    QAction* updateFile = new QAction("&Изменить документ", this);
    updateFile->setShortcut(tr("F4"));
    connect(updateFile,SIGNAL(triggered()), SLOT(slotUpdateFile()));

    menuFile->addAction(newFile);
    menuFile->addAction(readFile);
    menuFile->addAction(updateFile);

    QMenu* menuJournal;
    menuJournal = menuBar()->addMenu("&Журналы");
    QAction* mainJournal = new QAction("&Главный Журнал", this);
    connect(mainJournal, SIGNAL(triggered()), SLOT(slotShowMainJournal()));
    menuJournal->addAction(mainJournal);


    QMenu* menuHelp;
    menuHelp = menuBar()->addMenu("&Помощь");
    QAction* help = new QAction("&Справка", this);
    connect(help, SIGNAL(triggered()), SLOT(slotShowHelp()));

    menuHelp->addAction(help);
    */
    /*QToolBar* toolbar = new QToolBar("main toolbar", this);
    addToolBar(toolbar);
    toolbar->addAction(newFile);
    toolbar->addAction(readFile);
    toolbar->addAction(updateFile);
    statusBar()->showMessage("Ready");
    */
}

MainWindow::~MainWindow()
{

}
/*
void MainWindow::slotNewFile(){

}

void MainWindow::slotReadFile(){

}

void MainWindow::slotUpdateFile(){

}

void MainWindow::slotShowMainJournal(){

}

void MainWindow::slotShowHelp(){

}
*/
