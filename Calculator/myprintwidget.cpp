#include "myprintwidget.h"
#include "myprintwidgettable.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

MyPrintWidget::MyPrintWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    resize(740,600);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    MyPrintWidgetTable* tableView = new MyPrintWidgetTable(this);
    vbx->addWidget(tableView);
    QToolBar* toolBar = new QToolBar(this);
    vbx->addWidget(toolBar);
    QAction* viewAct = new QAction("Ch",this);
    toolBar->addAction(viewAct);
}
