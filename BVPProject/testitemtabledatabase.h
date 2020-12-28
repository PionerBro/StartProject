#ifndef TESTITEMTABLEDATABASE_H
#define TESTITEMTABLEDATABASE_H

#include <QObject>
#include <QSqlDatabase>

class QStandardItemModel;
class QStandardItem;

#define DATABASE_HOSTNAME   "BVPProject"
#define DATABASE_NAME       "TestTableItem.db"

#define TABLE               "Test"
#define TABLE_VENDORCODE    "VendorCode"
#define TABLE_NAME          "Name"
#define TABLE_MEASURE       "Measure"
#define TABLE_PRICE         "Price"
#define TABLE_FOLDER        "Folder"
#define TABLE_PARENT        "Parent"





class TestItemTableDatabase : public QObject
{
    Q_OBJECT
public:
    explicit TestItemTableDatabase(QObject *parent = nullptr);
    void createConnection();
    bool insertIntoTable(const QVariantList& data);
    bool insertFolderIntoTable(const QVariantList& data);
    bool updateRecord(const QVariantList& data);
    void select(QStandardItem* parent, int branch);
private:
    enum QueryValue{
        Id = 0,
        VendorCode = 1,
        Name = 2,
        Measure = 3,
        Price = 4,
        Folder = 5,
        Parent = 6,
        QueryValueCount = 7
    };
    const QStringList m_data;
    QSqlDatabase db;
private:
    void restoreDatabase();
    bool openDatabase();
    void closeDatabase();
    bool createTable();
signals:


};

#endif // TESTITEMTABLEDATABASE_H
