#include "calculatorhistoryitem.h"

bool compare(const CalculatorHistoryItem* first, const CalculatorHistoryItem* second){
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

CalculatorHistoryItem::CalculatorHistoryItem(const QVector<QVariant>& data, CalculatorHistoryItem* parent)
{
    parentItem = parent;
    sortCol = (parentItem) ? parentItem->sortCol : 0;
    itemData = data;
}

CalculatorHistoryItem::~CalculatorHistoryItem(){
    qDeleteAll(childItems);
}

QVariant CalculatorHistoryItem::data(int column) const{
    return itemData.value(column);
}

QVector<QVariant> CalculatorHistoryItem::rowData() const{
    return itemData;
}

CalculatorHistoryItem* CalculatorHistoryItem::child(int cRow){
    return childItems.value(cRow);
}

CalculatorHistoryItem* CalculatorHistoryItem::parent() const{
    return parentItem;
}

int CalculatorHistoryItem::row() const{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CalculatorHistoryItem*>(this));
    return 0;
}

int CalculatorHistoryItem::columnCount() const{
    return itemData.count();
}

int CalculatorHistoryItem::childCount() const{
    return childItems.count();
}

void CalculatorHistoryItem::setRowData(const QVector<QVariant> &data){
    itemData.clear();
    itemData<<data;
}

void CalculatorHistoryItem::setData(int column, const QVariant &data){
    if(column >= 0 && column < columnCount()){
        itemData[column] = data;
    }
}

void CalculatorHistoryItem::appendChild(CalculatorHistoryItem *child){
    childItems.append(child);
}

void CalculatorHistoryItem::setSortCol(int col){
    sortCol = col;
}

int CalculatorHistoryItem::getSortCol()const{
   return sortCol;
}

void CalculatorHistoryItem::sortItem(){
    if(!childItems.isEmpty()){
            std::sort(childItems.begin(), childItems.end(), compare);
    }
}
