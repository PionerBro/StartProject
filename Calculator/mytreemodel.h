#ifndef MYTREEMODEL_H
#define MYTREEMODEL_H

#include <QAbstractItemModel>

class MyTreeItem;


class MyTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    MyTreeModel(const QList<QVariant> &data, QObject* parent = 0);
    ~MyTreeModel();

    QVariant data(const QModelIndex& index, int role)const override;
    Qt::ItemFlags flags(const QModelIndex &index)const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex())const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index)const override;
    int rowCount(const QModelIndex &parent = QModelIndex())const override;
    int columnCount(const QModelIndex &parent = QModelIndex())const override;
private:
    void setupModelData(const QList<QList<QVariant>> &lines, MyTreeItem* parent);

    QList<QVariant> m_header;
    MyTreeItem* rootItem;
    MyTreeItem* root;
signals:
    void sendData(QString);
private slots:
    void rootItemChanged(QModelIndex index);
};

#endif // MYTREEMODEL_H
