#include "mydatabase.h"

#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <QList>

#include <QDebug>

MyDataBase::MyDataBase(QObject *parent) : QObject(parent)
{

}

bool MyDataBase::createConnection(){
    if(!QFile(DATABASE_NAME).exists()){
        QMessageBox::information(0, "Error Database", tr("Базы данных не существует"));
        return false;
    }else{
        return openDatabase();
    }
}

bool MyDataBase::openDatabase(){
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName(DATABASE_HOST);
    m_db.setDatabaseName(DATABASE_NAME);
    if(m_db.open()){
        return true;
    }else{
        QMessageBox::information(0, "Error Database", tr("Не удалось открыть базу данных"));
        return false;
    }
}

void MyDataBase::closeDatabase(){
    m_db.close();
}

bool MyDataBase::createTable(){
    QSqlQuery query(m_db);
    if(query.exec("CREATE TABLE " TABLE_MATERIALS " ( " MATERIALS_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                        MATERIALS_PARENT " INTEGER, "
                                                        MATERIALS_DIR " INTEGER, "
                                                        MATERIALS_NAME " VARCHAR(50), "
                                                        MATERIALS_UNIT " VARCHAR(20), "
                                                        MATERIALS_PRICE " DOUBLE)"
                  )){
        return true;
    }else{
        QMessageBox::information(0,"Error Database", tr("Не удалось создать таблицу данных"));
        return false;
    }
}

bool MyDataBase::createTableElements(){
    QSqlQuery query(m_db);
    if(query.exec("CREATE TABLE " TABLE_ELEMENTS " ( "  ELEMENTS_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                        ELEMENTS_PARENT " INTEGER, "
                                                        ELEMENTS_DIR    " INTEGER, "
                                                        ELEMENTS_DATE " VARCHAR(20), "
                                                        ELEMENTS_NAME " VARCHAR(50), "
                                                        ELEMENTS_PRICE " DOUBLE, "
                                                        ELEMENTS_OUTPUT " DOUBLE, "
                                                        ELEMENTS_PORTION " DOUBLE)"
                  )){
        return true;
    }else{
        QMessageBox::information(0,"Error Database", tr("Не удалось создать таблицу данных"));
        return false;
    }
}

bool MyDataBase::createTableItems(){
    QSqlQuery query(m_db);
    if(query.exec("CREATE TABLE " TABLE_ITEMS " ( "  ITEMS_DOC   " INTEGER, "
                                                     ITEMS_NUM   " INTEGER, "
                                                     ITEMS_COUNT " DOUBLE)"
                  )){
        return true;
    }else{
        QMessageBox::information(0,"Error Database", tr("Не удалось создать таблицу данных"));
        return false;
    }
}

bool MyDataBase::createTableMaterialsPrices(){
    QSqlQuery query(m_db);
    if(query.exec("CREATE TABLE " TABLE_MATERIALS_PRICES " ( "  MATERIALS_PRICES_DOC   " INTEGER, "
                                                     MATERIALS_PRICES_ID    " INTEGER, "
                                                     MATERIALS_PRICES_DATE  " VARCHAR(10), "
                                                     MATERIALS_PRICES_PRICE " DOUBLE)"
                  )){
        return true;
    }else{
        QMessageBox::information(0,"Error Database", tr("Не удалось создать таблицу данных"));
        return false;
    }
}

bool MyDataBase::select(const QString& table, QList<QList<QVariant>> &data){
    QSqlQuery query(m_db);
    int eCount;                                         //количество элементов заголовка
    if(table == TABLE_MATERIALS){
        eCount = 6;
        query.prepare("SELECT* FROM " TABLE_MATERIALS);
    }else if(table == TABLE_MATERIALS_PRICES){
        eCount = 4;
        query.prepare("SELECT* FROM " TABLE_MATERIALS_PRICES);
    }else if(table == TABLE_ELEMENTS){
        eCount = 8;
        query.prepare("SELECT* FROM " TABLE_ELEMENTS);
    }/*else if(table == TABLE_ITEMS){
        eCount = 3;
        query.prepare("SELECT* FROM " TABLE_ITEMS);
    }*/else
        return false;

    if(query.exec()){
        while(query.next()){
            QList<QVariant> vect;
            for(int i = 0; i < eCount; ++i){
                //if(i == 5 && TABLE_MATERIALS){
                  //  vect<<QString::number(query.value(i).toDouble(),'f',2);
                //}else
                    vect<<query.value(i);
            }
            if(table == TABLE_ELEMENTS){
                QSqlQuery query2(m_db);
                query2.prepare("SELECT " ITEMS_NUM ", " ITEMS_COUNT " FROM " TABLE_ITEMS " WHERE " ITEMS_DOC " = :Doc");
                query2.bindValue(":Doc", query.value(0).toLongLong());
                if(!query2.exec())
                    return false;
                while (query2.next()) {
                    vect<<query2.value(0)<<query2.value(1);
                }
            }
            data<<vect;
        }
        return true;
    }else{
       return false;
    }    
}

bool MyDataBase::insertIntoTable(const QString& table, const QList<QVariant> &data){
    QSqlQuery query(m_db);
    if(table == TABLE_MATERIALS){
        query.prepare("INSERT INTO " TABLE_MATERIALS " ( " MATERIALS_PARENT ", "
                                                       MATERIALS_DIR    ", "
                                                       MATERIALS_NAME   ", "
                                                       MATERIALS_UNIT   ", "
                                                       MATERIALS_PRICE  ") "
                              "VALUES (:Parent, :Dir, :Name, :Unit, :Price)"
                    );
        query.bindValue(":Parent", data.value(1).toLongLong());
        query.bindValue(":Dir", data.value(2).toLongLong());
        query.bindValue(":Name", data.value(3).toString());
        query.bindValue(":Unit", data.value(4).toString());
        query.bindValue(":Price", data.value(5).toDouble());
    }else if(table == TABLE_MATERIALS_PRICES){
        query.prepare("INSERT INTO " TABLE_MATERIALS_PRICES " ( "  MATERIALS_PRICES_DOC ", "
                                                           MATERIALS_PRICES_ID ", "
                                                           MATERIALS_PRICES_DATE ", "
                                                           MATERIALS_PRICES_PRICE ") "
                                  "VALUES (:Doc, :id, :Date, :Price)"
                        );
        query.bindValue(":Doc", data.value(0).toLongLong());
        query.bindValue(":id", data.value(1).toLongLong());
        query.bindValue(":Date", data.value(2).toString());
        query.bindValue(":Price", data.value(3).toDouble());
    }else if(table == TABLE_ELEMENTS){
        query.prepare("INSERT INTO " TABLE_ELEMENTS " ( "  ELEMENTS_PARENT ", "
                                                           ELEMENTS_DIR ", "
                                                           ELEMENTS_DATE ", "
                                                           ELEMENTS_NAME    ", "
                                                           ELEMENTS_PRICE   ", "
                                                           ELEMENTS_OUTPUT   ", "
                                                           ELEMENTS_PORTION  ") "
                                  "VALUES (:Parent, :Dir, :Date, :Name, :Price, :OutPut, :Portion)"
                        );
        query.bindValue(":Parent", data.value(1).toLongLong());
        query.bindValue(":Dir", data.value(2).toLongLong());
        query.bindValue(":Date", data.value(3).toString());
        query.bindValue(":Name", data.value(4).toString());
        query.bindValue(":Price", data.value(5).toDouble());
        query.bindValue(":OutPut", data.value(6).toDouble());
        query.bindValue(":Portion", data.value(7).toDouble());

    }else{
        return false;
    }
    if(query.exec()){
        if(table == TABLE_ELEMENTS){
            qlonglong doc = getLastNumNumber(TABLE_ELEMENTS);
            for(int i = 8; i < data.count(); i+=2){
            query.clear();
            query.prepare("INSERT INTO " TABLE_ITEMS " ( "  ITEMS_DOC ", "
                                                               ITEMS_NUM ", "
                                                               ITEMS_COUNT ") "
                                      "VALUES (:Doc, :Num, :Count)"
                            );
            query.bindValue(":Doc", doc);
            query.bindValue(":Num", data.value(i).toLongLong());
            query.bindValue(":Count", data.value(i+1).toDouble());
            if(!query.exec())
                return false;
            }
        }
        return true;
    }
    else
        return false;
}

qlonglong MyDataBase::getLastNumNumber(const QString& table)const{
    QSqlQuery query(m_db);
    if(table == TABLE_MATERIALS){
        query.prepare("SELECT count(*) FROM " TABLE_MATERIALS);
    }else if(table == TABLE_ELEMENTS){
        query.prepare("SELECT count(*) FROM " TABLE_ELEMENTS);
    }else if(table == TABLE_MATERIALS_PRICES){
        query.prepare("SELECT max( " MATERIALS_PRICES_DOC " ) FROM " TABLE_MATERIALS_PRICES);
    }else
        return false;
    if(query.exec()){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}

qlonglong MyDataBase::getLastFolderNumber(const QString& table) const{
    QSqlQuery query(m_db);
    if(table == TABLE_MATERIALS){
        query.prepare("SELECT max( " MATERIALS_DIR  " ) FROM " TABLE_MATERIALS);
    }else if(table == TABLE_ELEMENTS){
        query.prepare("SELECT max( " ELEMENTS_DIR  " ) FROM " TABLE_ELEMENTS);
    }else
        return false;
    if(query.exec()){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}

bool MyDataBase::updateTableItem(const QString& table, const QList<QVariant> &data){
    QSqlQuery query(m_db);
    if(table == TABLE_MATERIALS){
        query.prepare("UPDATE " TABLE_MATERIALS " SET "
                                    MATERIALS_NAME  " = :Name, "
                                    MATERIALS_UNIT  " = :Unit, "
                                    MATERIALS_PRICE " = :Price "
                    "WHERE " MATERIALS_ID " = :id");
        query.bindValue(":Name", data.value(3));
        query.bindValue(":Unit", data.value(4));
        query.bindValue(":Price", data.value(5));
        query.bindValue(":id", data.value(0));
    }else if(table == TABLE_ELEMENTS){
        query.prepare("UPDATE " TABLE_ELEMENTS " SET "
                                    ELEMENTS_DATE   " = :Date, "
                                    ELEMENTS_NAME   " = :Name, "
                                    ELEMENTS_PRICE   " = :Price, "
                                    ELEMENTS_OUTPUT  " = :OutPut, "
                                    ELEMENTS_PORTION " = :Portion "
                    "WHERE " ELEMENTS_ID " = :id");
        query.bindValue(":Date", data.value(3));
        query.bindValue(":Name", data.value(4));
        query.bindValue(":Price", data.value(5));
        query.bindValue(":OutPut", data.value(6));
        query.bindValue(":Portion", data.value(7));
        query.bindValue(":id", data.value(0));       
    }else
        return false;
    if(query.exec()){
        if(table == TABLE_ELEMENTS){
            query.clear();
            int doc = data.value(0).toDouble();
            if(!query.exec("DELETE FROM " TABLE_ITEMS " WHERE " ITEMS_DOC " = " + QString::number(doc))){
                qDebug()<<"Ошибка удаления данных из TABLE_ITEMS";
                return false;
            }
            for(int i = 8; i < data.count(); i+=2){
            query.clear();
            query.prepare("INSERT INTO " TABLE_ITEMS " ( "  ITEMS_DOC ", "
                                                               ITEMS_NUM ", "
                                                               ITEMS_COUNT ") "
                                      "VALUES (:Doc, :Num, :Count)"
                            );
            query.bindValue(":Doc", doc);
            query.bindValue(":Num", data.value(i).toLongLong());
            query.bindValue(":Count", data.value(i+1).toDouble());
            if(!query.exec()){
                qDebug()<<"Ошибка внесения данных в TABLE_ITEMS";
                return false;
                }
            }
        }
        return true;
    }
    else
        return false;
}

bool MyDataBase::selectAtNum(qlonglong num, const QString &table, QList<QList<QVariant>> &data){
    QSqlQuery query(m_db);
    int eCount;
    if(table == TABLE_MATERIALS)
        eCount = 6;
    else if(table == TABLE_MATERIALS_PRICES)
        eCount = 4;
    else if(table == TABLE_ELEMENTS)
        eCount = 8;
    else if(table == TABLE_ITEMS)
        eCount = 3;
    else
        return false;
    if(query.exec("SELECT* FROM " + table + " WHERE id = " + QString::number(num))){
        while(query.next()){
            QList<QVariant> vect;
            for(int i = 0; i<eCount;++i){
                vect<<query.value(i);
            }
            data<<vect;
        }
        return true;
    }else{
        return false;
    }

}
