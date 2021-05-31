#ifndef CALCULTATORUNITSTABLEMODEL_H
#define CALCULTATORUNITSTABLEMODEL_H

#include "calculatortreemodel.h"

class CalcultatorUnitsTableModel : public CalculatorTreeModel
{
    Q_OBJECT
public:
    CalcultatorUnitsTableModel(const QString& sqlTable, const QVector<QVariant>& headers = {}, QObject* parent = 0);

    virtual QVariant data(const QModelIndex& index, int role) const override;
};

#endif // CALCULTATORUNITSTABLEMODEL_H
