#include "calcwidget.h"
#include "mytreemodel.h"

#include <QToolBar>
#include <QAction>
#include <QTableView>
#include <QVBoxLayout>

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
    list <<"Date"
         <<"Num"
         <<"Name"
         <<"Price";

    MyTreeModel* model = new MyTreeModel(list, MyTreeModel::Materials, this);
    view->setModel(model);
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
}
