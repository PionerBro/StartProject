#include "database.h"
#include <QFile>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include <QDebug>

Database::Database():QSqlDatabase()
{

}

Database::Database(const QSqlDatabase& other):QSqlDatabase(other)
{

}

Database::~Database(){
    qDeleteAll(m_sqlTables);
}

bool Database::openDatabase(){
    if(!QFile(DB_NAME).exists()){
        QMessageBox::information(0, "Error", "Базы данных не существует!");
        return false;
    }
    *this = QSqlDatabase::addDatabase("QSQLITE", "Pioner");
    this->setDatabaseName(DB_NAME);
    this->setHostName(DB_HOSTNAME);
    if(!open()){
        QMessageBox::information(0, "Error", "Не Удалось открыть базу данных: " + lastError().text());
        return false;
    }
    if(!tables().contains(TABLE_DBTABLES) || !selectTablesInformation() || !selectTables()){
        QMessageBox::information(0, "Error", "База данных повреждена");
        return false;
    }

    for(int i = 0; i < m_sqlTablesNames.count(); i++){
        qDebug()<<m_sqlTablesNames.value(i)<<m_sqlTables.value(m_sqlTablesNames.value(i))->code<<m_sqlTables.value(m_sqlTablesNames.value(i))->type<<m_sqlTables.value(m_sqlTablesNames.value(i))->fields;
        for(int j = 0; j < m_sqlTables.value(m_sqlTablesNames.value(i))->fields.count(); ++j){
            qDebug()<<m_sqlTables.value(m_sqlTablesNames.value(i))->fields.value(j)->name
                    <<m_sqlTables.value(m_sqlTablesNames.value(i))->fields.value(j)->type
                    <<m_sqlTables.value(m_sqlTablesNames.value(i))->fields.value(j)->length
                    <<m_sqlTables.value(m_sqlTablesNames.value(i))->fields.value(j)->precision;
        }
    }

    return true;
}

/*  Получаем информацию о таблицах базы
 *  Информация хранится в дереве m_sqlTables
**/

bool Database::selectTablesInformation(){
    QSqlQuery query(*this);
    if(!query.exec("SELECT* FROM " TABLE_DBTABLES)){
        return false;
    }
    while(query.next()){
        m_sqlTables.insert(query.value(0).toString(), new TableInformation(query.value(1).toInt(),query.value(2).toInt()));
    }
    m_sqlTablesNames = m_sqlTables.keys();
    return true;
}

/*
 *
**/

bool Database::selectTables(){
    for(int i = 0; i < m_sqlTablesNames.count(); ++i){
        QString table = m_sqlTablesNames.value(i);
        if(!selectTable(table, m_sqlTables.value(table))){
            QMessageBox::information(0, "Error", "Ошибка выборки данных " + lastError().text());
            return false;
        }
    }
    return true;
}

/*
 *
**/

bool Database::selectTable(const QString &table, TableInformation* info){
    QSqlQuery query(*this);
    if(!query.exec("SELECT* FROM " + table)){
        return false;
    }
    while(query.next()){
        info->fields.append(new TableInformation::TableField(query.value(0).toString(), query.value(1).toString(), query.value(2).toInt(), query.value(3).toInt()));
    }
    return true;
}
