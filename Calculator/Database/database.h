#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>

#include "treeitem.h"
#include "treemodel.h"

#define DB_NAME    "../Calculator/sDatabase.db"
#define DB_HOSTAME "DatabaseHost"

#define TABLE_MATERIALS_TREE "TableMaterialsTree"
#define MAT_TREE_ID          "id"
#define MAT_TREE_PARENT      "ParenId"
#define MAT_TREE_DIR         "FolderID"
#define MAT_TREE_NAME        "Name"
#define MAT_TREE_UNIT        "Unit"
#define MAT_TREE_PRICE       "Price"

#define TABLE_MATERIALS_PRICES "TableMaterialsPrices"
#define MAT_PRS_DOC            "DocNum"
#define MAT_PRS_MAT_ID         "MatId"
#define MAT_PRS_DATE           "Date"
#define MAT_PRS_PRICE          "Price"

#define TABLE_UNITS "TableUnits"
#define UNITS_ID    "id"
#define UNITS_NAME  "Name"

class Database : public QSqlDatabase
{
public:
    Database();
    Database(const QSqlDatabase&);
    ~Database();

    bool openDatabase();

    bool selectMaterialsTreeItems();
    qlonglong insertMaterialsTreeItem(const QVector<QVariant>& data);
    const TreeItem* getRootItem();


    bool selectUnitsItems();
    qlonglong findUnitsId(const QString& name) const;
    QVector<TreeItem*>& getUnitsElements();
private:
    void checkError(const QSqlError&);

    bool createDatabase();
    bool createTable(const QString& table);
    bool createIndex(const QString& table, const QString& field);
    bool setForeignKey();

private:
    QVector<TreeItem*> materialsTreeFolders;
    QVector<TreeItem*> materialsTreeElements;
    QVector<TreeItem*> unitsElements;
};

#endif // DATABASE_H
