#include "itemtable.h"
#include "item.h"
#include "itemtabledatabase.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMessageBox>
#include <QDebug>

ItemTable::ItemTable(QStringList& data, QWidget* parent) : QDialog(parent), m_data(data)
{
    this->setMinimumSize(93,122);

    m_db = new ItemTableDatabase(this);
    m_db->createConnection();

    QToolBar* toolBar = new QToolBar(this);
    QAction* createItemAct = new QAction(tr("New"), this);
    QAction* editItemAct = new QAction(tr("Edit"), this);
    QAction* deleteItemAct = new QAction(tr("Delete"), this);
    connect(createItemAct, SIGNAL(triggered()), this, SLOT(createItem()));
    connect(editItemAct, SIGNAL(triggered()), this, SLOT(editItem()));
    connect(deleteItemAct, SIGNAL(triggered()), this, SLOT(deleteItem()));
    toolBar->addAction(createItemAct);
    toolBar->addAction(editItemAct);
    toolBar->addAction(deleteItemAct);
    view = new QTableView(this);    
    QVBoxLayout* vbx = new QVBoxLayout(this);
    vbx->addWidget(toolBar);
    vbx->addWidget(view);

    QStringList headers;
    headers<<tr("id")
           <<tr("Артикул")
           <<tr("Наименование")
           <<tr("Ед. Изм.")
           <<tr("Цена");
    this->setupModel(TABLE, headers);
    this->createView();

}

void ItemTable::setupModel(const QString& table, const QStringList& headers){
    model = new QSqlTableModel(this);
    model->setTable(table);
    for(int i = 0; i<model->columnCount(); ++i){
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
    model->setSort(0, Qt::AscendingOrder);
}

void ItemTable::createView(){
    view->setModel(model);
    view->hideColumn(0);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    connect(view, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(selectItem(const QModelIndex&)));
    model->select();
}

void ItemTable::createItem(){
    QStringList data;
    data<<""<<""<<""<<"";
    Item* item = new Item(data, this);
    if(item->exec()){               
        QVariantList list;
        list<<data[0]<<data[1]<<data[2]<<data[3];
        if(m_db->insertIntoTable(list)){
            model->select();
            qDebug()<<"true";
        }
    }


}

void ItemTable::editItem(){
    int row = view->currentIndex().row();
    if(row == -1){
        return;
    }
    QStringList data;
    for(int i=1; i<model->columnCount(); ++i){
        data<<model->data(model->index(row, i)).toString();
    }
    Item* item = new Item(data, this);
    if(item->exec()){
        QVariantList list;
        for(int i = 0; i<data.count(); ++i){
            list<<data[i];
        }
        list<<model->data(model->index(row, 0)).toInt();
        m_db->updateRecord(list);
        model->select();
    }
}

void ItemTable::deleteItem(){
    int row = view->currentIndex().row();
    if(row == -1){
        return;
    }
    model->removeRow(row);
    model->select();
}

void ItemTable::selectItem(const QModelIndex& index){
    for(int i = 1; i<model->columnCount(); ++i){
        m_data<<model->data(model->index(index.row(),i)).toString();
    }
    QDialog::accept();
}











