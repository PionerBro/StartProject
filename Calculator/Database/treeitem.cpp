#include "treeitem.h"

bool compare(const TreeItem* first, const TreeItem* second){
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

TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
{

    parentItem = parent;
    sortCol = (parentItem) ? parentItem->sortCol : 0;
    if(data.isEmpty()){
        itemType = ItemType::FolderOpen;
    }else{
        itemData = data;
        if(data.value(FieldName::FolderId).toLongLong()){
            itemType = ItemType::FolderClose;
            TreeItem* item = new TreeItem({}, parent);
            item->setRowData(data);
            appendChild(item);
        }else{
            itemType = ItemType::Element;
        }
    }
}

TreeItem::~TreeItem(){
    qDeleteAll(childItems);
}

QVariant TreeItem::data(int column) const{
    return itemData.value(column);
}

QVector<QVariant> TreeItem::rowData() const{
    return itemData;
}

TreeItem* TreeItem::child(int cRow){
    return childItems.value(cRow);
}

TreeItem* TreeItem::parent() const{
    return parentItem;
}

int TreeItem::row() const{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}

int TreeItem::columnCount() const{
    return itemData.count();
}

int TreeItem::childCount() const{
    return childItems.count();
}

void TreeItem::setRowData(const QVector<QVariant> &data){
    itemData.clear();
    itemData<<data;
}

void TreeItem::setData(int column, const QVariant &data){
    if(column >= 0 && column < columnCount()){
        itemData[column] = data;
    }
}

void TreeItem::appendChild(TreeItem *child){
    childItems.append(child);
}

void TreeItem::setSortCol(int col){
    sortCol = col;
}

int TreeItem::getSortCol()const{
   return sortCol;
}

void TreeItem::sortItem(){
    if(parentItem){
        if((childItems.count() > 1)){
            TreeItem* tmp = childItems.value(0);
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

int TreeItem::getItemType() const{
    return itemType;
}

