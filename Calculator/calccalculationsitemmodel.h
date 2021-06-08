#ifndef CALCCALCULATIONSITEMMODEL_H
#define CALCCALCULATIONSITEMMODEL_H

#include <QAbstractItemModel>
#include <QMap>

class CalcCalculationsModelItem;
class CalculatorDatabase;

class CalcCalculationsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    CalcCalculationsItemModel(const QVector<const QString>& tables, const QVector<QVariant>& headers = {}, QObject* parent = nullptr);
    ~CalcCalculationsItemModel();

    QVariant data(const QModelIndex& index, int role)const override;
    Qt::ItemFlags flags(const QModelIndex &index)const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex())const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index)const override;
    int rowCount(const QModelIndex &parent = QModelIndex())const override;
    int columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVector<QVariant> getRowData(const QModelIndex& index) const;
private:
    void setupModel();
    bool selectItems(QVector<QVector<QVariant>>);
private:
    QMap<int, CalcCalculationsModelItem*> m_rootItems;
    CalcCalculationsModelItem* m_root;
    CalculatorDatabase* m_db;
    QString m_sqlMainTable;
    QString m_sqlTablePrices;
    QVector<QVariant> m_hHeaders;
};

#endif // CALCCALCULATIONSITEMMODEL_H
