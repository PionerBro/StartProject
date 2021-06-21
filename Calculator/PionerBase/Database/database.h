#ifndef DATABASE_H
#define DATABASE_H

#define DB_NAME     "../PionerBase/Database/Pioner.db"
#define DB_HOSTNAME "PionerHost"

#define TABLE_DBTABLES "DBTables"

#include <QSqlDatabase>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QPair>

#include <QDebug>

class Database : public QSqlDatabase
{
public:
    Database();
    Database(const QSqlDatabase&);
    ~Database();

    bool openDatabase();
private:
    struct TableInformation{
        struct TableField{
            TableField(): name(""),type(""),length(0),precision(0){}
            TableField(const QString& iName, QString iType, int iLength, int iPrecision):name(iName), type(iType), length(iLength), precision(iPrecision){}

            QString name;
            QString type;
            int length;
            int precision;
        };

        enum TableType{
            NoType = 0,
            List = 1,
            Journal = 2,
            Document = 3
        };

        TableInformation():code(0),type(0){}
        TableInformation(int iCode,int iType):code(iCode), type(iType){}
        ~TableInformation(){qDeleteAll(fields);}
        int code;
        int type;
        QVector<TableField*> fields;
    };


    QStringList m_sqlTablesNames;
    QMap<QString, TableInformation*> m_sqlTables;
    QMap<QString, int> m_sqlTablesData;
private:
    bool selectTablesInformation();
    bool selectTables();
    bool selectTable(const QString& table, TableInformation* info);

};

#endif // DATABASE_H
