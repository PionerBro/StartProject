#include "itemtabledatabase.h"
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariantList>

ItemTableDatabase::ItemTableDatabase(QObject *parent) : QObject(parent)
{

}

void ItemTableDatabase::createConnection(){
    if(!QFile(DATABASE_NAME).exists()){
        this->restoreDatabase();
    }else{
        this->openDatabase();
    }
}

void ItemTableDatabase::restoreDatabase(){
    if(openDatabase()){
        if(createTable()){
            return;
        }
    }
    QMessageBox::information(0, "Database",tr("Ошибка восстановления базы данных"));
    exit(1);
}

bool ItemTableDatabase::openDatabase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);
    if(db.open()){
        return true;
    }
    QMessageBox::information(0, "Database", tr("Не удалось открыть базу данных"));
    return false;
}

void ItemTableDatabase::closeDatabase(){
    db.close();
}

bool ItemTableDatabase::createTable(){
    QSqlQuery query;
    if(query.exec("CREATE TABLE " TABLE " ( id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                    TABLE_VENDORCODE ", "
                                    TABLE_NAME       ", "
                                    TABLE_MEASURE    ", "
                                    TABLE_PRICE      " )"
                  )){
        return true;
    }
    QMessageBox::information(0, "Database", tr("Не удалось создать таблицу данных"));
    return false;
}

bool ItemTableDatabase::insertIntoTable(const QVariantList &data){
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " TABLE_VENDORCODE ", "
                                            TABLE_NAME        ", "
                                            TABLE_MEASURE     ", "
                                            TABLE_PRICE       " ) "
                  "VALUES ( :VendorCode, :Name, :Measure, :Price )"
                  );
    query.bindValue(":VendorCode", data[0].toString());
    query.bindValue(":Name",       data[1].toString());
    query.bindValue(":Measure",    data[2].toString());
    query.bindValue(":Price",      data[3].toString());
    if(query.exec()){
        return true;
    }
    return false;
}

bool ItemTableDatabase::updateRecord(const QVariantList &data){
    QSqlQuery query;
    query.prepare("UPDATE " TABLE " SET "
                            TABLE_VENDORCODE " = :VendorCode, "
                            TABLE_NAME       " = :Name, "
                            TABLE_MEASURE    " = :Measure, "
                            TABLE_PRICE      " = :Price "
                  "WHERE id = :id");
    query.bindValue(":VendorCode",  data[0].toString());
    query.bindValue(":Name",        data[1].toString());
    query.bindValue(":Measure",     data[2].toString());
    query.bindValue(":Price",       data[3].toString());
    query.bindValue(":id",          data[4].toInt());
    if(query.exec()){
        return true;
    }
    return false;
}




















