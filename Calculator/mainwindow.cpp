#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTreeView>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QHeaderView>
#include "mytreemodel.h"
#include <QTableView>
#include <QToolBar>
#include <QAction>
#include "directorywidget.h"
#include "calcitem.h"
#include "mydatabase.h"
#include "calcwidget.h"
#include <QMenu>
#include <QMenuBar>
#include <QApplication>

#include <QDebug>

MyDataBase db;

CalcWidget* calcWidget;
DirectoryWidget* dirWidget;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{    
    QString str = "9";
    double d = str.toDouble();
    str = QString::number(d,'f', 2);
    qDebug()<<str;
    //setStyleSheet("background-color: #1f4037; color: #ffff4d");
    if(!db.createConnection())
        exit(-1);
    calcWidget = new CalcWidget();
    dirWidget  = new DirectoryWidget();
    //calcWidget->setStyleSheet("background-color: #1f4037; color: #ffff4d");
    //dirWidget->setStyleSheet("background-color: #1f4037; color: #ffff4d");
    QMenu* menu = new QMenu(tr("Журналы"), this);
    QMenu* menuLists = new QMenu(tr("Списки"),this);
    menuBar()->addMenu(menu);
    menuBar()->addMenu(menuLists);
    QAction* actCalcJournal = new QAction(tr("Калькуляции"), menu);
    QAction* actDirItemsJournal = new QAction(tr("Сырье"), menu);
    QAction* actDirItemsLists = new QAction(tr("Цена сырья"),this);
    menu->addAction(actCalcJournal);
    menu->addAction(actDirItemsJournal);
    menuLists->addAction(actDirItemsLists);

    connect(actCalcJournal, SIGNAL(triggered()), calcWidget, SLOT(show()));
    connect(actDirItemsJournal, SIGNAL(triggered()), dirWidget, SLOT(show()));
    //connect(actDirItemsLists, SIGNAL(triggered()), );


}


MainWindow::~MainWindow()
{
    delete calcWidget;
    delete dirWidget;
}


