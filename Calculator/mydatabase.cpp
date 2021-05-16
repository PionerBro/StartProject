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

bool MyDataBase::select(QList<QList<QVariant>> &data){
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

qlonglong MyDataBase::getLastNumNumber(){
    QSqlQuery query(m_db);
    if(query.exec("SELECT count(*) FROM " TABLE_MATERIALS)){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}
