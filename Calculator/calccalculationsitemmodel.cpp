#include "calccalculationsitemmodel.h"
#include "calccalculationsmodelitem.h"
#include "calculatordatabase.h"

#include <QDebug>

extern CalculatorDatabase g_db;

CalcCalculationsItemModel::CalcCalculationsItemModel(const QVector<QString>& tables, const QVector<QVariant>& headers, QObject* parent) : QAbstractItemModel(parent)
{
    m_db = &g_db;
    m_sqlMainTable = tables.value(0);
    m_sqlTablePrices = tables.value(1);
    m_hHeaders = headers;
    m_editableCols.fill(false, m_hHeaders.count());
    setupModel();
}

CalcCalculationsItemModel::~CalcCalculationsItemModel(){
    qDeleteAll(m_rootItems);
}

QVariant CalcCalculationsItemModel::data(const QModelIndex& index, int role)const{
    if(!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        return static_cast<CalcCalculationsModelItem*>(index.internalPointer())->data(index.column());
    default:
        return QVariant();
    }
}

Qt::ItemFlags CalcCalculationsItemModel::flags(const QModelIndex &index)const{
    if(!index.isValid())
        return Qt::NoItemFlags;
    if(m_editableCols.value(index.column()))
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex CalcCalculationsItemModel::index(int row, int column, const QModelIndex &parent)const{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    CalcCalculationsModelItem* item;
    if(!parent.isValid())
        item = m_root;
    else
        item = static_cast<CalcCalculationsModelItem*>(parent.internalPointer());
    CalcCalculationsModelItem* child = item->child(row);
    if(child)
        return createIndex(row, column, child);
    else
        return QModelIndex();
}


QVariant CalcCalculationsItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_hHeaders.value(section);
    return QVariant();
}

QModelIndex CalcCalculationsItemModel::parent(const QModelIndex &index)const{
    if(!index.isValid())
        return QModelIndex();

    CalcCalculationsModelItem *childItem = static_cast<CalcCalculationsModelItem*>(index.internalPointer());
    CalcCalculationsModelItem *parentItem = childItem->parent();
    if(parentItem == m_root)
       return QModelIndex();
    return createIndex(parentItem->row(),0, parentItem);
}

int CalcCalculationsItemModel::rowCount(const QModelIndex &parent)const{
    CalcCalculationsModelItem* parentItem;
    if(parent.column()>0)
        return 0;
    if(!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<CalcCalculationsModelItem*>(parent.internalPointer());
    return parentItem->childCount();
}

int CalcCalculationsItemModel::columnCount(const QModelIndex &parent)const{
    Q_UNUSED(parent);
    return m_hHeaders.count();
}

QVector<QVariant> CalcCalculationsItemModel::getRowData(const QModelIndex& index) const{
    return static_cast<CalcCalculationsModelItem*>(index.internalPointer())->rowData();
}

void CalcCalculationsItemModel::setColEditable(int column, bool value){
    beginResetModel();
    if(m_editableCols.count() > column)
        m_editableCols[column] = value;
    endResetModel();
}

bool CalcCalculationsItemModel::setRoot(int num){
    if(!m_rootItems.contains(num))
        return false;
    m_root->setRowData({num});
    if(m_root->childCount()){
        for(int i = 0; i < m_root->childCount(); ++i){
            delete m_root->child(i);
        }
    }
    CalcCalculationsModelItem* tmp = m_rootItems.value(num);
    for(int i = 0; i < tmp->childCount(); ++i){
        CalcCalculationsModelItem* item = new CalcCalculationsModelItem(tmp->child(i)->rowData(), m_root);
        m_root->appendChild(item);
    }
    return true;
}

void CalcCalculationsItemModel::setupModel(){
    if(!m_rootItems.isEmpty()){
        qDeleteAll(m_rootItems);
        m_rootItems.clear();
    }
    CalcCalculationsModelItem* item = new CalcCalculationsModelItem({0});
    m_rootItems.insert(0, item);
    m_root = m_rootItems.value(0);
    QVector<QVector<QVariant>> data;
    if(selectItems(data)){
        for(int i = 0; i < data.count(); ++i){
            qlonglong itemNum = data.value(i).value(0).toLongLong();
            if(!m_rootItems.contains(itemNum)){
                item = new CalcCalculationsModelItem({itemNum});
                m_rootItems.insert(itemNum, item);
            }
            item = new CalcCalculationsModelItem(data.value(i), m_rootItems.value(itemNum));
            m_rootItems.value(itemNum)->appendChild(item);
        }
    }
    //QList<int> keys = m_rootItems.keys();
    //for(int i = 0; i < keys.count(); ++i){
      //  m_rootItems.value(keys.value(i))->sortItem();
    //}
    qDebug()<<"s";
}

bool CalcCalculationsItemModel::selectItems(QVector<QVector<QVariant>>& data){
    QVector<QVector<QVariant>> fdata;
    if(m_db->select(m_sqlMainTable, fdata)){
        for(int i = 0; i < fdata.count(); ++i){
            QVector<QVariant> vect;
            vect<<fdata.value(i).value(1)<<fdata.value(i).value(3)<<fdata.value(i).value(4)<<fdata.value(i).value(5)<<fdata.value(i).value(6)<<QString::number(fdata.value(i).value(5).toDouble()*fdata.value(i).value(6).toDouble(), 'f', 2);
            data<<vect;
        }
        qDebug()<<data;
        return true;
    }
    return false;
}

void CalcCalculationsItemModel::addNewRow(){
    beginResetModel();
    QVector<QVariant> data;
    data.fill(QVariant(), columnCount());
    CalcCalculationsModelItem* item = new CalcCalculationsModelItem(data, m_root);
    m_root->appendChild(item);
    endResetModel();
}

CalcCalculationsModelItem* CalcCalculationsItemModel::rootItem(){
    return m_root;
}

bool CalcCalculationsItemModel::createModelItem(){
    qlonglong iNum = m_root->data(0).toLongLong();
    if(iNum){
        //if (m_db->deleteFromTable(iNum, m_sqlMainTable))
           // return false;
        CalcCalculationsModelItem* item = m_rootItems.value(iNum);
        for(int i = 0; i < item->childCount(); ++i){
            //m_db->getIdFromName();

            qDebug()<<item->child(i)->rowData();
            delete (item->child(i));
        }
        for(int i = 0; i < m_root->childCount(); ++i){
            CalcCalculationsModelItem* tmp = new CalcCalculationsModelItem(m_root->child(i)->rowData(), item);
            item->appendChild(tmp);
        }
    }
    return true;
}
