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
        createTableElements();
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
    if(query.exec("CREATE TABLE " TABLE_ELEMENTS " ( " ELEMENTS_DATE " VARCHAR(20), "
                                                        ELEMENTS_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
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

bool MyDataBase::selectMaterials(QList<QList<QVariant>> &data){
    QSqlQuery query(m_db);
    if(query.exec("SELECT* FROM " TABLE_MATERIALS)){
        while(query.next()){
            QList<QVariant> vect;
            for(int i = 0; i < 6; ++i)
                vect<<query.value(i);
            data<<vect;
        }
        return true;
    }else{
       return false;
    }
}

bool MyDataBase::selectElements(QList<QList<QVariant>> &data){
    QSqlQuery query(m_db);
    if(query.exec("SELECT* FROM " TABLE_ELEMENTS)){
        while(query.next()){
            QList<QVariant> vect;
            for(int i = 0; i < 6; ++i)
                vect<<query.value(i);
            data<<vect;
        }
        return true;
    }else{
       return false;
    }
}


bool MyDataBase::insertIntoTable(const QList<QVariant> &data){
    QSqlQuery query(m_db);
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
    if(query.exec())
        return true;
    else
        return false;
}

bool MyDataBase::insertIntoElements(const QList<QVariant> &data){
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO " TABLE_ELEMENTS " ( " ELEMENTS_DATE ", "
                                                       ELEMENTS_NAME    ", "
                                                       ELEMENTS_PRICE   ", "
                                                       ELEMENTS_OUTPUT   ", "
                                                       ELEMENTS_PORTION  ") "
                              "VALUES (:Date, :Name, :Price, :OutPut, :Portion)"
                    );
    query.bindValue(":Date", data.value(0).toLongLong());
    query.bindValue(":Name", data.value(2).toLongLong());
    query.bindValue(":Price", data.value(3).toString());
    query.bindValue(":OutPut", data.value(4).toString());
    query.bindValue(":Portion", data.value(5).toDouble());
    if(query.exec())
        return true;
    else
        return false;
}

qlonglong MyDataBase::getLastNumNumber()const{
    QSqlQuery query(m_db);
    if(query.exec("SELECT count(*) FROM " TABLE_MATERIALS)){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}

qlonglong MyDataBase::getLastFolderNumber() const{
    QSqlQuery query(m_db);
    if(query.exec("SELECT max( " MATERIALS_DIR  " ) FROM " TABLE_MATERIALS)){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}

bool MyDataBase::updateTableItem(const QList<QVariant> &data){
    QSqlQuery query(m_db);
    query.prepare("UPDATE " TABLE_MATERIALS " SET "
                                    MATERIALS_NAME  " = :Name, "
                                    MATERIALS_UNIT  " = :Unit, "
                                    MATERIALS_PRICE " = :Price "
                    "WHERE " MATERIALS_ID " = :id");
    query.bindValue(":Name", data.value(3));
    query.bindValue(":Unit", data.value(4));
    query.bindValue(":Price", data.value(5));
    query.bindValue(":id", data.value(0));
    if(query.exec())
        return true;
    else
        return false;
}

bool MyDataBase::updateTableElements(const QList<QVariant> &data){
    QSqlQuery query(m_db);
    query.prepare("UPDATE " TABLE_MATERIALS " SET "
                                    MATERIALS_NAME  " = :Name, "
                                    MATERIALS_UNIT  " = :Unit, "
                                    MATERIALS_PRICE " = :Price "
                    "WHERE " MATERIALS_ID " = :id");
    query.bindValue(":Name", data.value(3));
    query.bindValue(":Unit", data.value(4));
    query.bindValue(":Price", data.value(5));
    query.bindValue(":id", data.value(0));
    if(query.exec())
        return true;
    else
        return false;
}

