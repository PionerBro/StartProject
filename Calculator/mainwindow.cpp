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

#include <QDebug>

MyDataBase db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    if(!db.createConnection())
        exit(-1);
    QWidget* widget = new QWidget(this);
    QToolBar* tool = new QToolBar("mew", widget);
    QAction* act = new QAction("New", tool);
    tool->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(crDialog()));
    QAction* act2 = new QAction("Calc", tool);
    tool->addAction(act2);
    connect(act2, SIGNAL(triggered()), this, SLOT(crCalc()));
    setCentralWidget(widget);
    QVBoxLayout* vbx = new QVBoxLayout(widget);
    QTableView* view = new QTableView(widget);
   // view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    vbx->addWidget(tool);
    vbx->addWidget(view);
    QList<QVariant> list;
    list <<"Num"
         <<"Name"
         <<"Folder"
         <<"Parent";

    MyTreeModel* model = new MyTreeModel(list,widget);
    view->setModel(model);
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
}


MainWindow::~MainWindow()
{
}

void MainWindow::crDialog(){
    DirectoryWidget* tDialog = new DirectoryWidget(this);
    tDialog->exec();
}

void MainWindow::crCalc(){
    CalcItem* item = new CalcItem(this);
    if(item->exec()){
        qDebug()<<"Calc created";
    }else
        qDebug()<<"Calc not created";

}
