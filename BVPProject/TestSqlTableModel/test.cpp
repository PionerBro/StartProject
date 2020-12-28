#include "test.h"
#include <QHBoxLayout>

Test::Test(QWidget *parent) : QWidget(parent)
{
    m_db = new DataBase(this);
    m_db->createConnection();
    QHBoxLayout* hbx = new QHBoxLayout(this);

    view = new QTableView(this);
    hbx->addWidget(view);



    QStringList list;
    list<<"id"<<"Name"<<"Price";
    this->setupModel(list);
    this->createView();
}

void Test::setupModel(const QStringList& headers){
    model = new QSqlTableModel(this);
    model->setTable(TABLE);
    for(int i = 0; i < model->columnCount(); ++i){
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
    model->setSort(0, Qt::AscendingOrder);
}

void Test::createView(){
    view->setModel(model);

    //view->setColumnHidden(0, true);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    model->setFilter("Price == 0 or id == 0");
    m_filter = 0;
    connect(view, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(rowDoubleClicked(const QModelIndex&)));
    for(int i = 0; i < 10000; ++i)
        m_db->insertIntoTable();
    model->select();

}

void Test::rowDoubleClicked(const QModelIndex &index){
    int row = index.row();
    if(index.sibling(row, 1).data().toString() == "b"){
        int filter = index.sibling(row, 0).data().toInt();
        QString str;
        if(m_filter != filter){
            str = "Price == " + QString::number(filter) + " or id == " + QString::number(filter);
        }else{
            filter = index.sibling(row, 2).data().toInt();
            str = "Price == " + QString::number(filter) + " or id == " + QString::number(filter);
        }
        m_filter = filter;
        model->setFilter(str);
    }
}
