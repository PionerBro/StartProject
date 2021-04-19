#include "mytreeitem.h"
#include <QDebug>

static bool compare(const MyTreeItem* first, const MyTreeItem* second){
    if(first->data(1).toString()<second->data(1).toString())
        return true;
    else if(first->data(1).toString()>second->data(1).toString())
        return false;
    else
        return true;
}

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

void MyTreeItem::sortItem(){
    if(!childItems.isEmpty()){
        MyTreeItem* tmp = childItems.value(0);
        childItems.removeFirst();
        std::sort(childItems.begin(), childItems.end(), compare);
        childItems.push_front(tmp);
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
