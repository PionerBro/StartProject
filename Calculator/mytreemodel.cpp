#include "mytreemodel.h"
#include "mytreeitem.h"
#include "mydatabase.h"
#include "directorywidget.h"
#include <QIcon>
#include <QDate>

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
                return QIcon("../Calculator/blue3.jpg").pixmap(QSize(25,25));
            else
                return QIcon("../Calculator/yellow2.png").pixmap(QSize(25,25));
        }else{
            return QIcon("../Calculator/blue1.png").pixmap(QSize(25,25));
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
        if(role!=Qt::BackgroundRole || reserveNum.isEmpty())
            return QVariant();
        else if(role == Qt::BackgroundRole && reserveCh.value(index.row())){
            return QBrush(QColor(Qt::red));
        }else if(role == Qt::BackgroundRole && reserveAc.value(index.row())){
            return QBrush(QColor(Qt::green));
        }else
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

//вызывается в виджете которому принадлежит модель, при нажатии кнопки на тулбаре
void MyTreeModel::setEditableCol(bool b){
    beginResetModel();
    colIsEditable = b;                                              //устанавливаем флаг для метода data()
    if(b){
        reserveCh.fill(false,tableItems.count());                       //заполняем резверную копию данными
        for(int i = 0; i<tableItems.count(); ++i){
            reserveData<<tableItems.value(i)->data(5);
        }
    }else{
        reserveData.clear();
        reserveNum.clear();
        reserveCh.clear();
        reserveAc.clear();
    }
    endResetModel();
}

//вызывется в делегате при изменении данных в ячейке, передает строку в которой изменились данные и сам текст данных
void MyTreeModel::reserveDataChanged(int row, const QString& text){
    if(!reserveAc.isEmpty()){
        reserveAc.clear();
        reserveNum.clear();
    }
    reserveData[row] = text;                                        //изменяем текст резервной копии
    reserveCh[row] = true;                                          //устанавливем флаг изменения в списке флагов элементов(нужен в методе data(const QModelIndex& index, int role))
    for(int i = 0; i < reserveNum.count(); ++i){
        if(reserveNum.value(i) == row){
            return;
        }
    }
    reserveNum<<row;                                                //добавляем в список номеров элементов MyTreeItem новый элемент если его в этом списке нет
    emit reserveDataChange(true);
}


//Принятие обновления данных: данные резервной копии которые изменились становяться основными
void MyTreeModel::viewAcceptTriggered(){

    beginResetModel();
    reserveAc.fill(false, tableItems.count());
    qlonglong docNum = db.getLastNumNumber(TABLE_MATERIALS_PRICES)+1;
    for(int i = 0; i < reserveNum.count(); ++i){                                //reserveNum список номеров MytreeItem в tableitems которые были изменены
        int num = reserveNum.value(i);
        MyTreeItem* item = tableItems.value(num);
        if(item->data(5).toDouble() != reserveData.value(num).toDouble()){      //получаем список данный элемента в базе, изменяем цену и обновляем
            QList<QVariant> data = item->rowData();
            data[5] = reserveData.value(num);
            QList<QVariant> dataPrices;
            dataPrices<<docNum<<data.value(0)<<QDate().currentDate().toString("dd.MM.yyyy")<<data.value(5);
            if(updateItem(item, data) && db.insertIntoTable(TABLE_MATERIALS_PRICES, dataPrices)){
                reserveCh[num] = false;
                reserveAc[num] = true;
            }
        }else{
            reserveCh[num] = false;
            reserveAc[num] = true;
        }
    }
    emit acceptIsComplete();                                                    //сигнал для изменения доступности кнопок в тулбаре
    endResetModel();
}

void MyTreeModel::viewRejectTriggered(){
    beginResetModel();
    reserveData.clear();
    reserveCh.clear();
    reserveNum.clear();
    for(int i = 0; i<tableItems.count(); ++i){
        reserveData<<tableItems.value(i)->data(5);
    }
    reserveCh.fill(false,tableItems.count());
    emit rejectIsComplete();
    endResetModel();
}
