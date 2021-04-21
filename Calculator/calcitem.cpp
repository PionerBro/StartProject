#include "calcitem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHeaderView>
#include <QDateEdit>


#include <QDebug>

CalcItem::CalcItem(QWidget* parent, Qt::WindowFlags f):QDialog(parent,f)
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
    QLabel* dateLabel = new QLabel(tr("Количество порций"), this);
    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(),this);
    dateEdit->setMinimumSize(80,25);
    dateEdit->setCalendarPopup(true);
    hbx22->addWidget(dateLabel);
    hbx22->addWidget(dateEdit);

    QTableWidget* table = new QTableWidget(20,5,this);
    QStringList headers;
    headers<<tr("Наименование")
             <<tr("Ед.изм")
             <<tr("Кол-во")
             <<tr("Цена")
             <<tr("Сумма");
    table->setHorizontalHeaderLabels(headers);
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

}

CalcItem::~CalcItem(){
    qDebug()<<"CalcItem destroyed";
}
