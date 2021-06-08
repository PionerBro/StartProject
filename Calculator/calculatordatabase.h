#ifndef CALCULATORDATABASE_H
#define CALCULATORDATABASE_H

#include <QObject>
#include <QSqlDatabase>

#define CALCULATOR_DATABASE_NAME "../Calculator/CalculatorDataBase.db"
#define CALCULATOR_DATABASE_HOST "CalculatorDataBaseHost"

#define TABLE_MATERIALS  "TableMaterialsTree"
#define MATERIALS_ID     "id"
#define MATERIALS_PARENT "ParentId"
#define MATERIALS_DIR    "DirId"
#define MATERIALS_NAME   "Name"
#define MATERIALS_UNIT   "Unit"
#define MATERIALS_PRICE  "Price"

#define TABLE_MATERIALS_PRICES "TableMaterialsPrices"
#define MATERIALS_PRICES_DOC    "Doc"
#define MATERIALS_PRICES_ID     "id"            //idTableMaterials
#define MATERIALS_PRICES_DATE   "Date"
#define MATERIALS_PRICES_PRICE  "Price"

#define TABLE_ELEMENTS_TREE   "TableElementsTree"
#define ELEMENTS_TREE_ID      "id"
#define ELEMENTS_TREE_PARENT  "ParentId"
#define ELEMENTS_TREE_DIR     "Dir"
#define ELEMENTS_TREE_NAME    "Name"
#define ELEMENTS_TREE_PRICE   "Price"
#define ELEMENTS_TREE_OUTPUT  "OutPut"
#define ELEMENTS_TREE_PORTION "Portion"

#define TABLE_ITEMS      "TableElementsTreeItems"
#define ITEMS_DOC        "Document"
#define ITEMS_EL_NUM     "ElementId"            //id TableElements
#define ITEMS_MAT_NUM    "MaterialsId"          //id TableMaterials
#define ITEMS_MAT_NAME   "MaterialsName"
#define ITEMS_COUNT      "Count"
#define ITEMS_PRICE      "Price"                //price TableMaterialsPrices

#define TABLE_ELEMENTS          "TableElements"
#define TABLE_ELEMENTS_DOC      "Document"
#define TABLE_ELEMENTS_DATE     "Date"
#define TABLE_ELEMENTS_NUM      "id"           //id TableElementsTree
#define TABLE_ELEMENTS_NAME     "Name"
#define TABLE_ELEMENTS_PRICE    "Price"
#define TABLE_ELEMENTS_OUTPUT   "OutPut"
#define TABLE_ELEMENTS_PORTION  "Portion"

#define TABLE_UNITS             "TableUnits"
#define TABLE_UNITS_ID          "id"
#define TABLE_UNITS_NAME        "Name"

class CalculatorDatabase : public QObject
{
    Q_OBJECT
public:
    explicit CalculatorDatabase(QObject *parent = nullptr);
    ~CalculatorDatabase();

    bool createConnection();
    bool select(const QString& tableName, QVector<QVector<QVariant>>& data);
    bool insertIntoTable(const QString& tableName, const QVector<QVariant>& data);
    bool updateTableItem(const QString& tableName, const QVector<QVariant>& data);
    bool selectAtNum(qlonglong num, const QString& tableName, QList<QList<QVariant>>& data);
    qlonglong getRowsCount(const QString& tableName) const;
    qlonglong getLastFolderNumber(const QString& tableName) const;
    qlonglong getMaxFieldValue(int field, const QString& table);
private:
    QSqlDatabase m_db;
private:
    bool openDatabase();
    void closeDatabase();
    bool createDatabase();
    bool createTable(const QString& tableName);
signals:

};

#endif // CALCULATORDATABASE_H
