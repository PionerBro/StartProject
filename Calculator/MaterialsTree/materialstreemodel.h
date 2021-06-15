#ifndef MATERIALSTREEMODEL_H
#define MATERIALSTREEMODEL_H

#include "../Database/treemodel.h"

class MaterialsTreeModel : public TreeModel
{
    Q_OBJECT
public:
    MaterialsTreeModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent);
    virtual ~MaterialsTreeModel();
};

#endif // MATERIALSTREEMODEL_H

