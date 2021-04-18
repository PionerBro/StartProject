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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* widget = new QWidget(this);
    setCentralWidget(widget);
    QVBoxLayout* vbx = new QVBoxLayout(widget);
    QTableView* view = new QTableView(widget);
   // view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    vbx->addWidget(view);
    QStringList list;
    list <<"1"
         <<"11"
         <<"2"
         <<"22"
         <<"3"
         <<"33"
         <<"4"
         <<"44"
         <<"5"
         <<"55";
    MyTreeModel* model = new MyTreeModel(list,widget);
    view->setModel(model);
    connect(view, SIGNAL(doubleClicked(const QModelIndex&)), model, SLOT(rootItemChanged(const QModelIndex&)));
}


MainWindow::~MainWindow()
{
}

