#include "calculatormaterialstreemodel.h"
#include "calculatordatabase.h"
#include "calculatortreeitem.h"

#include <QDebug>

CalculatorMaterialsTreeModel::CalculatorMaterialsTreeModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent) : CalculatorTreeModel(sqlTable, headers, parent)
{

}


QVariant CalculatorMaterialsTreeModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();

    if(index.column() == 5 && (role == Qt::DisplayRole || role == Qt::EditRole)){
        if((!getEditModeValue()) || (!isEditableCol(5))){
            CalculatorTreeItem* item = static_cast<CalculatorTreeItem*>(index.internalPointer());
            if(item->getItemType() == ItemType::Element)
               return QString::number(item->data(5).toDouble(), 'f', 2);
        }else{
            return QString::number(getReserveData(index.row(), 5).toDouble(), 'f', 2);
        }
    }
    return CalculatorTreeModel::data(index, role);
}

bool CalculatorMaterialsTreeModel::createDataBaseItem(const QString& tableName, QVector<QVariant>& data){
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


