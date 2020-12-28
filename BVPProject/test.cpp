#include "test.h"
#include "testitemtabledatabase.h"
#include "item.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QToolBar>
#include <QDebug>

Test::Test(QWidget* parent) : QDialog(parent)
{
    QToolBar* toolbar = new QToolBar(this);
    QAction* createItemAct = new QAction("New Item", this);
    connect(createItemAct, SIGNAL(triggered()),this, SLOT(createItem()));
    QAction* editItemAct = new QAction("Edit", this);
    connect(editItemAct, SIGNAL(triggered()), this, SLOT(editRecords()));
    QAction* deleteItemAct = new QAction("Delete", this);
    connect(deleteItemAct, SIGNAL(triggered()), this, SLOT(deleteRecords()));
    QAction* createFolderAct = new QAction("New Folder", this);
    connect(createFolderAct, SIGNAL(triggered()), this, SLOT(createFolder()));
    toolbar->addAction(createItemAct);
    toolbar->addAction(editItemAct);
    toolbar->addAction(deleteItemAct);
    toolbar->addAction(createFolderAct);
    view = new QTreeView(this);

    QVBoxLayout* hbx = new QVBoxLayout(this);
    hbx->addWidget(toolbar);
    hbx->addWidget(view);

    db = new TestItemTableDatabase(this);
    db->createConnection();

    QStringList headers;
    headers<<"Наименование"
           <<"id"
           <<"Eдиница измерения"
           <<"Цена"
           <<"Каталог"
           <<"id Родительского каталога";

    this->setupModel(headers);
    this->createView();
}


void Test::setupModel(const QStringList& headers){
    model = new QStandardItemModel(this);
    model->setColumnCount(6);
    for(int i = 0; i<headers.count(); ++i){
       model->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
}

void Test::createView(){
    view->setModel(model);
    view->hideColumn(id);
    view->hideColumn(Measure);
    view->hideColumn(Folder);
    view->hideColumn(Parent);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->header()->setStretchLastSection(true);


    //connect(view, SIGNAL(ItemDoubleClicked()), this, SLOT(viewDoubleClicked()));
    connect(view, SIGNAL(expanded(const QModelIndex&)), this, SLOT(itemExpanded(const QModelIndex&)));
    connect(view, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(itemCollapsed(const QModelIndex&)));
    db->select(model->invisibleRootItem(), 0);
}

void Test::createItem(){
    QModelIndex index = view->currentIndex();
    QStringList data;
    data<<""<<""<<""<<"";
    Item* item = new Item(data, this);
    if(item->exec()){
        QVariantList list;
        list<<data[0]<<data[1]<<data[2]<<data[3];
        if(index.siblingAtColumn(Folder).data().toInt()){
            list<<index.siblingAtColumn(id).data();
        }else{
            list<<index.siblingAtColumn(Parent).data();
        }
        if(db->insertIntoTable(list)){
           updateTree();
        }
    }
}

void Test::editRecords(){

}

void Test::deleteRecords(){

}

void Test::createFolder(){
    QStringList data;
    data<<""<<""<<""<<"";
    Item* item = new Item(data, this);
    if(item->exec()){
        QVariantList list;
        list<<data[1]<<data[2]<<branch;
        if(db->insertFolderIntoTable(list)){

        }
    }
}

void Test::itemExpanded(const QModelIndex& item){
    itemId<<item;
    qDebug()<<item.internalPointer();
    qDebug()<<"s";
    qDebug()<<item;
}

void Test::itemCollapsed(const QModelIndex& item){
    for(int i =0; i<itemId.count(); ++i){
        if(item == itemId[i]){
            itemId.removeAt(i);
        }
    }
}

void Test::updateTree(){
    model->removeRows(0, model->rowCount());
    db->select(model->invisibleRootItem(), 0);
    int count = itemId.count();

    qDebug()<<count;
    qDebug()<<itemId[0];
    qDebug()<<itemId[1].parent().data();
    QModelIndex index = model->index(itemId[1].row(), itemId[0].column(), itemId[0]);
    qDebug()<<index;

    //view->expand(itemId[0]);
    //view->expand(index);



    //for(int i = 0; i < count; ++i){

    //}
    //itemId.clear();

    /*QList<QModelIndex> items;
        items<<itemId[i];

    for(int i = 0; i < count; ++i){
        if(!view->isExpanded(items[i])){
            qDebug()<<items[i];

            view->expand(items[i]);
        }
    }*/
    /*int count = itemId.count();
    qDebug()<<count;
    QList<QModelIndex> items;
    for(int i = 0; i < count; ++i){
        items<<itemId[i];
    }
    itemId.clear();
    for(int i = 0; i < count; ++i){
        if(items[i].parent() == model->invisibleRootItem()->index()){
            view->expand(items[i]);
        }else{
            view->expand(model->index(items[i].row(), items[i].parent().column(), items[i].parent()));
        }
    }*/
}

































