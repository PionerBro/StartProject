#include "calccalculationsmodelitem.h"

CalcCalculationsModelItem::CalcCalculationsModelItem(const QVector<QVariant>& data, CalcCalculationsModelItem* parent )
{
    m_parent = parent;
    m_itemData = data;
    m_sortCol = (m_parent) ? m_parent->m_sortCol : 0;
}

CalcCalculationsModelItem::~CalcCalculationsModelItem(){
    qDeleteAll(m_childItems);
}

QVariant CalcCalculationsModelItem::data(int column) const{
    return m_itemData.value(column);
}

CalcCalculationsModelItem* CalcCalculationsModelItem::child(int row) const{
    return m_childItems.value(row);
}

CalcCalculationsModelItem* CalcCalculationsModelItem::parent() const{
    return m_parent;
}

int CalcCalculationsModelItem::row() const{
    if(m_parent)
        return m_parent->m_childItems.indexOf(const_cast<CalcCalculationsModelItem*>(this));
    return 0;
}
int CalcCalculationsModelItem::columnCount()const{
    return m_itemData.count();
}
int CalcCalculationsModelItem::childCount()const{
    return m_childItems.count();
}

void CalcCalculationsModelItem::appendChild(CalcCalculationsModelItem* child){
    m_childItems.append(child);
}

void CalcCalculationsModelItem::setRowData(const QVector<QVariant> &data){
    m_itemData.clear();
    m_itemData<<data;
}

QVector<QVariant> CalcCalculationsModelItem::rowData() const{
    return m_itemData;
}

void CalcCalculationsModelItem::sortItem(){
    if(!m_childItems.isEmpty()){
            std::sort(m_childItems.begin(), m_childItems.end(), compare);
    }
}

int CalcCalculationsModelItem::getSortCol()const{
    return m_sortCol;
}

bool compare(const CalcCalculationsModelItem* first, const CalcCalculationsModelItem* second){
    int sortCol = first->parent()->parent()->getSortCol();
    QString firstS = first->data(sortCol).toString().toUpper();
    QString secondS = second->data(sortCol).toString().toUpper();
    if(firstS < secondS)
        return true;
    else if(firstS > secondS)
        return false;
    else
        return true;
}
