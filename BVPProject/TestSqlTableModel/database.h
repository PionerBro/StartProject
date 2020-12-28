#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>


#define DATABASE_HOSTNAME "TestDatabase"
#define DATABASE_NAME     "Database.db"
#define TABLE             "TestTable"
#define TABLE_NAME        "Name"
#define TABLE_PRICE       "Price"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    void createConnection();
    void insertIntoTable();
private:
    bool restoreDataBase();
    bool openDataBase();
    bool createTable();
    void close();
private:
    QSqlDatabase db;
signals:

};

#endif // DATABASE_H
