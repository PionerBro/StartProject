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

    if(table == TABLE_ELEMENTS)
        sortCol = 4;
    else if(table == TABLE_MATERIALS)
        sortCol = 3;

    db.select(sqlTable, list);
    setupModelData(list, root);
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
    }else if(index.column() == 5 && sqlTable == TABLE_MATERIALS && colIsEditable){
        if(role == Qt::EditRole || role == Qt::DisplayRole)
            return reserveData.value(index.row());
        return QVariant();
    }else if(index.column() == 6 && sqlTable==TABLE_MATERIALS){
        if(role!=Qt::DisplayRole)
            return QVariant();
        if(reserveCh.value(index.row()))
            return "Edit";
        else
            return QVariant();
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
    if(sqlTable == TABLE_MATERIALS && index.column() == 5 && colIsEditable)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex MyTreeModel::index(int row, int column, const QModelIndex &parent)const{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    if(treeModelType){
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
    }else{
        MyTreeItem* child = tableItems.value(row);
        if(child)
            return createIndex(row, column, child);
        else
            return QModelIndex();
    }
}
QVariant MyTreeModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_header.value(section);
    return QVariant();
}

QModelIndex MyTreeModel::parent(const QModelIndex &index)const{
    if(!index.isValid())
        return QModelIndex();
    if(treeModelType){
        MyTreeItem *childItem = static_cast<MyTreeItem*>(index.internalPointer());
        MyTreeItem *parentItem = childItem->parent();
        if(parentItem == rootItem)
           return QModelIndex();
        return createIndex(parentItem->row(),0, parentItem);
    }else{
        return QModelIndex();
    }
}
int MyTreeModel::rowCount(const QModelIndex &parent)const{
    if(treeModelType){
        MyTreeItem* parentItem;
        if(parent.column()>0)
            return 0;
        if(!parent.isValid())
            parentItem = rootItem;
        else
            parentItem = static_cast<MyTreeItem*>(parent.internalPointer());
        return parentItem->childCount();
    }else{
        return tableItems.count();
    }
}

int MyTreeModel::columnCount(const QModelIndex &parent)const{
    Q_UNUSED(parent);
    return m_header.count();
}

static bool compare(const MyTreeItem* first,const MyTreeItem* second){
    QString firstS = first->data(first->sortCol).toString().toUpper();
    QString secondS = second->data(second->sortCol).toString().toUpper();
    if(firstS < secondS)
        return true;
    else if(firstS > secondS)
        return false;
    else
        return true;
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
        }else{
        tableItems<<item;
        }
    }
    for(int i = 0; i<folders.count();++i){
        folders[i]->sortItem();
    }
    rootItem = parent;
    //*********************************************
    std::sort(tableItems.begin(), tableItems.end(), compare);
    qDebug()<<"**********************";
    for(int i = 0; i<tableItems.count(); ++i){
        qDebug()<<tableItems.value(i)->data(sortCol).toString();
    }
}

void MyTreeModel::rootItemChanged(QModelIndex index){

    MyTreeItem* item = static_cast<MyTreeItem*>(index.internalPointer());
    beginResetModel();
    if(item->folder){
        if(treeModelType){
            if(!item->isOpen){
                rootItem = item;
            }
            else{
                rootItem = item->parent();
            }
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
    QString newItemSortData = newItem->data(sortCol).toString().toUpper();
    bool insFlag = false;
    for(int i = 0; i<tableItems.count(); ++i)
    {
        if(newItemSortData < tableItems.value(i)->data(sortCol).toString().toUpper()){
            tableItems.insert(i, newItem);
            insFlag = true;
            break;
        }
    }
    if(!insFlag)
        tableItems.push_back(newItem);
    endResetModel();
    qDebug()<<"**********************";
    for(int i = 0; i<tableItems.count(); ++i){
        qDebug()<<tableItems.value(i)->data(sortCol).toString();
    }
    return true;
}

bool MyTreeModel::updateItem(MyTreeItem* itemT, const QList<QVariant> &data){
    beginResetModel();
    if(!db.updateTableItem(sqlTable, data))
        return false;
    itemT->setRowData(data);
    itemT->parent()->sortItem();
    qlonglong itemTNum = itemT->data(0).toLongLong();
    QString itemTSortData = itemT->data(sortCol).toString().toUpper();
    bool insFlag = false;
    bool delFlag = false;
    int del = -1;
    int ins;
    for(int i = 0; i < tableItems.count(); ++i){
        if(!delFlag && tableItems.value(i)->data(0).toLongLong()==itemTNum){
            del = i;
            delFlag = true;
            if(insFlag)
                break;
        }
        if(!insFlag && itemTSortData < tableItems.value(i)->data(sortCol).toString().toUpper()){
            insFlag = true;
            ins = i;
            if(delFlag)
                break;
        }
    }
    if(!insFlag)
        ins = tableItems.count();
    tableItems.insert(ins, itemT);
    if(del >= ins)
        ++del;
    tableItems.removeAt(del);
    endResetModel();
    qDebug()<<"**********************";
    for(int i = 0; i<tableItems.count(); ++i){
        qDebug()<<tableItems.value(i)->data(sortCol).toString();
    }
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
    QString newItemSortData = newItem->data(sortCol).toString().toUpper();
    bool insFlag = false;
    for(int i = 0; i<tableItems.count(); ++i)
    {
        if(newItemSortData < tableItems.value(i)->data(sortCol).toString().toUpper()){
            tableItems.insert(i, newItem);
            insFlag = true;
            break;
        }
    }
    if(!insFlag)
        tableItems.push_back(newItem);
    endResetModel();
    qDebug()<<"**********************";
    for(int i = 0; i<tableItems.count(); ++i){
        qDebug()<<tableItems.value(i)->data(sortCol).toString();
    }
    return true;
}

void MyTreeModel::setTreeModelType(bool b){
    beginResetModel();
    treeModelType = b;
    endResetModel();
}

void MyTreeModel::setEditableCol(bool b){
    beginResetModel();
    colIsEditable = b;
    reserveCh.fill(false,tableItems.count());
    if(b){
        for(int i = 0; i<tableItems.count(); ++i){
            reserveData<<tableItems.value(i)->data(5);
        }
    }else{
        reserveData.clear();
    }
    endResetModel();
}

void MyTreeModel::reserveDataChanged(int row, const QString& text){
    reserveData[row] = text;
    reserveCh[row] = true;
    emit reserveDataChange(true);
}
