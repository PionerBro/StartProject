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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* widget = new QWidget(this);
    QToolBar* tool = new QToolBar("mew", widget);
    QAction* act = new QAction("New", tool);
    tool->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(crDialog()));
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
