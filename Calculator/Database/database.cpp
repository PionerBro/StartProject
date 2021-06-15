#include "database.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QFile>

#include <QDebug>

Database::Database():QSqlDatabase()
{

}

Database::Database(const QSqlDatabase& other):QSqlDatabase(other)
{

}

Database::~Database(){
    delete materialsTreeFolders.value(0);
    qDeleteAll(unitsElements);
}

bool Database::openDatabase(){
    setDatabaseName(DB_NAME);
    setHostName(DB_HOSTAME);
    if(!QFile(DB_NAME).exists()){
        if(!createDatabase())
            return false;
    }else{
        if(!open()){
            checkError(lastError());
            return false;
        }
    }
    if(!setForeignKey())
        return false;
    if(!selectMaterialsTreeItems() && !selectUnitsItems())
        return false;
    return true;
}

bool Database::createDatabase(){
    if(!open()){
        checkError(lastError());
        return false;
    }

    if(!setForeignKey())
        return false;

    transaction();
    if(!createTable(TABLE_MATERIALS_TREE)){
        rollback();
        return false;
    }
    if(!createTable(TABLE_MATERIALS_PRICES)){
        rollback();
        return false;
    }
    if(!createTable(TABLE_UNITS)){
        rollback();
        return false;
    }

    if(!createIndex(TABLE_MATERIALS_TREE, MAT_TREE_ID)){
        rollback();
        return false;
    }
    commit();
    return true;
}

bool Database::createTable(const QString &table){
    QSqlQuery query(*this);
    if(table == TABLE_MATERIALS_TREE){
        query.prepare("CREATE TABLE " TABLE_MATERIALS_TREE " ( "
                                                MAT_TREE_ID     " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                MAT_TREE_PARENT " INTEGER, "
                                                MAT_TREE_DIR    " INTEGER, "
                                                MAT_TREE_NAME   " VARCHAR(50), "
                                                MAT_TREE_UNIT   " VARCHAR(20), "
                                                MAT_TREE_PRICE  " DOUBLE, "
                                                "FOREIGN KEY(" MAT_TREE_UNIT ") REFERENCES " TABLE_UNITS "(" UNITS_ID "))"
                );
    }else if(table == TABLE_MATERIALS_PRICES){
        query.prepare("CREATE TABLE " TABLE_MATERIALS_PRICES " ( "
                                                MAT_PRS_DOC     " INTEGER, "
                                                MAT_PRS_MAT_ID  " INTEGER, "
                                                MAT_PRS_DATE    " VARCHAR(20), "
                                                MAT_PRS_PRICE   " DOUBLE, "
                                                "FOREIGN KEY(" MAT_PRS_MAT_ID  ") REFERENCES " TABLE_MATERIALS_TREE "(" MAT_TREE_ID "))"
                );
    }else if(table == TABLE_UNITS){
        query.prepare("CREATE TABLE " TABLE_UNITS " ( "
                                                UNITS_ID     " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                UNITS_NAME  " VARCHAR(20))"
                );
    }
    if(!query.exec()){
        checkError(lastError());
        return false;
    }
    return true;
}

bool Database::createIndex(const QString &table, const QString &field){
    QSqlQuery query(*this);
    query.prepare("CREATE INDEX MAT_TREE_ID_IDX ON " + table + "(" + field + ")");
    if(!query.exec()){
        checkError(lastError());
        return false;
    }
    qDebug()<<"d";
    return true;
}

void Database::checkError(const QSqlError& error){
    QString errorText;
    switch (error.type()) {
    case QSqlError::NoError:
        errorText = "No error occurred! ";
        break;
    case QSqlError::ConnectionError:
        errorText = "Connection error! ";
        break;
    case QSqlError::StatementError:
        errorText = "SQL statement syntax error! ";
        break;
    case QSqlError::TransactionError:
        errorText = "Transaction failed error! ";
        break;
    case QSqlError::UnknownError:
        errorText = "UnknownError! ";
        break;
    }
    errorText += error.text();
    QMessageBox::information(0, "Database", errorText);
}

bool Database::setForeignKey(){
    QSqlQuery query(*this);
    query.prepare("PRAGMA foreign_keys = ON;");
    if(!query.exec()){
        checkError(lastError());
        return false;
    }
    return true;
}

bool Database::selectMaterialsTreeItems(){
    QSqlQuery query(*this);
    query.prepare("SELECT " TABLE_MATERIALS_TREE "." MAT_TREE_ID ", "
                            MAT_TREE_PARENT ", "
                            MAT_TREE_DIR    ", "
                            TABLE_MATERIALS_TREE "." MAT_TREE_NAME   ", "
                            TABLE_UNITS "." UNITS_NAME ", "
                            MAT_TREE_PRICE  " FROM " TABLE_MATERIALS_TREE ", " TABLE_UNITS
                            " WHERE " TABLE_UNITS "." UNITS_ID " = " MAT_TREE_UNIT
                  );

    if(!query.exec()){
        checkError(lastError());
        return false;
    }

    TreeItem* item = new TreeItem({0,0,0});
    materialsTreeFolders<<item;
    while(query.next()){
        QVector<QVariant> iData;
        for(int i = 0; i < 6; ++i){
            iData<<query.value(i);
        }
        int folderNum = iData.value(FieldName::ParentId).toLongLong();
        TreeItem* item = new TreeItem(iData, materialsTreeFolders[folderNum]);
        materialsTreeFolders[folderNum]->appendChild(item);
        (item->getItemType()) ? materialsTreeFolders<<item : materialsTreeElements<<item;
    }
    for(int i = 0; i < materialsTreeFolders.count(); ++i){
        materialsTreeFolders.value(i)->sortItem();
    }
    std::sort(materialsTreeElements.begin(), materialsTreeElements.end(), compare);

    return true;
}

bool Database::selectUnitsItems(){
    QSqlQuery query(*this);
    query.prepare("SELECT* FROM " TABLE_UNITS);
    if(!query.exec()){
        checkError(lastError());
        return false;
    }
    TreeItem* item = new TreeItem({0, QVariant()});
    unitsElements<<item;
    while(query.next()){
        QVector<QVariant> vect;
        for(int i = 0; i < 2; ++i){
            vect<<query.value(i);
        }
        item = new TreeItem(vect);
        unitsElements<<item;
    }
    return true;
}

qlonglong Database::insertMaterialsTreeItem(const QVector<QVariant> &data){
    QSqlQuery query(*this);
    qlonglong unitId = findUnitsId(data.value(4).toString());
    transaction();
    /*query.prepare("SELECT " UNITS_ID " FROM " TABLE_UNITS " WHERE " UNITS_NAME " = :Name");
    query.bindValue(":Name", data.value(4));
    if(!query.exec()){
        checkError(lastError());
        rollback();
        return -1;
    }
    int unitId = -1;
    while(query.next()){
        unitId = query.value(0).toInt();
    }
    query.clear();
    */
    query.prepare("INSERT INTO " TABLE_MATERIALS_TREE " ( "
                                        MAT_TREE_PARENT ", "
                                        MAT_TREE_DIR    ", "
                                        MAT_TREE_NAME   ", "
                                        MAT_TREE_UNIT   ", "
                                        MAT_TREE_PRICE  ") "
                                        "VALUES(:Parent, :Folder, :Name, :Unit, :Price)"
                 );
    query.bindValue(":Parent", data.value(1));
    query.bindValue(":Folder", data.value(2));
    query.bindValue(":Name", data.value(3));
    query.bindValue(":Unit", unitId);
    query.bindValue(":Price", data.value(5));
    if(!query.exec()){
        checkError(lastError());
        rollback();
        return -1;
    }
    query.clear();
    query.prepare("SELECT count(*) FROM " TABLE_MATERIALS_TREE);
    if(!query.exec()){
        checkError(lastError());
        rollback();
        return -1;
    }
    commit();
    while(query.next()){
        return query.value(0).toLongLong();
    }
    return -1;
}

qlonglong Database::findUnitsId(const QString& name)const{
    for(qlonglong i = 0; i < unitsElements.count(); ++i){
        if(unitsElements.value(i)->data(1).toString() == name)
            return i;
    }
    return 0;
}
