#include "calcitem.h"
#include "directorywidget.h"
#include "mytreemodel.h"
#include "cellwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHeaderView>
#include <QDateEdit>


#include <QDebug>

CalcItem::CalcItem(QWidget* parent, Qt::WindowFlags f):QDialog(parent,f), tableRow(0)
{
    setWindowFlag(Qt::WindowMaximizeButtonHint,true);
    resize(800,400);
    setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    vbx->addLayout(hbx1);
    QLabel* nameLabel = new QLabel(tr("Наименование: "), this);
    QLineEdit* nameEdit = new QLineEdit(this);
    hbx1->addWidget(nameLabel);
    hbx1->addWidget(nameEdit);

    QHBoxLayout* hbx2 = new QHBoxLayout;
    vbx->addLayout(hbx2);
    QHBoxLayout* hbx21 = new QHBoxLayout;
    hbx2->addLayout(hbx21);
    hbx2->setAlignment(hbx21, Qt::AlignLeft);
    QLabel* portionsLabel = new QLabel(tr("Количество порций"), this);
    QLineEdit* portionEdit = new QLineEdit(this);
    hbx21->addWidget(portionsLabel,0,Qt::AlignLeft);
    hbx21->addWidget(portionEdit,0,Qt::AlignLeft);
    hbx21->addSpacing(100);

    QHBoxLayout* hbx22 = new QHBoxLayout;
    hbx2->addLayout(hbx22);
    hbx2->setAlignment(hbx22, Qt::AlignRight);
    QLabel* dateLabel = new QLabel(tr("Дата"), this);
    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(),this);
    dateEdit->setMinimumSize(80,25);
    dateEdit->setCalendarPopup(true);
    hbx22->addWidget(dateLabel);
    hbx22->addWidget(dateEdit);

    QHBoxLayout* hbx3 = new QHBoxLayout;
    vbx->addLayout(hbx3);
    vbx->setAlignment(hbx3, Qt::AlignLeft);
    QPushButton* btnAddRow = new QPushButton(tr("Добавить строку"),this);
    btnAddRow->setMaximumSize(100,30);
    hbx3->addWidget(btnAddRow);
    connect(btnAddRow, SIGNAL(clicked()), this, SLOT(addRow()));

    table = new QTableWidget(0,5,this);
    QStringList headers;
    headers<<tr("Наименование")
             <<tr("Ед.изм")
             <<tr("Кол-во")
             <<tr("Цена")
             <<tr("Сумма");
    table->setHorizontalHeaderLabels(headers);
    table->setColumnWidth(0,300);
    table->horizontalHeader()->setStretchLastSection(true);
    vbx->addWidget(table);
    QHBoxLayout* hbxBtn = new QHBoxLayout;
    vbx->addLayout(hbxBtn);
    vbx->setAlignment(hbxBtn, Qt::AlignHCenter);
    QPushButton* btnOk = new QPushButton(tr("Ок"),this);
    btnOk->setMaximumSize(100,30);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    hbxBtn->addWidget(btnOk);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    btnCancel->setMaximumSize(100,30);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    hbxBtn->addWidget(btnCancel);
    btnOk->setFocusPolicy(Qt::NoFocus);
    btnCancel->setFocusPolicy(Qt::NoFocus);
    btnAddRow->setFocusPolicy(Qt::NoFocus);
    table->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    connect(table, SIGNAL(cellActivated(int,int)), table, SIGNAL(cellClicked(int,int)));

}

CalcItem::~CalcItem(){
    qDebug()<<"CalcItem destroyed";
}

void CalcItem::addRow(){
    CellWidget* cellWidget = new CellWidget(table);
    connect(cellWidget->buttonWidget(), SIGNAL(clicked()), this, SLOT(chooseElement()));
    cellWidget->setRow(tableRow);
    cellWidget->setColumn(0);

    table->insertRow(tableRow);
    table->setCellWidget(tableRow,0, cellWidget);
    table->setRowHeight(tableRow,25);
    cellWidget->setText(QString("%1").arg(tableRow));

    ++tableRow;
}

void CalcItem::chooseElement(){
    QPushButton* sender = static_cast<QPushButton*>(QObject::sender());
    setCurRow(static_cast<CellWidget*>(sender->parent())->row());
    DirectoryWidget* dirWidget = new DirectoryWidget(sender);
    connect(dirWidget->getModel(), SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));

    if(dirWidget->exec()){
        qDebug()<<"good";
    }else{
        qDebug()<<"bad";

    }
}

void CalcItem::receiveData(QString data){
    CellWidget* cellWidget = new CellWidget(table);
    connect(cellWidget->buttonWidget(), SIGNAL(clicked()), this, SLOT(chooseElement()));
    cellWidget->setRow(currRow());
    cellWidget->setColumn(0);
    table->setCellWidget(currRow(), 0, cellWidget);
    cellWidget->setText(data);
    qDebug()<<data<<" "<<table->currentRow();
}

int CalcItem::currRow() const{
    return m_currentRow;
}

void CalcItem::setCurRow(int row){
    m_currentRow = row;
}

