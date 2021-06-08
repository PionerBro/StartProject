#ifndef CALCULATORMATERIALSTREEMODEL_H
#define CALCULATORMATERIALSTREEMODEL_H

#include "calculatortreemodel.h"
#include "calculatorhistorymodel.h"

class CalculatorMaterialsTreeModel : public CalculatorTreeModel
{
    Q_OBJECT
public:
    CalculatorMaterialsTreeModel(const QString& sqlTable, const QVector<QVariant>& headers = {}, QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role)const override;
private:
    virtual bool createDataBaseItem(const QString& tableName, QVector<QVariant>& data) override;
    virtual bool updateDataBaseItems(const QString& tableName, const QVector<QVector<QVariant>> &data, const QVariant& date) override;
private:
    CalculatorHistoryModel* historyModel;
};

#endif // CALCULATORMATERIALSTREEMODEL_H
