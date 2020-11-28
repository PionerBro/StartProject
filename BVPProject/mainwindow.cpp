#include "mainwindow.h"
#include "mainjournal.h"
#include "order.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QPushButton>
#include <QFrame>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mnj(new MainJournal)
{

    QMenu* menuFile;
    menuFile = menuBar()->addMenu("&Файл");
    QAction* newFile = new QAction("&Создать документ", this);
    newFile->setShortcut(tr("INSERT"));
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


    QToolBar* toolbar = new QToolBar("main toolbar", this);
    addToolBar(toolbar);
    toolbar->addAction(newFile);
    toolbar->addAction(readFile);
    toolbar->addAction(updateFile);


    statusBar()->showMessage("Ready");
    addDockWidget(Qt::TopDockWidgetArea, mnj);
    mnj->hide();
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotNewFile(){
    Order* order = new Order(this);
    connect(order, SIGNAL(signalNewDoc(QStringList)), mnj, SLOT(slotNewDoc(QStringList)));
    setCentralWidget(order);
}

void MainWindow::slotReadFile(){

}

void MainWindow::slotUpdateFile(){

}

void MainWindow::slotShowMainJournal(){
    mnj->show();
}

void MainWindow::slotShowHelp(){

}
