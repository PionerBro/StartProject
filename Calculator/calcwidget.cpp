#include "calcwidget.h"
#include "mytreemodel.h"
#include "mydatabase.h"
#include "calcitem.h"
#include "directorywidget.h"

#include <QToolBar>
#include <QAction>
#include <QTableView>
#include <QVBoxLayout>

#include <QDebug>

CalcWidget::CalcWidget(QWidget *parent) : QWidget(parent)
{
    QToolBar* tool = new QToolBar("mew", this);
    QAction* act = new QAction("New", tool);
    tool->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(crDialog()));
    QAction* act2 = new QAction("Calc", tool);
    tool->addAction(act2);
    connect(act2, SIGNAL(triggered()), this, SLOT(crCalc()));
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QTableView* view = new QTableView(this);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    vbx->addWidget(tool);
    vbx->addWidget(view);
    QList<QVariant> list;
    list <<"Num"
         <<"Parent"
         <<"Dir"
         <<"Date"
         <<"Name"
         <<"Price"
         <<"OutPut"
         <<"Portion";

    MyTreeModel* model = new MyTreeModel(list, TABLE_ELEMENTS, this);
    view->setModel(model);
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
}

void CalcWidget::crDialog(){
    DirectoryWidget* tDialog = new DirectoryWidget(this);
    tDialog->exec();
}

void CalcWidget::crCalc(){
    CalcItem* item = new CalcItem(this);
    if(item->exec()){
        qDebug()<<"Calc created";
    }else
        qDebug()<<"Calc not created";

}
