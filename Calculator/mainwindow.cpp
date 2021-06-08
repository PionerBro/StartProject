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
#include "calculatordatabase.h"
#include "calculatormaterialsdialog.h"
#include "calcultatorunitstablemodel.h"
#include "calculatorcalculationsdialog.h"
#include "calculatormaterialshistorydialog.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>

#include <QDebug>

MyDataBase db;
CalculatorDatabase g_db;

CalcWidget* calcWidget;
DirectoryWidget* dirWidget;
CalculatorMaterialsDialog* calcMaterialsWidget;
CalcultatorUnitsTableModel* unitsModel;
CalculatorCalculationsDialog* calcCalcsDialog;
CalculatorMaterialsHistoryDialog* calcMatHistDialog;

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
    if(!g_db.createConnection())
        exit(-1);
    calcWidget = new CalcWidget();
    dirWidget  = new DirectoryWidget();
    calcMatHistDialog = new CalculatorMaterialsHistoryDialog();
    calcMaterialsWidget = new CalculatorMaterialsDialog();
    qDebug()<<"sdd";
    unitsModel = new CalcultatorUnitsTableModel(TABLE_UNITS, {tr(""),tr("Наименование")}, this);
    unitsModel->setupModelData();
    calcCalcsDialog = new CalculatorCalculationsDialog();

    //calcWidget->setStyleSheet("background-color: #1f4037; color: #ffff4d");
    //dirWidget->setStyleSheet("background-color: #1f4037; color: #ffff4d");
    QMenu* menu = new QMenu(tr("Журналы"), this);
    QMenu* menuLists = new QMenu(tr("Списки"),this);
    menuBar()->addMenu(menu);
    menuBar()->addMenu(menuLists);
    QAction* actCalcJournal = new QAction(tr("Калькуляции"), menu);
    QAction* actDirItemsJournal = new QAction(tr("Сырье"), menu);
    QAction* actDirItemsLists = new QAction(tr("Сырье"), menuLists);
    QAction* actCalcItemsLists = new QAction(tr("Калькуляции"), menuLists);
    menu->addAction(actCalcJournal);
    menu->addAction(actDirItemsJournal);
    menuLists->addAction(actDirItemsLists);
    menuLists->addAction(actCalcItemsLists);

    connect(actCalcJournal, SIGNAL(triggered()), calcWidget, SLOT(show()));
    connect(actDirItemsJournal, SIGNAL(triggered()), dirWidget, SLOT(show()));
    connect(actDirItemsLists, SIGNAL(triggered()), calcMaterialsWidget, SLOT(show()));
    connect(actCalcItemsLists, SIGNAL(triggered()), calcCalcsDialog, SLOT(show()));
    connect(calcMaterialsWidget, SIGNAL(histActTr(qlonglong)), calcMatHistDialog, SLOT(showAtNum(qlonglong)));
    connect(calcMaterialsWidget, SIGNAL(crHistItem(QVector<QVariant>&)), calcMatHistDialog, SLOT(createItem(QVector<QVariant>&)));
}


MainWindow::~MainWindow()
{
    delete calcWidget;
    delete dirWidget;
    delete calcMaterialsWidget;
    delete calcCalcsDialog;
    delete calcMatHistDialog;
}


