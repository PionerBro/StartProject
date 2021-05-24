#include "directorywidget.h"
#include "mytreemodel.h"
#include "mytreeitem.h"
#include "diritem.h"
#include "calcitem2delegate.h"
#include "mydatabase.h"
#include "myitemdelegate.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QToolBar>
#include <QHeaderView>
#include <QAction>

#include <QDebug>


DirectoryWidget::DirectoryWidget(QWidget* parent, Qt::WindowFlags f):QDialog(parent, f)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    resize(400, 400);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QToolBar* toolBar = new QToolBar(this);
    newAct = new QAction(QIcon("../Calculator/newFile.jpg").pixmap(25,25),"new", toolBar);
    editAct = new QAction(QIcon("../Calculator/EditFile.png").pixmap(25,25),"edit", toolBar);
    newFolder = new QAction(QIcon("../Calculator/folder2.png").pixmap(25,25),"newFolder", toolBar);
    delItem = new QAction("delete", toolBar);
    selItem = new QAction("selectItem", toolBar);
    chTypeModel = new QAction(QIcon("../Calculator/typeModel5.png").pixmap(25,25),"chTypeModel",toolBar);
    editListAct = new QAction(QIcon("../Calculator/editStyle.png").pixmap(25,25),"EditList", toolBar);
    acceptAct = new QAction(QIcon("../Calculator/acceptElem.png").pixmap(25,25),"Accept", toolBar);
    rejectAct = new QAction(QIcon("../Calculator/editElem.png").pixmap(25,25),"Reject", toolBar);
    newAct->setShortcut(Qt::Key_Insert);
    editAct->setShortcut(Qt::Key_F4);
    delItem->setShortcut(Qt::Key_Delete);
    QList<QKeySequence> shCuts;
    shCuts<<Qt::Key_Enter<<Qt::Key_Return;
    newAct->setEnabled(false);
    editAct->setEnabled(false);
    newFolder->setEnabled(false);
    delItem->setEnabled(false);
    selItem->setEnabled(false);
    selItem->setShortcuts(shCuts);
    chTypeModel->setCheckable(true);
    editListAct->setCheckable(true);
    acceptAct->setEnabled(false);
    rejectAct->setEnabled(false);
    toolBar->addAction(newAct);
    toolBar->addAction(editAct);
    toolBar->addAction(newFolder);
    toolBar->addAction(delItem);
    toolBar->addAction(selItem);
    toolBar->addAction(chTypeModel);
    toolBar->addAction(editListAct);
    toolBar->addAction(acceptAct);
    toolBar->addAction(rejectAct);
    view = new QTableView(this);
    vbx->addWidget(toolBar);
    vbx->addWidget(view);
    QList<QVariant> header;
    header<<""
           <<"Parent"
           <<"Folder"
           <<"Наименование"
           <<"Ед.Изм."
           <<"Цена"
            <<"";

    model = new MyTreeModel(header, TABLE_MATERIALS, this);
    view->setModel(model);
    viewSettings();
    connect(newAct, SIGNAL(triggered()),this, SLOT(createItem()));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));
    connect(newFolder, SIGNAL(triggered()), this, SLOT(createFolder()));
    connect(delItem, SIGNAL(triggered()), this, SLOT(deleteItem()));
    connect(selItem, SIGNAL(triggered()), this, SLOT(selectItem()));
    connect(chTypeModel, SIGNAL(toggled(bool)), this, SLOT(chTypeModelSlot(bool)));
    connect(editListAct, SIGNAL(toggled(bool)), this, SLOT(editListActSlot(bool)));
    connect(acceptAct, SIGNAL(triggered()), model, SLOT(viewAcceptTriggered()));
    connect(rejectAct, SIGNAL(triggered()), model, SLOT(viewRejectTriggered()));
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
    connect(model, SIGNAL(reserveDataChange(bool)), this, SLOT(reserveDataChangedSlot(bool)));
    connect(model, SIGNAL(acceptIsComplete()), this, SLOT(acceptActSlot()));
    connect(model, SIGNAL(rejectIsComplete()), this, SLOT(rejectActSlot()));
}

DirectoryWidget::~DirectoryWidget(){
    qDebug()<<"DirectoryWidget destroyed";
}

void DirectoryWidget::viewSettings(){
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setColumnHidden(1,true);
    view->setColumnHidden(2,true);
    view->setColumnHidden(6,true);
    view->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    view->setItemDelegateForColumn(5, new MyItemDelegate(this));
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setCurrentIndex(model->index(0,1,QModelIndex()));
}

void DirectoryWidget::createItem(){
    MyTreeItem* itemP = model->currentRoot();
    QList<QVariant> data;
    DirItem* item = new DirItem(itemP->data(2).toLongLong(), data, DirItem::Element ,this);
    if(item->exec()){
        qDebug()<<"DirItem created";
        model->createItem(itemP, data);
    }else
        qDebug()<<"DirItem not created";
}

void DirectoryWidget::editItem(){
    QModelIndex index = view->currentIndex();
    if(index.isValid()){
        MyTreeItem* tItem = static_cast<MyTreeItem*>(index.internalPointer());
        if(tItem->isOpen)
            return;
        QList<QVariant> data = tItem->rowData();
        DirItem* item = new DirItem(data, this);
        if(item->exec()){
            model->updateItem(tItem, data);
            qDebug()<<"DirItem changed";
        }
        else
            qDebug()<<"DirItem not changed";
    }
}

void DirectoryWidget::createFolder(){
    MyTreeItem* itemP = model->currentRoot();
    QList<QVariant> data;
    DirItem* item = new DirItem(itemP->data(2).toLongLong(), data, DirItem::Folder, this);
    if(item->exec()){
        model->createFolder(itemP, data);
        qDebug()<<"DirFolder created";
    }
    else
        qDebug()<<"DirFolder not created";
}

void DirectoryWidget::deleteItem(){
    QModelIndex index= view->currentIndex();
    if(index.isValid()){
        MyTreeItem* tItem = static_cast<MyTreeItem*>(index.internalPointer());
        if(tItem->isOpen)
            return;
    }
    qDebug()<<"deleteItem"<<index.row()<<index.column();
}

void DirectoryWidget::selectItem(){
    emit view->doubleClicked(view->currentIndex());
}

MyTreeModel* DirectoryWidget::getModel()const{
    return model;
}

void DirectoryWidget::chTypeModelSlot(bool b){
    model->setTreeModelType(b);
    newAct->setEnabled(b);
    editAct->setEnabled(b);
    newFolder->setEnabled(b);
    delItem->setEnabled(b);
    selItem->setEnabled(b);
    editListAct->setEnabled(!b);
}

void DirectoryWidget::editListActSlot(bool b){
    chTypeModel->setEnabled(!b);
    model->setEditableCol(b);
    view->setColumnHidden(6, !b);
    //if(acceptAct->isEnabled())
    //    acceptAct->setEnabled(b);
}

void DirectoryWidget::reserveDataChangedSlot(bool){
    acceptAct->setEnabled(true);
    rejectAct->setEnabled(true);
    editListAct->setEnabled(false);
}

void DirectoryWidget::acceptActSlot(){
    acceptAct->setEnabled(false);
    rejectAct->setEnabled(false);
    editListAct->setEnabled(true);
}

void DirectoryWidget::rejectActSlot(){
    acceptAct->setEnabled(false);
    rejectAct->setEnabled(false);
    editListAct->setEnabled(true);
}
