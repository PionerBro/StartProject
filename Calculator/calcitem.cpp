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

CalcItem::CalcItem(QWidget* parent, Qt::WindowFlags f):QDialog(parent,f), tableRows(0)
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
    connect(btnAddRow, SIGNAL(clicked()), this, SLOT(addRow()));

    QHBoxLayout* hbx32 = new QHBoxLayout;
    hbx3->addLayout(hbx32);
    hbx3->setAlignment(hbx32, Qt::AlignRight);
    sumLabel = new QLabel("sad",this);
    hbx32->addWidget(sumLabel, 0, Qt::AlignRight);
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
    connect(table, SIGNAL(cellChanged(int,int)), this, SLOT(dataChanged(int,int)));
}

CalcItem::~CalcItem(){
    qDebug()<<"CalcItem destroyed";
}

/*add row in table slot*/
void CalcItem::addRow(){
    /*create and set cell widget*/
    CellWidget* cellWidget = new CellWidget(table);
    connect(cellWidget->buttonWidget(), SIGNAL(clicked()), this, SLOT(chooseElement()));
    cellWidget->setRow(tableRows);
    cellWidget->setColumn(0);

    table->insertRow(tableRows);                             //insert row to the end
    table->setCellWidget(tableRows,0, cellWidget);           //set cell widget in position
    table->setRowHeight(tableRows,25);
    cellWidget->setText(QString("%1").arg(tableRows));

    ++tableRows;                                             //increment table rows
    emit cellWidget->buttonWidget()->clicked();
}


/*Cell Widget's button clicked slot*/
void CalcItem::chooseElement(){
    QPushButton* sender = static_cast<QPushButton*>(QObject::sender());         //get button object, which clicked
    setCurRow(static_cast<CellWidget*>(sender->parent())->row());               //set current row variable
    DirectoryWidget* dirWidget = new DirectoryWidget(sender);                   //create numenclature Dialog widget
    connect(dirWidget->getModel(), SIGNAL(sendData(QList<QVariant>&)), this, SLOT(receiveData(QList<QVariant>&))); //when element is chosen emit signal with element's data

    if(dirWidget->exec()){
        qDebug()<<"good";
    }else{
        qDebug()<<"bad";

    }
}

/*fill the cell data slot emit by nomenclature widget signal*/
void CalcItem::receiveData(QList<QVariant>& data){
    /*create and set cell widget*/
    CellWidget* cellWidget = new CellWidget(table);
    connect(cellWidget->buttonWidget(), SIGNAL(clicked()), this, SLOT(chooseElement()));
    cellWidget->setRow(currRow());
    cellWidget->setColumn(0);
    table->setCellWidget(currRow(), 0, cellWidget);         //set cell widget in current position
    cellWidget->setText(data.value(1).toString());

    QTableWidgetItem* item = new QTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setData(Qt::DisplayRole, data.value(2));
    table->setItem(currRow(),1,item);

    item = new QTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setData(Qt::DisplayRole, data.value(0));
    table->setItem(currRow(),3,item);

    //item->setData(Qt::EditRole, data.value(0));
    //qDebug()<<item->text();

    qDebug()<<data<<" "<<table->currentRow();
}

int CalcItem::currRow() const{
    return m_currentRow;
}

void CalcItem::setCurRow(int row){
    m_currentRow = row;
}

void CalcItem::dataChanged(int row, int column){
    if(column==2 || column == 3){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        if(table->item(row,2) && table->item(row,3)){
            item->setData(Qt::DisplayRole, table->item(row,2)->data(Qt::DisplayRole).toInt() * table->item(row,3)->data(Qt::DisplayRole).toInt());
        }
        table->setItem(row, 4, item);
        int sum = 0;
        for(int i = 0; i < tableRows; ++i){
            if(table->item(i,4)){
                sum+=table->item(i,4)->data(Qt::DisplayRole).toInt();
            }
        }
        sumLabel->setText(QString::number(sum));
    }
}
