#include "calculatormaterialsdialog.h"
#include "calculatormaterialsitem.h"
#include "calculatormaterialstreemodel.h"
#include "calculatortreeitem.h"
#include "calculatordatabase.h"
#include "calculatormaterialsdelegate.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QToolBar>
#include <QHeaderView>
#include <QAction>

#include <QDebug>

CalculatorMaterialsDialog::CalculatorMaterialsDialog(QWidget* parent, Qt::WindowFlags f):QDialog(parent, f)
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    resize(400, 400);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QToolBar* toolBar = new QToolBar(this);
    selItem = new QAction(QIcon("../Calculator/select2.png").pixmap(25,25),"selectItem", toolBar);
    newAct = new QAction(QIcon("../Calculator/newFile.jpg").pixmap(25,25),"new", toolBar);
    editAct = new QAction(QIcon("../Calculator/EditFile.png").pixmap(25,25),"edit", toolBar);
    newFolder = new QAction(QIcon("../Calculator/folder2.png").pixmap(25,25),"newFolder", toolBar);
    delItem = new QAction("delete", toolBar);
    treeTypeModel = new QAction(QIcon("../Calculator/typeModel5.png").pixmap(25,25),"chTypeModel",toolBar);
    editListAct = new QAction(QIcon("../Calculator/editStyle.png").pixmap(25,25),"EditList", toolBar);
    acceptAct = new QAction(QIcon("../Calculator/acceptElem.png").pixmap(25,25),"Accept", toolBar);
    rejectAct = new QAction(QIcon("../Calculator/editElem.png").pixmap(25,25),"Reject", toolBar);
    QList<QKeySequence> shCuts;
    shCuts<<Qt::Key_Enter<<Qt::Key_Return;
    selItem->setShortcuts(shCuts);
    newAct->setShortcut(Qt::Key_Insert);
    editAct->setShortcut(Qt::Key_F4);
    delItem->setShortcut(Qt::Key_Delete);
    newAct->setEnabled(false);
    editAct->setEnabled(false);
    newFolder->setEnabled(false);
    delItem->setEnabled(false);
    treeTypeModel->setCheckable(true);
    editListAct->setCheckable(true);
    acceptAct->setEnabled(false);
    rejectAct->setEnabled(false);
    toolBar->addAction(selItem);
    toolBar->addAction(newAct);
    toolBar->addAction(editAct);
    toolBar->addAction(newFolder);
    toolBar->addAction(delItem);
    toolBar->addAction(treeTypeModel);
    toolBar->addAction(editListAct);
    toolBar->addAction(acceptAct);
    toolBar->addAction(rejectAct);
    view = new QTableView(this);
    vbx->addWidget(toolBar);
    vbx->addWidget(view);
    QVector<QVariant> header;
    header<<""
           <<"Parent"
           <<"Folder"
           <<"Наименование"
           <<"Ед.Изм."
           <<"Цена";

    model = new CalculatorMaterialsTreeModel(TABLE_MATERIALS, header, this);
    model->setupModelData();
    model->setColumnEditable(5);
    model->setSortCol(3);
    view->setModel(model);
    viewSettings();
    connect(newAct, SIGNAL(triggered()),this, SLOT(createItem()));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));
    connect(newFolder, SIGNAL(triggered()), this, SLOT(createFolder()));
    //connect(delItem, SIGNAL(triggered()), this, SLOT(deleteItem()));
    connect(selItem, SIGNAL(triggered()), this, SLOT(selectItem()));
    connect(treeTypeModel, SIGNAL(toggled(bool)), this, SLOT(treeTypeModelSlot(bool)));
    connect(editListAct, SIGNAL(toggled(bool)), this, SLOT(editListActSlot(bool)));
    connect(acceptAct, SIGNAL(triggered()), model, SLOT(reserveDataAccept()));
    connect(rejectAct, SIGNAL(triggered()), model, SLOT(reserveDataReject()));
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
    connect(model, SIGNAL(reserveDataChanged()), this, SLOT(reserveDataChangedSlot()));
    connect(model, SIGNAL(reserveDataAccepted()), this, SLOT(acceptActSlot()));
    connect(model, SIGNAL(reserveDataRejected()), this, SLOT(rejectActSlot()));
}

void CalculatorMaterialsDialog::viewSettings(){
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setColumnHidden(1,true);
    view->setColumnHidden(2,true);
    //view->setColumnHidden(6,true);
    view->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    view->setItemDelegateForColumn(5, new CalculatorMaterialsDelegate(this));
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setCurrentIndex(model->index(0,1,QModelIndex()));
}

void CalculatorMaterialsDialog::createItem(){
    QVector<QVariant> itemData = {0,model->currentRoot()->data(FieldName::FolderId),0};
    itemData.reserve(6);
    CalculatorMaterialsItem* item = new CalculatorMaterialsItem(itemData, this);
    if(item->exec()){
        model->createModelItem(itemData);
    }
}

void CalculatorMaterialsDialog::createFolder(){
    QVector<QVariant> itemData = {0,model->currentRoot()->data(FieldName::FolderId),1};
    itemData.reserve(6);
    CalculatorMaterialsItem* item = new CalculatorMaterialsItem(itemData, this);
    if(item->exec()){
        model->createModelItem(itemData);
    }
}

void CalculatorMaterialsDialog::editItem(){
    QModelIndex index = view->currentIndex();
    if(!index.isValid())
        return;
    QVector<QVariant> itemData = model->getRowData(index);
    CalculatorMaterialsItem* item = new CalculatorMaterialsItem(itemData, this);
    if(item->exec()){
        model->updateModelItem(itemData, index);
    }
}

void CalculatorMaterialsDialog::selectItem(){
    QModelIndex index = view->currentIndex();
    if(!index.isValid())
        return;
    emit view->doubleClicked(index);
}

void CalculatorMaterialsDialog::treeTypeModelSlot(bool value){
    if(editListAct->isChecked())
        return;
    newAct->setEnabled(value);
    newFolder->setEnabled(value);
    editAct->setEnabled(value);
    editListAct->setEnabled(!value);

    model->setTreeMode(value);
}

void CalculatorMaterialsDialog::editListActSlot(bool value){
    if(treeTypeModel->isChecked())
        return;
    treeTypeModel->setEnabled(!value);
    selItem->setEnabled(!value);
    model->setEditMode(value);
    //view->setColumnHidden(6,!value);
    if(value){
        disconnect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
    }else{
        connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
    }
}

void CalculatorMaterialsDialog::reserveDataChangedSlot(){
    acceptAct->setEnabled(true);
    rejectAct->setEnabled(true);
    editListAct->setEnabled(false);
}

void CalculatorMaterialsDialog::acceptActSlot(){
    acceptAct->setEnabled(false);
    rejectAct->setEnabled(false);
    editListAct->setEnabled(true);
}

void CalculatorMaterialsDialog::rejectActSlot(){
    acceptAct->setEnabled(false);
    rejectAct->setEnabled(false);
    editListAct->setEnabled(true);
}
