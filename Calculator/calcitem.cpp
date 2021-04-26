#include "calcitem.h"
#include "directorywidget.h"
#include "mytreemodel.h"
#include "cellwidget.h"
#include "calctablewidget.h"

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
    QLabel* dateLabel = new QLabel(tr("Дата"), this);
    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(),this);
    dateEdit->setMinimumSize(80,25);
    dateEdit->setCalendarPopup(true);
    hbx22->addWidget(dateLabel);
    hbx22->addWidget(dateEdit);

    QHBoxLayout* hbx3 = new QHBoxLayout;
    QHBoxLayout* hbx31 = new QHBoxLayout;
    vbx->addLayout(hbx3);
    hbx3->addLayout(hbx31);
    hbx3->setAlignment(hbx31, Qt::AlignLeft);
    QPushButton* btnAddRow = new QPushButton(tr("Добавить строку"),this);
    btnAddRow->setMaximumSize(100,30);
    btnAddRow->setShortcut(Qt::Key_Insert);
    hbx31->addWidget(btnAddRow);

    QHBoxLayout* hbx32 = new QHBoxLayout;
    hbx3->addLayout(hbx32);
    hbx3->setAlignment(hbx32, Qt::AlignRight);
    sumLabel = new QLabel("sad",this);
    hbx32->addWidget(sumLabel, 0, Qt::AlignRight);
    table = new CalcTableWidget(0,5,this);
    QStringList headers;
    headers<<tr("Наименование")
             <<tr("Ед.изм")
             <<tr("Кол-во")
             <<tr("Цена")
             <<tr("Сумма");
    table->setHorizontalHeaderLabels(headers);
    table->setColumnWidth(0,300);
    table->horizontalHeader()->setStretchLastSection(true);
    connect(btnAddRow, SIGNAL(clicked()), table, SLOT(addNewRow()));
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
    connect(table, SIGNAL(cellChanged(int,int)), this, SLOT(dataChanged(int,int)));

}

CalcItem::~CalcItem(){
    qDebug()<<"CalcItem destroyed";
}

void CalcItem::dataChanged(int row, int column){
    if(column==2 || column == 3){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        if(table->item(row,2) && table->item(row,3)){
            item->setData(Qt::DisplayRole, table->item(row,2)->data(Qt::DisplayRole).toDouble() * table->item(row,3)->data(Qt::DisplayRole).toDouble());
        }
        table->setItem(row, 4, item);
        double sum = 0;
        for(int i = 0; i < table->rowCount(); ++i){
            if(table->item(i,4)){
                sum+=table->item(i,4)->data(Qt::DisplayRole).toDouble();
            }
        }

        sumLabel->setText(QString::number(sum, 'f', 2));
    }
}
