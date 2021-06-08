#include "calculatorhistorymodel.h"
#include "calculatordatabase.h"
#include "calculatorhistoryitem.h"

#include <QIcon>
#include <QDate>

#include <QDebug>

extern CalculatorDatabase g_db;

CalculatorHistoryModel::CalculatorHistoryModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent) : QAbstractItemModel(parent)
{
    m_db = &g_db;
    m_sqlTable = sqlTable;
    //setSortCol(0);
    m_hHeaderData = headers;
    setupModelData();
}

CalculatorHistoryModel::~CalculatorHistoryModel(){
    delete m_root;
}

QVariant CalculatorHistoryModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();

    int iColumn = index.column();

    switch (role) {
    case Qt::DecorationRole:
        if(!iColumn){
            return QIcon("../Calculator/blue1.png").pixmap(QSize(25,25));
        }
        return QVariant();
    case Qt::EditRole:
    case Qt::DisplayRole:
    {
        if(!iColumn)
            return QVariant();
        QVariant data = static_cast<CalculatorHistoryItem*>(index.internalPointer())->data(iColumn);
        if(iColumn == 2)
            return data.toDate();
        if(iColumn == 3)
            return QString::number(data.toDouble(), 'f', 2);
        return data;
    }
    default:
        return QVariant();
    }
}

Qt::ItemFlags CalculatorHistoryModel::flags(const QModelIndex &index) const{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex CalculatorHistoryModel::index(int row, int column, const QModelIndex &parent) const{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    CalculatorHistoryItem* item;
    if(!parent.isValid())
        item = m_curRootItem;
    else
        item = static_cast<CalculatorHistoryItem*>(parent.internalPointer());
    CalculatorHistoryItem* child = item->child(row);
    if(child)
        return createIndex(row, column, child);
    else
        return QModelIndex();
}

QVariant CalculatorHistoryModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_hHeaderData.value(section);
    return QVariant();
}

QModelIndex CalculatorHistoryModel::parent(const QModelIndex &index) const{
    if(!index.isValid())
        return QModelIndex();

    CalculatorHistoryItem *childItem = static_cast<CalculatorHistoryItem*>(index.internalPointer());
    CalculatorHistoryItem *parentItem = childItem->parent();
    if(parentItem == m_curRootItem)
        return QModelIndex();
    return createIndex(parentItem->row(),0, parentItem);
}

int CalculatorHistoryModel::rowCount(const QModelIndex &parent) const{
    CalculatorHistoryItem* parentItem;
    if(parent.column()>0)
        return 0;
    if(!parent.isValid())
        parentItem = m_curRootItem;
    else
        parentItem = static_cast<CalculatorHistoryItem*>(parent.internalPointer());
    return parentItem->childCount();
}

int CalculatorHistoryModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return m_hHeaderData.count();
}


//усинавливаем sortCol каждого родителя и сортируем
void CalculatorHistoryModel::setSortCol(int col){
    m_sortCol = col;
    beginResetModel();
    m_root->setSortCol(col);
    QList<qlonglong> keys = m_materialsId.keys();
    for(qlonglong i = 1; i < keys.count(); ++i){
        m_materialsId.value(keys.value(i))->setSortCol(col);
        m_materialsId.value(keys.value(i))->sortItem();
    }
    endResetModel();
}

bool CalculatorHistoryModel::createModelItem(QVector<QVariant>& data){
    ++m_lastDoc;
    data[0] = m_lastDoc;
    if(!createDataBaseItem(m_sqlTable, data))
        return false;
    qlonglong matId = data.value(1).toLongLong();
    if(!m_materialsId.contains(matId)){
        CalculatorHistoryItem* item = new CalculatorHistoryItem({matId}, m_root);
        m_root->appendChild(item);
        m_materialsId.insert(matId, item);
    }
    beginResetModel();
    CalculatorHistoryItem* item = new CalculatorHistoryItem(data, m_materialsId.value(matId));
    m_materialsId.value(matId)->appendChild(item);
    m_materialsId.value(matId)->sortItem();
    endResetModel();

    return true;
}

//устанавлием номер документа в векторах данных, добаялем в БД и создаем элементы, сортируем
bool CalculatorHistoryModel::createModelItems(QVector<QVector<QVariant>> &data){
    ++m_lastDoc;
    for(int i = 0; i < data.count(); ++i){
        data[i][0] = m_lastDoc;
        if(!createDataBaseItem(m_sqlTable, data.value(i)))
            return false;
        qlonglong matId = data.value(i).value(1).toLongLong();
        if(!m_materialsId.contains(matId)){
            CalculatorHistoryItem* item = new CalculatorHistoryItem({matId}, m_root);
            m_root->appendChild(item);
            m_materialsId.insert(matId, item);
        }
        beginResetModel();
        CalculatorHistoryItem* item = new CalculatorHistoryItem(data.value(i), m_materialsId.value(matId));
        m_materialsId.value(matId)->appendChild(item);
        m_materialsId.value(matId)->sortItem();
        endResetModel();
    }
    return true;
}

/*
 *  создаем корень, получаем данные из БД
 *  если у элемента с id нету папки создаем и добавляем в map папок и сортируем
 *  корень - первый элемент map, получаем последний номер документа из БД
*/

void CalculatorHistoryModel::setupModelData(){
    if(m_root){
        delete m_root;
        m_materialsId.clear();
    }
    m_root = new CalculatorHistoryItem({0});
    m_curRootItem = m_root;
    m_materialsId.insert(0, m_root);

    QVector<QVector<QVariant>> dbData;
    if(selectDataBaseAll(m_sqlTable, dbData)){
        for(int i = 0; i < dbData.count(); ++i){
            QVector<QVariant> itemData = dbData.value(i);
            qlonglong matId = itemData.value(1).toLongLong();
            if(!m_materialsId.contains(matId)){
                CalculatorHistoryItem* item = new CalculatorHistoryItem({matId}, m_root);
                m_root->appendChild(item);
                m_materialsId.insert(matId, item);
            }
            CalculatorHistoryItem* item = new CalculatorHistoryItem(itemData, m_materialsId.value(matId));
            m_materialsId.value(matId)->appendChild(item);
        }
        QList<qlonglong> keys = m_materialsId.keys();
        for(qlonglong i = 1; i < keys.count(); ++i){
            m_materialsId.value(keys.value(i))->sortItem();
        }
        m_lastDoc = getLastDocNumber();
        if(m_lastDoc == -1)
            return;
    }
}

bool CalculatorHistoryModel::selectDataBaseAll(const QString& table, QVector<QVector<QVariant>>& data){
    if(m_db->select(table, data))
        return true;
    return false;
}

bool CalculatorHistoryModel::createDataBaseItem(const QString& table, const QVector<QVariant>& data){
    if(m_db->insertIntoTable(table, data))
        return true;
    return false;
}

qlonglong CalculatorHistoryModel::getLastDocNumber()const{
    return m_db->getMaxFieldValue(0, m_sqlTable);
}

//устанавливаем корень для показа элементов соответствуещего id
bool CalculatorHistoryModel::setRootNum(qlonglong num){
    if(!m_materialsId.contains(num))
        return false;
    beginResetModel();
    m_curRootItem = m_materialsId.value(num);
    endResetModel();
    return true;
}

