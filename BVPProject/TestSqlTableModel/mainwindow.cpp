#include "mainwindow.h"
#include "test.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Test* widget = new Test(this);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
}

