#include "database.h"
#include <QFile>
#include <QDebug>
#include <QSqlQuery>

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

void DataBase::createConnection(){
    if(!QFile(DATABASE_NAME).exists())
        if(!restoreDataBase())
            exit(1);
    if(!openDataBase())
        exit(1);
}

bool DataBase::openDataBase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);
    if(!db.open()){
        qDebug()<<"error open Database";
        return false;
    }
    return true;
}

bool DataBase::restoreDataBase(){
    if(openDataBase()){
        if(createTable()){
            return true;
        }
    }
    qDebug()<<"error restore Database";
    return false;
}

bool DataBase::createTable(){
    QSqlQuery query;
    if(query.exec("CREATE TABLE " TABLE " ( "
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  TABLE_NAME " VARCHAR(15) NOT NULL, "
                                  TABLE_PRICE " INTEGER )"
                  )){
        return true;
    }
    return false;
}

void DataBase::close(){
    db.close();
}

void DataBase::insertIntoTable(){
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( "
                              TABLE_NAME ", "
                              TABLE_PRICE " )"
                            " VALUES(:Name, :Price)");
    query.bindValue(":Name", "a");
    query.bindValue(":Price", 0);
    query.exec();
}

