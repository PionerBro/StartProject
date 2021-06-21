#include "mainwindow.h"
#include "Database/database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Database db;
    db.openDatabase();
}

MainWindow::~MainWindow()
{
}

