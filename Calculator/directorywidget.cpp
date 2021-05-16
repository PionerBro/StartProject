#include "directorywidget.h"
#include "mytreemodel.h"
#include "mytreeitem.h"
#include "diritem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QToolBar>
#include <QHeaderView>
#include <QAction>

#include <QDebug>


DirectoryWidget::DirectoryWidget(QWidget* parent, Qt::WindowFlags f):QDialog(parent, f)
{
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QToolBar* toolBar = new QToolBar(this);
    QAction* newAct = new QAction("new", toolBar);
    newAct->setShortcut(Qt::Key_Insert);
    connect(newAct, SIGNAL(triggered()),this, SLOT(createItem()));
    QAction* editAct = new QAction("edit", toolBar);
    editAct->setShortcut(Qt::Key_F4);
    connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));
    QAction* newFolder = new QAction("newFolder", toolBar);
    connect(newFolder, SIGNAL(triggered()), this, SLOT(createFolder()));
    QAction* delItem = new QAction("delete", toolBar);
    delItem->setShortcut(Qt::Key_Delete);
    connect(delItem, SIGNAL(triggered()), this, SLOT(deleteItem()));
    QAction* selItem = new QAction("selectItem", toolBar);
    QList<QKeySequence> shCuts;
    shCuts<<Qt::Key_Enter<<Qt::Key_Return;
    selItem->setShortcuts(shCuts);
    connect(selItem, SIGNAL(triggered()), this, SLOT(selectItem()));
    toolBar->addAction(newAct);
    toolBar->addAction(editAct);
    toolBar->addAction(newFolder);
    toolBar->addAction(delItem);
    toolBar->addAction(selItem);

    view = new QTableView(this);
    vbx->addWidget(toolBar);
    vbx->addWidget(view);
    QList<QVariant> header;
    header<<"Num"
           <<"Parent"
           <<"Folder"
           <<"Name"
           <<"Unit"
           <<"Price";
    model = new MyTreeModel(header, this);
    view->setModel(model);
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
    viewSettings();
}

DirectoryWidget::~DirectoryWidget(){
    qDebug()<<"DirectoryWidget destroyed";
}

void DirectoryWidget::viewSettings(){
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    //view->setColumnHidden(0,true);
    //view->setColumnHidden(2,true);
    //view->setColumnHidden(3,true);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setCurrentIndex(model->index(0,1,QModelIndex()));
}

void DirectoryWidget::createItem(){
    QList<QVariant> data;
    MyTreeItem* itemP = model->currentRoot();
    DirItem* item = new DirItem(itemP->data(2).toLongLong(), data, this);
    if(item->exec()){
        qDebug()<<"Item created";
        model->createItem(itemP, data);
        qDebug()<<data;
    }else
        qDebug()<<"Item not created";
}

void DirectoryWidget::editItem(){
    QModelIndex index = view->currentIndex();
    if(index.isValid()){
        MyTreeItem* tItem = static_cast<MyTreeItem*>(index.internalPointer());
        QList<QVariant> data = tItem->rowData();
        DirItem* item = new DirItem(data, this);
        if(item->exec())
            qDebug()<<"Item changed";
        else
            qDebug()<<"Item not changed";
    }
}

void DirectoryWidget::createFolder(){
    QModelIndex index= view->currentIndex();
    qDebug()<<"createFolder"<<index.row()<<index.column();
}

void DirectoryWidget::deleteItem(){
    QModelIndex index= view->currentIndex();
    qDebug()<<"deleteItem"<<index.row()<<index.column();
}

void DirectoryWidget::selectItem(){
    emit view->doubleClicked(view->currentIndex());
}

MyTreeModel* DirectoryWidget::getModel()const{
    return model;
}

