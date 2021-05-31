#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QSqlDatabase>

#define DATABASE_NAME "../Calculator/DataBase.db"
#define DATABASE_HOST "DataBaseHost"

#define TABLE_MATERIALS  "TableMaterials"
#define MATERIALS_ID     "id"
#define MATERIALS_PARENT "Parent"
#define MATERIALS_DIR    "Dir"
#define MATERIALS_NAME   "Name"
#define MATERIALS_UNIT   "Unit"
#define MATERIALS_PRICE  "Price"

#define TABLE_MATERIALS_PRICES "TableMaterialsPrices"
#define MATERIALS_PRICES_DOC    "Doc"
#define MATERIALS_PRICES_ID     "id"            //idTableMaterials
#define MATERIALS_PRICES_DATE   "Date"
#define MATERIALS_PRICES_PRICE  "Price"

#define TABLE_ELEMENTS   "TableElements"
#define ELEMENTS_ID      "id"
#define ELEMENTS_PARENT  "Parent"
#define ELEMENTS_DIR     "Dir"
#define ELEMENTS_DATE    "Date"
#define ELEMENTS_NAME    "Name"
#define ELEMENTS_PRICE   "Price"
#define ELEMENTS_OUTPUT  "OutPut"
#define ELEMENTS_PORTION "Portion"

#define TABLE_ITEMS      "TableItems"
#define ITEMS_DOC        "Document"             //id TableElements
#define ITEMS_NUM        "id"                   //id TableMaterials
#define ITEMS_COUNT      "Count"


class MyDataBase : public QObject
{
    Q_OBJECT
public:    
    explicit MyDataBase(QObject *parent = nullptr);

    bool createConnection();
    bool select(const QString&, QList<QList<QVariant>>& data);
    bool insertIntoTable(const QString&, const QList<QVariant>& data);
    bool updateTableItem(const QString&, const QList<QVariant>& data);
    bool selectAtNum(qlonglong num, const QString&, QList<QList<QVariant>>& data);
    qlonglong getLastNumNumber(const QString&) const;
    qlonglong getLastFolderNumber(const QString&) const;
private:
    QSqlDatabase m_db;
private:
    bool openDatabase();
    void closeDatabase();
    bool createDatabase();
    bool createTable();
    bool createTableElements();
    bool createTableItems();
    bool createTableMaterialsPrices();
signals:

};

#endif // MYDATABASE_H
