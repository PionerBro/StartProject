#include "testitemtabledatabase.h"
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariantList>
#include <QStandardItemModel>
#include <QDebug>
#include <QSqlRecord>


TestItemTableDatabase::TestItemTableDatabase(QObject *parent) : QObject(parent)
{

}

void TestItemTableDatabase::createConnection(){
    if(!QFile(DATABASE_NAME).exists()){
        this->restoreDatabase();
    }else{
        this->openDatabase();
    }
}

void TestItemTableDatabase::restoreDatabase(){
    if(openDatabase()){
        if(createTable()){
            return;
        }
    }
    QMessageBox::information(0, "Database",tr("Ошибка восстановления базы данных"));
    exit(1);
}

bool TestItemTableDatabase::openDatabase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);
    if(db.open()){
        return true;
    }
    QMessageBox::information(0, "Database", tr("Не удалось открыть базу данных"));
    return false;
}

void TestItemTableDatabase::closeDatabase(){
    db.close();
}

bool TestItemTableDatabase::createTable(){
    QSqlQuery query;
    if(query.exec("CREATE TABLE " TABLE " ( id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                    TABLE_VENDORCODE " VARCHAR(10), "
                                    TABLE_NAME       " VARCHAR(50), "
                                    TABLE_MEASURE    " VARCHAR(10), "
                                    TABLE_PRICE      " INTEGER, "
                                    TABLE_FOLDER     " INTEGER NOT NULL DEFAULT 0 CHECK( " TABLE_FOLDER " IN(0,1)), "
                                    TABLE_PARENT     " INTEGER)"
                  )){
        return true;
    }
    QMessageBox::information(0, "Database", tr("Не удалось создать таблицу данных"));
    return false;
}

bool TestItemTableDatabase::insertIntoTable(const QVariantList &data){
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " TABLE_VENDORCODE ", "
                                            TABLE_NAME        ", "
                                            TABLE_MEASURE     ", "
                                            TABLE_PRICE       ", "
                                            TABLE_FOLDER      ", "
                                            TABLE_PARENT      " )"
                  "VALUES ( :VendorCode, :Name, :Measure, :Price, 0, :Parent )"
                  );
    query.bindValue(":VendorCode", data[0].toString());
    query.bindValue(":Name",       data[1].toString());
    query.bindValue(":Measure",    data[2].toString());
    query.bindValue(":Price",      data[3].toString());
    query.bindValue(":Parent",     data[4].toInt());
    if(query.exec()){
        return true;
    }
    return false;
}

bool TestItemTableDatabase::insertFolderIntoTable(const QVariantList &data){
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " TABLE_NAME        ", "
                                            TABLE_MEASURE     ", "
                                            TABLE_FOLDER      ", "
                                            TABLE_PARENT      " )"
                  "VALUES (:Name, :Measure, 1, :Parent )"
                  );
    query.bindValue(":Name",       data[0].toString());
    query.bindValue(":Measure",    data[1].toString());
    query.bindValue(":Parent",     data[2].toString());
    if(query.exec()){
        return true;
    }
    return false;
}



bool TestItemTableDatabase::updateRecord(const QVariantList &data){
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

void TestItemTableDatabase::select(QStandardItem* parentItem, int branch){
    QSqlQuery query;
    query.exec("SELECT* FROM " TABLE " WHERE " TABLE_PARENT " = " + QString::number(branch));
    while(query.next()){
        QStandardItem* item = new QStandardItem(query.value(VendorCode).toString()+query.value(Name).toString());
        if(query.value(Folder).toInt()){
            QList<QStandardItem*> list;
            list<<item;
            item = new QStandardItem(query.value(Id).toString());
            list<<item;
            for(int i = Measure; i<QueryValueCount; ++i){
                item = new QStandardItem(query.value(i).toString());
                list<<item;
            }
            parentItem->appendRow(list);
            select(list[0],query.value(Id).toInt());
        }else{         
            QList<QStandardItem*> list;
            list<<item;
            item = new QStandardItem(query.value(Id).toString());
            list<<item;
            for(int i = Measure; i<QueryValueCount; ++i){
                item = new QStandardItem(query.value(i).toString());
                list<<item;
            }
            parentItem->appendRow(list);
        }
    }
}






























