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

#include <QDebug>

MyDataBase db;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    if(!db.createConnection())
        exit(-1);
    CalcWidget* widget = new CalcWidget(this);
    setCentralWidget(widget);
}


MainWindow::~MainWindow()
{
}


