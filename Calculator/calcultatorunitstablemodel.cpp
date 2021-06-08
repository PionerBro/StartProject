#include "calcultatorunitstablemodel.h"
#include "calculatortreeitem.h"

#include <QIcon>

CalcultatorUnitsTableModel::CalcultatorUnitsTableModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent): CalculatorTreeModel(sqlTable, headers, parent)
{

}

QVariant CalcultatorUnitsTableModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();

    int column = index.column();

    switch (role) {
    case Qt::DecorationRole:
        if(!column)
            return QIcon("../Calculator/blue1.png").pixmap(QSize(25,25));;
        return QVariant();
    case Qt::DisplayRole:
        if(!column)
            return QVariant();
        return static_cast<CalculatorTreeItem*>(index.internalPointer())->data(index.column());
    default:
        return QVariant();
    }
}
