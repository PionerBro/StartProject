#include "calcwidget.h"
#include "mytreemodel.h"
#include "mydatabase.h"
#include "calcitem.h"
#include "directorywidget.h"
#include "mytreeitem.h"

#include <QToolBar>
#include <QAction>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

#include <QDebug>

CalcWidget::CalcWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlag(Qt::WindowMaximizeButtonHint);
    setWindowFlag(Qt::WindowMinimizeButtonHint);
    setWindowFlag(Qt::WindowCloseButtonHint);
    resize(800,400);
    QToolBar* tool = new QToolBar("mew", this);
    QAction* act = new QAction(QIcon("newFile.jpg").pixmap(25,25), "", tool);
    QAction* act2 = new QAction(QIcon("EditFile.png").pixmap(25,25),"", tool);
    QAction* act3 = new QAction(QIcon("folder2.png").pixmap(25,25), "", tool);
    QAction* act4 = new QAction(QIcon("typeModel5.png").pixmap(25,25),"",tool);
    act4->setCheckable(true);
    tool->addAction(act);
    tool->addAction(act2);
    tool->addAction(act3);
    tool->addAction(act4);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    view = new QTableView(this);
    vbx->addWidget(tool);
    vbx->addWidget(view);
    QList<QVariant> list;
    list <<""
         <<"Parent"
         <<"Dir"
         <<"Дата"
         <<"Наименование"
         <<"Цена"
         <<"OutPut"
         <<"Portion";

    model = new MyTreeModel(list, TABLE_ELEMENTS, this);
    view->setModel(model);
    viewSettings();

    connect(act, SIGNAL(triggered()), this, SLOT(createItem()));
    connect(act2, SIGNAL(triggered()), this, SLOT(editItem()));
    connect(act3, SIGNAL(triggered()), this, SLOT(createFolder()));
    connect(act4, SIGNAL(toggled(bool)), model, SLOT(setTreeModelType(bool)));
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));    
    connect(model, SIGNAL(sendData(QList<QVariant>&)), this, SLOT(editItem()));
    //act4->toggle();
}

void CalcWidget::viewSettings(){
    view->setColumnHidden(1,true);
    view->setColumnHidden(2,true);
    view->setColumnHidden(6,true);
    view->setColumnHidden(7,true);
    view->horizontalHeader()->setStretchLastSection(true);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setColumnWidth(0, 30);
    view->setColumnWidth(5,200);
}

void CalcWidget::createItem(){
    MyTreeItem* itemP = model->currentRoot();
    CalcItem* item = new CalcItem(itemP, CalcItem::Element, this);
    connect(item, SIGNAL(sendData(QList<QVariant>&,MyTreeItem*)), this, SLOT(addNewElement(QList<QVariant>&,MyTreeItem*)));
    item->show();
    item->raise();
    item->activateWindow();
}

void CalcWidget::addNewElement(QList<QVariant>& data, MyTreeItem* itemP){
    model->createItem(itemP, data);
}

void CalcWidget::editItem(){
    QModelIndex index = view->currentIndex();
    if(index.isValid()){
        MyTreeItem* itemT = static_cast<MyTreeItem*>(index.internalPointer());
        if(itemT->isOpen)
            return;
        CalcItem* item;
        if(itemT->data(2).toLongLong()){
            item = new CalcItem(itemT, CalcItem::Folder, this);
        }else{
            item = new CalcItem(itemT, CalcItem::Element, this);
        }
        connect(item, SIGNAL(sendData(QList<QVariant>&,MyTreeItem*)), this, SLOT(editElement(QList<QVariant>&,MyTreeItem*)));
        item->show();
        item->raise();
        item->activateWindow();
    }
}

void CalcWidget::editElement(QList<QVariant>& data, MyTreeItem* itemT){
    model->updateItem(itemT,data);
}

void CalcWidget::createFolder(){
    MyTreeItem* itemP = model->currentRoot();
    CalcItem* item = new CalcItem(itemP, CalcItem::Folder, this);
    connect(item, SIGNAL(sendData(QList<QVariant>&,MyTreeItem*)), this, SLOT(addNewFolder(QList<QVariant>&,MyTreeItem*)));
    item->show();
    item->raise();
    item->activateWindow();
}

void CalcWidget::addNewFolder(QList<QVariant>& data, MyTreeItem* itemT){
    model->createFolder(itemT,data);
}

void CalcWidget::deleteItem(){

}
void CalcWidget::selectItem(){

}

