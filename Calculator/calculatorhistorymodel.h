#ifndef CALCULATORHISTORYMODEL_H
#define CALCULATORHISTORYMODEL_H

#include <QAbstractItemModel>
#include <QMap>

class CalculatorDatabase;
class CalculatorHistoryItem;

class CalculatorHistoryModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    CalculatorHistoryModel(const QString& sqlTable, const QVector<QVariant>& headers = {}, QObject* parent = 0);
    virtual ~CalculatorHistoryModel();

    QVariant data(const QModelIndex& index, int role)const override;
    Qt::ItemFlags flags(const QModelIndex &index)const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex())const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index)const override;
    int rowCount(const QModelIndex &parent = QModelIndex())const override;
    int columnCount(const QModelIndex &parent = QModelIndex())const override;
    void setSortCol(int col);
    bool setRootNum(qlonglong num);
    bool createModelItem(QVector<QVariant> &data);
    bool createModelItems(QVector<QVector<QVariant>>& data);
private:
    bool selectDataBaseAll(const QString& table, QVector<QVector<QVariant>>& data);
    bool createDataBaseItem(const QString& table, const QVector<QVariant>& data);
    qlonglong getLastDocNumber() const;
    void setupModelData();
protected:
    QString m_sqlTable;
    CalculatorDatabase* m_db;
    qlonglong m_lastDoc;
    int m_sortCol;
    QVector<QVariant> m_hHeaderData;
    CalculatorHistoryItem* m_curRootItem;
    CalculatorHistoryItem* m_root = nullptr;
    QMap<qlonglong, CalculatorHistoryItem*> m_materialsId;
};

#endif // CALCULATORHISTORYMODEL_H
