#include "materialstreemodel.h"

MaterialsTreeModel::MaterialsTreeModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent):TreeModel(sqlTable,headers,parent)
{

}

MaterialsTreeModel::~MaterialsTreeModel(){

}
