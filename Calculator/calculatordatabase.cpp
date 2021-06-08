#include "calculatordatabase.h"
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <QList>
#include <QVector>
#include <QSqlRecord>
#include <QSqlField>

#include <QDebug>

CalculatorDatabase::CalculatorDatabase(QObject *parent) : QObject(parent)
{

}

CalculatorDatabase::~CalculatorDatabase(){
    closeDatabase();
}

bool CalculatorDatabase::createConnection(){
    if(!QFile(CALCULATOR_DATABASE_NAME).exists()){
        QMessageBox::information(0, "Error Database", tr("Базы данных не существует"));
        return false;
    }else{
        return openDatabase();
    }
}

bool CalculatorDatabase::openDatabase(){
    m_db = QSqlDatabase::addDatabase("QSQLITE", "CalculatorDB");
    m_db.setHostName(CALCULATOR_DATABASE_HOST);
    m_db.setDatabaseName(CALCULATOR_DATABASE_NAME);
    if(m_db.open()){
        return true;
    }else{
        QMessageBox::information(0, "Error Database", tr("Не удалось открыть базу данных"));
        return false;
    }
}

void CalculatorDatabase::closeDatabase(){
    m_db.close();
}

bool CalculatorDatabase::createTable(const QString& tableName){
    QSqlQuery query(m_db);
    if(tableName == TABLE_MATERIALS){
        query.prepare("CREATE TABLE " TABLE_MATERIALS " ( " MATERIALS_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                        MATERIALS_PARENT " INTEGER, "
                                                        MATERIALS_DIR " INTEGER, "
                                                        MATERIALS_NAME " VARCHAR(50), "
                                                        MATERIALS_UNIT " VARCHAR(20), "
                                                        MATERIALS_PRICE " DOUBLE)"
                  );
    }else if(tableName == TABLE_MATERIALS_PRICES){
        query.prepare("CREATE TABLE " TABLE_MATERIALS_PRICES " ( "  MATERIALS_PRICES_DOC   " INTEGER , "
                                                     MATERIALS_PRICES_ID    " INTEGER, "
                                                     MATERIALS_PRICES_DATE  " VARCHAR(10), "
                                                     MATERIALS_PRICES_PRICE " DOUBLE)"
                  );
    }else if(tableName == TABLE_ELEMENTS_TREE){
        query.prepare("CREATE TABLE " TABLE_ELEMENTS_TREE " ( " ELEMENTS_TREE_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                                ELEMENTS_TREE_PARENT " INTEGER, "
                                                                ELEMENTS_TREE_DIR    " INTEGER, "
                                                                ELEMENTS_TREE_NAME " VARCHAR(50), "
                                                                ELEMENTS_TREE_PRICE " DOUBLE, "
                                                                ELEMENTS_TREE_OUTPUT " DOUBLE, "
                                                                ELEMENTS_TREE_PORTION " DOUBLE)"
                  );
    }else if(tableName == TABLE_ITEMS){
        query.prepare("CREATE TABLE " TABLE_ITEMS " ( " ITEMS_DOC   " INTEGER, "
                                                        ITEMS_EL_NUM   " INTEGER, "
                                                        ITEMS_MAT_NUM " INTEGER, "
                                                        ITEMS_MAT_NAME " VARCHAR(50), "
                                                        ITEMS_COUNT " DOUBLE, "
                                                        ITEMS_PRICE " DOUBLE)"
                  );
    }else if(tableName == TABLE_ELEMENTS){
        query.prepare("CREATE TABLE " TABLE_ELEMENTS " ( "  TABLE_ELEMENTS_DOC " INTEGER, "
                                                        TABLE_ELEMENTS_DATE " VARCHAR(10), "
                                                        TABLE_ELEMENTS_NUM    " INTEGER, "
                                                        TABLE_ELEMENTS_NAME " VARCHAR(50), "
                                                        TABLE_ELEMENTS_PRICE " DOUBLE, "
                                                        TABLE_ELEMENTS_OUTPUT " DOUBLE, "
                                                        TABLE_ELEMENTS_PORTION " DOUBLE)"

                );
    }else if(tableName == TABLE_UNITS){
        query.prepare("CREATE TABLE " TABLE_UNITS " ( " TABLE_UNITS_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                        TABLE_UNITS_NAME " VARCHAR(20))"
                );
    }else{
        return false;
    }
    if(query.exec()){
        return true;
    }else{
        QMessageBox::information(0,"Error Database", tr("Не удалось создать таблицу данных"));
        return false;
    }
}

bool CalculatorDatabase::select(const QString& table, QVector<QVector<QVariant>> &data){
    QSqlQuery query(m_db);
    if(query.exec("SELECT* FROM " + table)){
        while(query.next()){
            QVector<QVariant> vect;
            for(int i = 0; i < (query.record()).count(); ++i){
                vect<<query.value(i);
            }
            data<<vect;
        }
        return true;
    }else{
       return false;
    }
}

bool CalculatorDatabase::insertIntoTable(const QString& table, const QVector<QVariant> &data){
    QSqlQuery query(m_db);
    int startField = (table == TABLE_MATERIALS || table == TABLE_ELEMENTS_TREE || table == TABLE_UNITS)? 1 : 0;
    QSqlRecord rec = m_db.record(table);
    int fieldCount = rec.count();
    QString prepStm1 = "INSERT INTO " + table + "( ";
    QString prepStm2 = "VALUES (";
    QVector<QString> bindingData;
    for(int i = startField; i < fieldCount; ++i){
        QString field = rec.fieldName(i);
        prepStm1+=field;
        prepStm2+=":"+field;
        if(i != fieldCount-1){
            prepStm1+=", ";
            prepStm2+=", ";
        }else{
            prepStm1+=") ";
            prepStm2+=")";
        }
        bindingData<<":"+field;
        /*int type = rec.field(startField).type();
        qDebug()<<type;
        if(type == QMetaType::Int || type==QMetaType::LongLong){
            query.bindValue(":"+field, data.value(startField).toLongLong());
        }else if(type==QMetaType::QString){
            query.bindValue(":"+field, data.value(startField).toString());
        }else if(type==QMetaType::Double){
            query.bindValue(":"+field, data.value(startField).toDouble());
        }*/
    }
    query.prepare(prepStm1+prepStm2);
    for(int i = 0; i < bindingData.count(); ++i){
        query.bindValue(bindingData.value(i), data.value(i+startField));
    }
    if(query.exec()){
        return true;
    }else{
        return false;
    }
}

qlonglong CalculatorDatabase::getRowsCount(const QString& table)const{
    QSqlQuery query(m_db);
   if(query.exec("SELECT count(*) FROM " + table)){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}

qlonglong CalculatorDatabase::getLastFolderNumber(const QString& table) const{
    QSqlQuery query(m_db);
    if(table == TABLE_MATERIALS || table == TABLE_ELEMENTS_TREE){
        query.prepare("SELECT max(DirId) FROM " + table);
    }else{
        return -1;
    }
    if(query.exec()){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}

bool CalculatorDatabase::updateTableItem(const QString& table, const QVector<QVariant> &data){
    QSqlQuery query(m_db);
    if(table == TABLE_ELEMENTS_TREE || table == TABLE_MATERIALS || table == TABLE_UNITS){
        QSqlRecord rec = m_db.record(table);
        QString prepStm = "UPDATE " + table + " SET ";
        QString whereStm = "WHERE id = :id";
        QVector<QString> bindingData;
        bindingData<<":id";
        for(int i = 1; i<rec.count(); ++i){
            QString fieldName = rec.fieldName(i);
            if(i != rec.count()-1){
                prepStm += fieldName + " = :" + fieldName + ", ";
            }else{
                prepStm += fieldName + " = :" + fieldName + " ";
            }
            bindingData<<":" + fieldName;
        }
        query.prepare(prepStm + whereStm);
        for(int i = 0; i < bindingData.count(); ++i){
            query.bindValue(bindingData.value(i), data.value(i));
        }
    }else
        return false;
    if(query.exec()){
        return true;
    }
    else
        return false;
}

bool CalculatorDatabase::selectAtNum(qlonglong num, const QString &table, QList<QList<QVariant>> &data){
    QSqlQuery query(m_db);
    QSqlRecord rec = m_db.record(table);
    if(query.exec("SELECT* FROM " + table + " WHERE id = " + QString::number(num))){
        while(query.next()){
            QList<QVariant> vect;
            for(int i = 0; i<rec.count();++i){
                vect<<query.value(i);
            }
            data<<vect;
        }
        return true;
    }else{
        return false;
    }

}

qlonglong CalculatorDatabase::getMaxFieldValue(int field, const QString& table){
    QSqlQuery query(m_db);
    QString fieldName = m_db.record(table).fieldName(field);
    if(fieldName.isEmpty())
        return -1;
    query.prepare("SELECT max(" + fieldName + ") FROM " + table);
    if(query.exec()){
        query.next();
        return query.value(0).toLongLong();
    }else{
        return -1;
    }
}
