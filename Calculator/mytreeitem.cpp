#include "mytreeitem.h"
#include <QDebug>

MyTreeItem::MyTreeItem(const QList<QVariant> &data, MyTreeItem* parent)
{
    itemData = data;
    parentItem = parent;
    folder = false;
    isOpen = false;
}

MyTreeItem::~MyTreeItem(){
    qDebug()<<itemData.value(0);
    qDeleteAll(childItems);
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
