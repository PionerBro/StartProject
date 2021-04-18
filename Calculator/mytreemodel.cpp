#include "mytreemodel.h"
#include "mytreeitem.h"

MyTreeModel::MyTreeModel(const QStringList &data, QObject* parent):QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData<<"Title"<<"Summary";
    root = new MyTreeItem(rootData);
    for(int i = 0; i <5; ++i){
        rootData.clear();
        rootData<<"item " + QString::number(i)<<i;
        MyTreeItem* item = new MyTreeItem(rootData,root);
        item->folder = true;
        MyTreeItem* itemBack = new MyTreeItem(rootData,root);
        item->appendChild(itemBack);
        itemBack->folder = true;
        itemBack->isOpen = true;
        root->appendChild(item);
        item->appendChild(new MyTreeItem(rootData,item));
    }
    for(int i = 6; i <11; ++i){
        rootData.clear();
        rootData<<"item " + QString::number(i);
        MyTreeItem* item = new MyTreeItem(rootData,root);
        root->appendChild(item);
    }
    setupModelData(data, root);
}

MyTreeModel::~MyTreeModel(){
    delete root;
}

QVariant MyTreeModel::data(const QModelIndex& index, int role)const{
    if(!index.isValid())
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();
    MyTreeItem *item = static_cast<MyTreeItem*>(index.internalPointer());
    return item->data(index.column());
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
        return rootItem->data(section);
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
    if(parent.isValid())
        return static_cast<MyTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

void MyTreeModel::setupModelData(const QStringList &lines, MyTreeItem* parent){
    /*QList<QVariant> list;
    list << "s"<<"11";
    MyTreeItem* item = new MyTreeItem(list, parent);
    rootItem->appendChild(item);
    list.clear();
    list<<"af";
    item = new MyTreeItem(list, parent);
    rootItem->appendChild(item);
    list.clear();
    list<<"as"<<"ss";
    MyTreeItem* item2 = new MyTreeItem(list, item);
    item->appendChild(item2);
    */
    rootItem = parent;
}

void MyTreeModel::rootItemChanged(const QModelIndex& index){
    MyTreeItem* item = static_cast<MyTreeItem*>(index.internalPointer());
    beginResetModel();
    if(item->folder){
        if(!item->isOpen){
            rootItem = item;
        }
        else{
            rootItem = item->parent();
        }
    }
    endResetModel();
}
