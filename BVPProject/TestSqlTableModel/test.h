#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include "database.h"

class Test : public QWidget
{
    Q_OBJECT
public:
    explicit Test(QWidget *parent = nullptr);
private:
    QSqlTableModel* model;
    QTableView* view;
    DataBase* m_db;
    int m_filter;
private:
    void setupModel(const QStringList& headers);
    void createView();
private slots:
    void rowDoubleClicked(const QModelIndex &index);
signals:

};

#endif // TEST_H
