#ifndef ITEMTABLE_H
#define ITEMTABLE_H

#include <QDialog>

class ItemTableDatabase;
class QSqlTableModel;
class QTableView;


class ItemTable : public QDialog
{
    Q_OBJECT
public:
    ItemTable(QStringList& data, QWidget* parent = nullptr);
private:
    QStringList& m_data;
    ItemTableDatabase* m_db;
    QSqlTableModel* model;
    QTableView* view;
private:
    void setupModel(const QString &table, const QStringList& headers);
    void createView();
private slots:
    void createItem();
    void editItem();
    void deleteItem();   
    void selectItem(const QModelIndex&);

signals:

};

#endif // ITEMTABLE_H
