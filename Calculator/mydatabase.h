#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QSqlDatabase>

#define DATABASE_NAME "DataBase.db"
#define DATABASE_HOST "DataBaseHost"

#define TABLE_MATERIALS  "TableMaterials"
#define MATERIALS_ID     "id"
#define MATERIALS_PARENT "Parent"
#define MATERIALS_DIR    "Dir"
#define MATERIALS_NAME   "Name"
#define MATERIALS_UNIT   "Unit"
#define MATERIALS_PRICE  "Price"


class MyDataBase : public QObject
{
    Q_OBJECT
public:
    explicit MyDataBase(QObject *parent = nullptr);

    bool createConnection();
    bool select(QList<QList<QVariant>>& data);
    bool insertIntoTable(const QList<QVariant>& data);
    qlonglong getLastNumNumber();
private:
    QSqlDatabase m_db;
private:
    bool openDatabase();
    void closeDatabase();
    bool createDatabase();
    bool createTable();
signals:

};

#endif // MYDATABASE_H
