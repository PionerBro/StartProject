#include "mytreeitem.h"
#include <QDebug>

static bool compare(const MyTreeItem* first, const MyTreeItem* second){
    QString firstS = first->data(first->sortCol).toString().toUpper();
    QString secondS = second->data(second->sortCol).toString().toUpper();
    if(firstS < secondS)
        return true;
    else if(firstS > secondS)
        return false;
    else
        return true;
}

MyTreeItem::MyTreeItem(const QList<QVariant> &data, MyTreeItem* parent)
{   
    parentItem = parent;
    sortCol = (data.count() == 6) ? 3 : 4;
    if(data.isEmpty()){
        folder=true;
        isOpen=true;
    }else{
        itemData = data;
        isOpen = false;
        if(data.value(2).toLongLong()){
            folder = true;
            MyTreeItem* item = new MyTreeItem({}, parent);
            item->setRowData(data);
            appendChild(item);
        }else{
            folder = false;
        }
    }
}

MyTreeItem::~MyTreeItem(){
    qDebug()<<"Delete TreeItem"<<itemData.value(0);
    qDeleteAll(childItems);
}


void MyTreeItem::sortItem(){
    if(parentItem){
        if((childItems.count() > 1)){
            MyTreeItem* tmp = childItems.value(0);
            childItems.removeFirst();
            std::sort(childItems.begin(), childItems.end(), compare);
            childItems.push_front(tmp);
        }
    }else{
        if(!childItems.isEmpty()){
            std::sort(childItems.begin(), childItems.end(), compare);
        }
    }
}

QList<QVariant> MyTreeItem::rowData() const{
    return itemData;
}

QVariant MyTreeItem::data(int column) const{
    return itemData.value(column);
}

MyTreeItem* MyTreeItem::child(int row){
    return childItems.value(row);
}

MyTreeItem* MyTreeItem::parent(){
    return parentItem;
}

int MyTreeItem::row() const{
    if(parentItem)
        return parentItem->childItems.indexOf(const_cast<MyTreeItem*>(this));
    return 0;
}
int MyTreeItem::columnCount()const{
    return itemData.count();
}
int MyTreeItem::childCount()const{
    return childItems.count();
}

void MyTreeItem::appendChild(MyTreeItem* child){
    childItems.append(child);
}

void MyTreeItem::setRowData(const QList<QVariant> &data){
    itemData.clear();
    itemData<<data;
}
