#include "mydatabase.h"

#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <QList>

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

bool MyDataBase::select(const QString& table, QList<QList<QVariant>> &data){
    QSqlQuery query(m_db);
    int eCount;                                         //количество элементов заголовка
    if(table == TABLE_MATERIALS){
        eCount = 6;
        query.prepare("SELECT* FROM " TABLE_MATERIALS);
    }
    else if(table == TABLE_ELEMENTS){
        eCount = 8;
        query.prepare("SELECT* FROM " TABLE_ELEMENTS);
    }
    else
        return false;

    if(query.exec()){
        while(query.next()){
            QList<QVariant> vect;
            for(int i = 0; i < eCount; ++i)
                vect<<query.value(i);
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
        query.bindValue(":Date", data.value(3).toLongLong());
        query.bindValue(":Name", data.value(4).toLongLong());
        query.bindValue(":Price", data.value(5).toString());
        query.bindValue(":OutPut", data.value(6).toString());
        query.bindValue(":Portion", data.value(7).toDouble());

    }else{
        return false;
    }
    if(query.exec())
        return true;
    else
        return false;
}

qlonglong MyDataBase::getLastNumNumber(const QString& table)const{
    QSqlQuery query(m_db);
    if(table == TABLE_MATERIALS){
        query.prepare("SELECT count(*) FROM " TABLE_MATERIALS);
    }else if(table == TABLE_ELEMENTS){
        query.prepare("SELECT count(*) FROM " TABLE_ELEMENTS);
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
    if(query.exec())
        return true;
    else
        return false;
}

