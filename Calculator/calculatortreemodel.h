#ifndef CALCULATORTREEMODEL_H
#define CALCULATORTREEMODEL_H

#include <QAbstractItemModel>


class CalculatorTreeItem;
class CalculatorDatabase;

class CalculatorTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    CalculatorTreeModel(const QString& sqlTable, const QVector<QVariant>& headers = {}, QObject* parent = 0);
    virtual ~CalculatorTreeModel();

    QVariant data(const QModelIndex& index, int role)const override;
    Qt::ItemFlags flags(const QModelIndex &index)const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex())const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index)const override;
    int rowCount(const QModelIndex &parent = QModelIndex())const override;
    int columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVector<QVariant> getRowData(const QModelIndex& index) const;

    void setSortCol(int col);
    void sethHeaderData(QVector<QVariant>& headers);
    void setColumnEditable(int col, bool value = true);
    CalculatorTreeItem* currentRoot() const;
    bool createModelItem(QVector<QVariant>& data);
    bool updateModelItem(const QVector<QVariant>& data, const QModelIndex& index);
    void setupModelData();
    void setCalculatorDatabase(CalculatorDatabase* db);
    bool getEditModeValue()const;
private:
    bool updateModelItems(const QVector<QVector<QVariant>> &data, const QVector<CalculatorTreeItem*> &items, const QVariant& date = QVariant());
    virtual bool selectDataBaseAll(const QString& tableName, QVector<QVector<QVariant>>& data);
    virtual bool createDataBaseItem(const QString& tableName, QVector<QVariant>& data);
    virtual bool updateDataBaseItem(const QString& tableName, const QVector<QVariant>& data);
    virtual bool updateDataBaseItems(const QString& tableName, const QVector<QVector<QVariant>> &data, const QVariant& date = QVariant());
protected:
    QVariant getReserveData(int row, int col) const;
    bool isEditableCol(int col) const;
protected:
    CalculatorDatabase* m_db;
    QString m_sqlTable;
private:
    int m_sortCol;
    QVector<QVariant> m_hHeaderData;
    CalculatorTreeItem* m_root = nullptr;
    CalculatorTreeItem* m_curRootItem;
    QVector<CalculatorTreeItem*> m_treeElements;
    QVector<CalculatorTreeItem*> m_treeFolders;
    bool treeMode = false;
    bool editMode = false;
    QVector<bool> editableCols;
    QMap<int, QVector<QVariant>> reserveData;
    QVector<bool> reserveCh;
    QVector<bool> reserveAc;
    QList<int> changedRow;
public slots:
    void setTreeMode(bool value);
    void setEditMode(bool value);
    void reserveDataChange(int row, int column, const QString& text);
private slots:
    void rootItemChanged(QModelIndex index);
    void reserveDataAccept(const QVariant& date);
    void reserveDataReject();
signals:
    void sendData(QVector<QVariant>&);
    void reserveDataChanged();
    void reserveDataAccepted();
    void reserveDataRejected();
};

#endif // CALCULATORTREEMODEL_H
