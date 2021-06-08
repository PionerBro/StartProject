#include "calccalculationsitemmodel.h"
#include "calccalculationsmodelitem.h"
#include "calculatordatabase.h"

extern CalculatorDatabase g_db;

CalcCalculationsItemModel::CalcCalculationsItemModel(const QVector<const QString>& tables, const QVector<QVariant>& headers, QObject* parent) : QAbstractItemModel(parent)
{
    m_db = &g_db;
    m_sqlMainTable = tables.value(0);
    m_sqlTablePrices = tables.value(1);
    m_hHeaders = headers;
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
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex CalcCalculationsItemModel::index(int row, int column, const QModelIndex &parent)const{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    if(!parent.isValid()){
        return QModelIndex();
    }else{
         CalcCalculationsModelItem* item = static_cast<CalcCalculationsModelItem*>(parent.internalPointer())->child(row);
         if(item){
            QModelIndex itemIndex = createIndex(row, column, item);
            return itemIndex;
         }else{
             return QModelIndex();
         }
    }
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


void CalcCalculationsItemModel::setupModel(){
    if(!m_rootItems.isEmpty()){
        qDeleteAll(m_rootItems);
        m_rootItems.clear();
    }
    CalcCalculationsModelItem* item = new CalcCalculationsModelItem({0});
    m_rootItems.insert(0, item);
    QVector<QVariant> vect;
    vect.fill(QVariant(), m_hHeaders.count());
    item = new CalcCalculationsModelItem(vect, m_rootItems.value(0));
    m_rootItems.value(0)->appendChild(item);
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
    QList<int> keys = m_rootItems.keys();
    for(int i = 0; i < keys.count(); ++i){
        m_rootItems.value(keys.value(i))->sortItem();
    }
}

bool CalcCalculationsItemModel::selectItems(QVector<QVector<QVariant>>){
    return true;
}
