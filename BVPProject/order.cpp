#include "order.h"
#include "itemtable.h"
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>

Order::Order(QWidget *parent) : QWidget(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    QLabel* lbl_number = new QLabel(tr("Номер:"), this);
    lbl_number->setMaximumSize(50,25);
    lne_number = new QLineEdit(this);
    lne_number->setMaximumSize(75,25);
    date = new QDateTimeEdit(this);
    date->setDisplayFormat("dd.MM.yyyy");
    date->setCalendarPopup(true);
    date->setMaximumSize(150,25);
    date->setDateTime(QDateTime::currentDateTime());
    QHBoxLayout* hbx_date = new QHBoxLayout;
    hbx_date->setAlignment(Qt::AlignCenter);
    hbx_date->addWidget(lbl_number);
    hbx_date->addWidget(lne_number);
    hbx_date->addWidget(date);

    QLabel* lbl_name = new QLabel(tr("Ф.И.О:"), this);
    lbl_name->setMaximumSize(75,25);
    lbl_name->setAlignment(Qt::AlignRight);
    lne_name = new QLineEdit(this);
    lne_name->setMaximumSize(250,25);
    QHBoxLayout* hbx_name = new QHBoxLayout;
    hbx_name->setAlignment(Qt::AlignLeft);
    hbx_name->addWidget(lbl_name);
    hbx_name->addWidget(lne_name);

    QLabel* lbl_source = new QLabel(tr("Оформил:"), this);
    lbl_source->setMaximumSize(75,25);
    lbl_source->setAlignment(Qt::AlignRight);
    list_source = new QComboBox(this);
    QStringList str_source;
    str_source<<tr("Лельчицы")<<tr("Мозырь")<<tr("Гомель")<<tr("Калинковичи");
    list_source->addItems(str_source);
    list_source->setMaximumSize(250,25);
    QHBoxLayout* hbx_source = new QHBoxLayout;
    hbx_source->setAlignment(Qt::AlignLeft);
    hbx_source->addWidget(lbl_source);
    hbx_source->addWidget(list_source);

    QLabel* lbl_destination = new QLabel(tr("Доставка:"), this);
    lbl_destination->setMaximumSize(75,25);
    lbl_destination->setAlignment(Qt::AlignRight);
    lne_destination = new QLineEdit(this);
    lne_destination->setMaximumSize(250,25);
    QHBoxLayout* hbx_destination = new QHBoxLayout;
    hbx_destination->setAlignment(Qt::AlignLeft);
    hbx_destination->addWidget(lbl_destination);
    hbx_destination->addWidget(lne_destination);

    createTable();


    QHBoxLayout* hbx_table = new QHBoxLayout;
    hbx_table->addWidget(table);

    btn_ok = new QPushButton(tr("Принять"), this);
    connect(btn_ok, SIGNAL(clicked()), SLOT(slotOkClicked()));
    btn_close = new QPushButton(tr("Закрыть"), this);
    connect(btn_close, SIGNAL(clicked()), SLOT(close()));

    QHBoxLayout* hbx_btn = new QHBoxLayout;
    hbx_btn->setAlignment(Qt::AlignCenter);
    hbx_btn->addWidget(btn_ok);
    hbx_btn->addWidget(btn_close);

    QVBoxLayout* vbx = new QVBoxLayout(this);
    vbx->setAlignment(Qt::AlignTop);
    vbx->addLayout(hbx_date);
    vbx->addLayout(hbx_name);
    vbx->addLayout(hbx_source);
    vbx->addLayout(hbx_destination);
    vbx->addLayout(hbx_table);
    vbx->addLayout(hbx_btn);
}

void Order::slotOkClicked(){
    QStringList list;
    for (int i = 0; i < t_row; ++i) {
        sum += table->item(i, 5)->text().toDouble();
    }

    //list << date->date().toString("dd.MM.yy") << lne_number->text() << lne_name->text() << lne_destination->text() << list_source->currentText() << QString::number(sum);
    QMessageBox::information(0,"r", QString().setNum(sum));
    //emit signalNewDoc(list);
    this->close();
}

void Order::createTable(){
    table = new QTableWidget(t_row, t_col, this);
    QStringList table_str;
    table_str << tr("Наименование") << tr("Хар-ки") << tr("Ед.изм.") << tr("Кол-во") << tr("Цена") << tr("Сумма") << tr("Выполнено");
    table->setHorizontalHeaderLabels(table_str);
    table->horizontalHeader()->setStretchLastSection(true);
    QTableWidgetItem* it = 0;
    for(int i = 0; i<t_row; ++i){
        QWidget* tableWgt = new QWidget(this);
        QPushButton* tableBtn = new QPushButton(".", tableWgt);
        connect(tableBtn, SIGNAL(clicked()), this, SLOT(selectName()));
        tableBtn->setFixedSize(10,10);
        QLabel* tableLbl = new QLabel(tableWgt);
        QHBoxLayout* tablehbx = new QHBoxLayout(tableWgt);
        table->setContentsMargins(0,0,0,0);
        tablehbx->addWidget(tableLbl);
        tablehbx->addWidget(tableBtn, Qt::AlignRight);
        table->setCellWidget(i, 0, tableWgt);
        for(int j = 1; j<t_col; ++j){
            it = new QTableWidgetItem;
            it->setText("");
            table->setItem(i,j,it);
        }
    }
}

void Order::selectName(){
    QStringList data;
    ItemTable* itemTable = new ItemTable(data,this);
    if(itemTable->exec()){
        QWidget* tableWgt = new QWidget(this);
        QPushButton* tableBtn = new QPushButton(".", tableWgt);
        connect(tableBtn, SIGNAL(clicked()), this, SLOT(selectName()));
        tableBtn->setFixedSize(10,10);
        QLabel* tableLbl = new QLabel(data[0]+ " " + data[1], tableWgt);
        QHBoxLayout* tablehbx = new QHBoxLayout(tableWgt);
        table->setContentsMargins(0,0,0,0);
        tablehbx->addWidget(tableLbl);
        tablehbx->addWidget(tableBtn, Qt::AlignRight);
        table->setCellWidget(row,0, tableWgt);
        table->item(row, 2)->setText(data[2]);
        table->item(row, 4)->setText(data[3]);
    }
}















