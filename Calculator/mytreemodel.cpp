#include "mytreemodel.h"
#include "mytreeitem.h"
#include "mydatabase.h"
#include "directorywidget.h"
#include <QIcon>

#include <QDebug>

extern MyDataBase db;

MyTreeModel::MyTreeModel(const QList<QVariant> &data, const QString& table, QObject* parent):QAbstractItemModel(parent), sqlTable(table)
{
    m_header = data;
    root = new MyTreeItem(m_header);

    QList<QList<QVariant>> list;

    db.select(sqlTable, list);
    setupModelData(list, root);

    if(table == TABLE_ELEMENTS)
        sortCol = 4;
    else if(table == TABLE_MATERIALS)
        sortCol = 3;
}

MyTreeModel::~MyTreeModel(){
    delete root;
}

QVariant MyTreeModel::data(const QModelIndex& index, int role)const{
    if(!index.isValid())
           return QVariant();

    if(index.column()==0){
        if(role != Qt::DecorationRole)
            return QVariant();
        MyTreeItem *item = static_cast<MyTreeItem*>(index.internalPointer());
        if(item->data(2).toLongLong()){
            if(item->isOpen)
                return QIcon("blue3.jpg").pixmap(QSize(25,25));
            else
                return QIcon("yellow2.png").pixmap(QSize(25,25));
        }else{
            return QIcon("blue1.png").pixmap(QSize(25,25));
        }
    }else if(index.column() == sortCol){
        if(role!=Qt::DisplayRole)
            return QVariant();
        MyTreeItem *item = static_cast<MyTreeItem*>(index.internalPointer());
           return item->data(index.column());
    }else{
        if(role != Qt::DisplayRole)
            return QVariant();
        MyTreeItem *item = static_cast<MyTreeItem*>(index.internalPointer());
        if(item->data(2).toLongLong())
            return QVariant();
        else
            return item->data(index.column());
    }
}

Qt::ItemFlags MyTreeModel::flags(const QModelIndex &index)const{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex MyTreeModel::index(int row, int column, const QModelIndex &parent)const{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    MyTreeItem* item;
    if(!parent.isValid())
        item = rootItem;
    else
        item = static_cast<MyTreeItem*>(parent.internalPointer());
    MyTreeItem* child = item->child(row);
    if(child)
        return createIndex(row, column, child);
    else
        return QModelIndex();
}
QVariant MyTreeModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_header.value(section);
    return QVariant();
}

QModelIndex MyTreeModel::parent(const QModelIndex &index)const{
    if(!index.isValid())
        return QModelIndex();
    MyTreeItem *childItem = static_cast<MyTreeItem*>(index.internalPointer());
    MyTreeItem *parentItem = childItem->parent();
    if(parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->row(),0, parentItem);
}
int MyTreeModel::rowCount(const QModelIndex &parent)const{
    MyTreeItem* parentItem;
    if(parent.column()>0)
        return 0;
    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<MyTreeItem*>(parent.internalPointer());
    return parentItem->childCount();
}

int MyTreeModel::columnCount(const QModelIndex &parent)const{
    Q_UNUSED(parent);
    return m_header.count();
}
//выгрузка элементов базы данных
void MyTreeModel::setupModelData(const QList<QList<QVariant>> &lines, MyTreeItem* parent){

    QList<MyTreeItem*> folders;
    folders<<parent;
    for(int i = 0; i<lines.count(); ++i){
        QList<QVariant> tmp = lines.value(i);
        int fNum = tmp.value(1).toInt();
        MyTreeItem* item = new MyTreeItem(tmp,folders[fNum]);
        folders[fNum]->appendChild(item);
        if(tmp.value(2).toInt()){
            folders<<item;
            //item->folder = true;
            /*MyTreeItem* pItem = new MyTreeItem(tmp, item->parent());
            item->appendChild(pItem);
            pItem->isOpen = true;
            pItem->folder = true;
            */
        }
    }
    for(int i = 0; i<folders.count();++i){
        folders[i]->sortItem();
    }
    rootItem = parent;
}

void MyTreeModel::rootItemChanged(QModelIndex index){
    MyTreeItem* item = static_cast<MyTreeItem*>(index.internalPointer());
    beginResetModel();
    if(item->folder){
        if(!item->isOpen){
            rootItem = item;
        }
        else{
            rootItem = item->parent();
        }
    }else{
        QList<QVariant> data;
        data = item->rowData();
        emit sendData(data);
        //if(sqlTable == TABLE_MATERIALS)
        //    static_cast<DirectoryWidget*>(QAbstractItemModel::parent())->accept();
    }
    endResetModel();
}

MyTreeItem* MyTreeModel::currentRoot() const{
    return rootItem;
}

bool MyTreeModel::createItem(MyTreeItem *itemP, QList<QVariant> &data){
    beginResetModel();
    if(!db.insertIntoTable(sqlTable, data))
        return false;
    qlonglong num = db.getLastNumNumber(sqlTable);
    if(num==-1)
        return false;
    data[0] = num;
    MyTreeItem* newItem = new MyTreeItem(data, itemP);
    itemP->appendChild(newItem);
    itemP->sortItem();
    endResetModel();
    return true;
}

bool MyTreeModel::updateItem(MyTreeItem* itemT, const QList<QVariant> &data){
    beginResetModel();
    if(!db.updateTableItem(sqlTable, data))
        return false;
    itemT->setRowData(data);
    itemT->parent()->sortItem();
    endResetModel();
    return true;
}

bool MyTreeModel::createFolder(MyTreeItem *itemP, QList<QVariant>& data){
    beginResetModel();
    qlonglong folderNum = db.getLastFolderNumber(sqlTable) + 1;
    if(!folderNum)
        return false;
    data[2] = folderNum;
    if(!db.insertIntoTable(sqlTable, data))
        return false;
    qlonglong num = db.getLastNumNumber(sqlTable);
    if(num==-1)
        return false;
    data[0] = num;
    MyTreeItem* newItem = new MyTreeItem(data, itemP);
    itemP->appendChild(newItem);
    /*newItem->folder = true;
    MyTreeItem* pItem = new MyTreeItem(data, itemP);
    newItem->appendChild(pItem);
    pItem->isOpen = true;
    pItem->folder = true;*/
    itemP->sortItem();
    endResetModel();
    return true;
}
