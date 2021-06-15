#ifndef CALCULATORCALCULATIONSTREEMODEL_H
#define CALCULATORCALCULATIONSTREEMODEL_H

#include "calculatortreemodel.h"

class CalculatorCalculationsTreeModel : public CalculatorTreeModel
{
    Q_OBJECT
public:
    CalculatorCalculationsTreeModel(const QString& sqlTable, const QVector<QVariant>& headers = {}, QObject* parent = 0);

    QVariant data(const QModelIndex& index, int role)const override;
    bool createDataBaseItem(const QString& tableName, QVector<QVariant>& data) override;
};

#endif // CALCULATORCALCULATIONSTREEMODEL_H
