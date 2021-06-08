#include "calculatorcalculationstreemodel.h"
#include "calculatortreeitem.h"
#include "calculatordatabase.h"

#include <QDebug>

CalculatorCalculationsTreeModel::CalculatorCalculationsTreeModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent) : CalculatorTreeModel(sqlTable, headers, parent)
{

}

QVariant CalculatorCalculationsTreeModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();

    int iColumn = index.column();
    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        {
            CalculatorTreeItem* item =static_cast<CalculatorTreeItem*>(index.internalPointer());
            if(item->getItemType() == ItemType::Element){
                if(iColumn == 4){
                    return QString::number(item->data(iColumn).toDouble(), 'f', 2);
                }else if(iColumn == 6)
                    return item->data(iColumn).toString() + tr("г.");
            }
            break;
        }
    case Qt::TextAlignmentRole:
        if(iColumn == 6)
            return Qt::AlignCenter;
        break;
    }
    return CalculatorTreeModel::data(index, role);

}

/*
bool CalculatorCalculationsTreeModel::createDataBaseItem(const QString& tableName, QVector<QVariant>& data){
    if(data.value(FieldName::FolderId).toInt()){
        qlonglong fNum = m_db->getLastFolderNumber(tableName);
        if(fNum == -1){
            return false;
        }
        data[FieldName::FolderId] = fNum + 1;
    }
    if(!m_db->insertIntoTable(tableName, data)){
        return false;
    }
    qlonglong num  = m_db->getRowsCount(tableName);
    if(num == -1)
        exit(-2);
    data[FieldName::Id] = num;
    return true;
}

bool CalculatorCalculationsTreeModel::updateDataBaseItems(const QString& tableName, const QVector<QVector<QVariant>> &data, const QVariant& date){
    QVector<QVector<QVariant>> vect;
    for(int i = 0; i < data.count(); ++i){
        vect.push_back({0,data.value(i).value(0),date.toString(),data.value(i).value(5)});
        if(!m_db->updateTableItem(tableName, data.value(i)))
            return false;
    }
    if(!historyModel->createModelItems(vect))
        return false;
    qDebug()<<"good";
    return true;
};
*/
