#include "calculatorcalculationsdialog.h"
#include "calculatormaterialsitem.h"
#include "calculatorcalculationstreemodel.h"
#include "calculatortreeitem.h"
#include "calculatordatabase.h"
#include "calculatormaterialsdelegate.h"
#include "calccalculationsitem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QToolBar>
#include <QHeaderView>
#include <QAction>

#include <QDebug>

CalculatorCalculationsDialog::CalculatorCalculationsDialog(QWidget* parent, Qt::WindowFlags f):QDialog(parent, f)
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
    toolBar->addAction(selItem);
    toolBar->addAction(newAct);
    toolBar->addAction(editAct);
    toolBar->addAction(newFolder);
    toolBar->addAction(delItem);
    toolBar->addAction(treeTypeModel);
    view = new QTableView(this);
    vbx->addWidget(toolBar);
    vbx->addWidget(view);
    QVector<QVariant> headers;
    headers<<""
           <<"Parent"
           <<"Folder"
           <<"Наименование"
           <<"Цена"
           <<"Выход"
            <<"Порция";
    model = new CalculatorCalculationsTreeModel(TABLE_ELEMENTS_TREE, headers, this);
    model->setupModelData();
    model->setSortCol(3);
    view->setModel(model);
    viewSettings();
    connect(newAct, SIGNAL(triggered()),this, SLOT(createItem()));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));
    connect(newFolder, SIGNAL(triggered()), this, SLOT(createFolder()));
    //connect(delItem, SIGNAL(triggered()), this, SLOT(deleteItem()));
    connect(selItem, SIGNAL(triggered()), this, SLOT(selectItem()));
    connect(treeTypeModel, SIGNAL(toggled(bool)), this, SLOT(treeTypeModelSlot(bool)));
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
}

void CalculatorCalculationsDialog::viewSettings(){
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setColumnHidden(1,true);
    view->setColumnHidden(2,true);
    view->setColumnHidden(5,true);
    view->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    view->setItemDelegateForColumn(5, new CalculatorMaterialsDelegate(this));
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setCurrentIndex(model->index(0,1,QModelIndex()));
}

void CalculatorCalculationsDialog::createItem(){
    QVector<QVariant> itemData = {0,model->currentRoot()->data(FieldName::FolderId),0};
    itemData.reserve(7);
    CalcCalculationsItem* item = new CalcCalculationsItem(this);
    if(item->exec()){
        model->createModelItem(itemData);
    }
}

void CalculatorCalculationsDialog::createFolder(){
    QVector<QVariant> itemData = {0,model->currentRoot()->data(FieldName::FolderId),1};
    itemData.reserve(7);
    CalculatorMaterialsItem* item = new CalculatorMaterialsItem(itemData, this);
    if(item->exec()){
        model->createModelItem(itemData);
    }
}

void CalculatorCalculationsDialog::editItem(){
    QModelIndex index = view->currentIndex();
    if(!index.isValid())
        return;
    QVector<QVariant> itemData = model->getRowData(index);
    CalculatorMaterialsItem* item = new CalculatorMaterialsItem(itemData, this);
    if(item->exec()){
        model->updateModelItem(itemData, index);
    }
}

void CalculatorCalculationsDialog::selectItem(){
    QModelIndex index = view->currentIndex();
    if(!index.isValid())
        return;
    emit view->doubleClicked(index);
}

void CalculatorCalculationsDialog::treeTypeModelSlot(bool value){
    newAct->setEnabled(value);
    newFolder->setEnabled(value);
    editAct->setEnabled(value);
    model->setTreeMode(value);
}
