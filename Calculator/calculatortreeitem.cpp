#include "calculatortreeitem.h"

bool compare(const CalculatorTreeItem* first, const CalculatorTreeItem* second){
    int sortCol = first->parent()->getSortCol();
    QString firstS = first->data(sortCol).toString().toUpper();
    QString secondS = second->data(sortCol).toString().toUpper();
    if(firstS < secondS)
        return true;
    else if(firstS > secondS)
        return false;
    else
        return true;
}

CalculatorTreeItem::CalculatorTreeItem(const QVector<QVariant>& data, CalculatorTreeItem* parent)
{

    parentItem = parent;
    sortCol = (parentItem) ? parentItem->sortCol : 0;
    if(data.isEmpty()){
        itemType = ItemType::FolderOpen;
    }else{
        itemData = data;
        if(data.value(FieldName::FolderId).toLongLong()){
            itemType = ItemType::FolderClose;
            CalculatorTreeItem* item = new CalculatorTreeItem({}, parent);
            item->setRowData(data);
            appendChild(item);
        }else{
            itemType = ItemType::Element;
        }
    }
}

CalculatorTreeItem::~CalculatorTreeItem(){
    qDeleteAll(childItems);
}

QVariant CalculatorTreeItem::data(int column) const{
    return itemData.value(column);
}

QVector<QVariant> CalculatorTreeItem::rowData() const{
    return itemData;
}

CalculatorTreeItem* CalculatorTreeItem::child(int cRow){
    return childItems.value(cRow);
}

CalculatorTreeItem* CalculatorTreeItem::parent() const{
    return parentItem;
}

int CalculatorTreeItem::row() const{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CalculatorTreeItem*>(this));
    return 0;
}

int CalculatorTreeItem::columnCount() const{
    return itemData.count();
}

int CalculatorTreeItem::childCount() const{
    return childItems.count();
}

void CalculatorTreeItem::setRowData(const QVector<QVariant> &data){
    itemData.clear();
    itemData<<data;
}

void CalculatorTreeItem::setData(int column, const QVariant &data){
    if(column >= 0 && column < columnCount()){
        itemData[column] = data;
    }
}

void CalculatorTreeItem::appendChild(CalculatorTreeItem *child){
    childItems.append(child);
}

void CalculatorTreeItem::setSortCol(int col){
    sortCol = col;
}

int CalculatorTreeItem::getSortCol()const{
   return sortCol;
}

void CalculatorTreeItem::sortItem(){
    if(parentItem){
        if((childItems.count() > 1)){
            CalculatorTreeItem* tmp = childItems.value(0);
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

int CalculatorTreeItem::getItemType() const{
    return itemType;
}
