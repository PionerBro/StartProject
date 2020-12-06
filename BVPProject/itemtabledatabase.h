#ifndef ITEMTABLEDATABASE_H
#define ITEMTABLEDATABASE_H

#include <QObject>
#include <QSqlDatabase>


#define DATABASE_HOSTNAME   "BVPProject"
#define DATABASE_NAME       "TableItem.db"

#define TABLE               "Items"
#define TABLE_VENDORCODE    "VendorCode"
#define TABLE_NAME          "Name"
#define TABLE_MEASURE       "Measure"
#define TABLE_PRICE         "Price"


class ItemTableDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ItemTableDatabase(QObject *parent = nullptr);
    void createConnection();
    bool insertIntoTable(const QVariantList& data);
    bool updateRecord(const QVariantList& data);
private:
    const QStringList m_data;
    QSqlDatabase db;
private:
    void restoreDatabase();
    bool openDatabase();
    void closeDatabase();
    bool createTable();
signals:

};

#endif // ITEMTABLEDATABASE_H
