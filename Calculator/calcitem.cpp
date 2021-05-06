#include "calcitem.h"
#include "directorywidget.h"
#include "mytreemodel.h"
#include "cellwidget.h"
#include "calctablewidget.h"
#include "calcitem2delegate.h"
#include "printwidget.h"
#include "mydoublevalidator.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHeaderView>
#include <QDateEdit>
#include <QDialog>
#include <QAction>

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
    nameEdit = new QLineEdit(this);
    nameEdit->setText("");
    hbx1->addWidget(nameLabel);
    hbx1->addWidget(nameEdit);


    QHBoxLayout* hbx2 = new QHBoxLayout;
    vbx->addLayout(hbx2);
    QHBoxLayout* hbx21 = new QHBoxLayout;
    hbx2->addLayout(hbx21);
    hbx2->setAlignment(hbx21, Qt::AlignLeft);
    QLabel* portionsLabel = new QLabel(tr("Выход, кг.: "), this);
    portionEdit = new QLineEdit(this);
    portionEdit->setText("");
    portionEdit->setMaximumSize(60, 25);
    MyDoubleValidator* validator = new MyDoubleValidator(0,10000,3,this);
    validator->setLocale(QLocale::C);
    portionEdit->setValidator(validator);
    hbx21->addWidget(portionsLabel,0,Qt::AlignLeft);
    hbx21->addWidget(portionEdit,0,Qt::AlignLeft);
    hbx21->addSpacing(100);
    connect(portionEdit, SIGNAL(textChanged(QString)), this, SLOT(priceSlot()));

    QHBoxLayout* hbx22 = new QHBoxLayout;
    hbx2->addLayout(hbx22);
    hbx2->setAlignment(hbx22, Qt::AlignRight);
    QLabel* dateLabel = new QLabel(tr("Дата"), this);
    dateEdit = new QDateEdit(QDate::currentDate(),this);
    dateEdit->setFixedSize(80,20);
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
    sumLabel = new QLabel("",this);
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
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    CalcItem2Delegate* delegate = new CalcItem2Delegate(table);
    table->setItemDelegateForColumn(2, delegate);
    table->setItemDelegateForColumn(3, delegate);
    table->setItemDelegateForColumn(4, delegate);
    table->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    connect(btnAddRow, SIGNAL(clicked()), table, SLOT(addNewRow()));
    connect(table, SIGNAL(cellChanged(int,int)), this, SLOT(dataChanged(int,int)));
    vbx->addWidget(table);
    QHBoxLayout* hbxSize = new QHBoxLayout;
    vbx->addLayout(hbxSize);
    vbx->setAlignment(hbxSize, Qt::AlignRight);
    QLabel* sizeLabel = new QLabel(tr("Порция, г.: "),this);
    sizeEdit = new QLineEdit("",this);
    sizeEdit->setMaximumSize(100, 25);
    sizeEdit->setValidator(new MyDoubleValidator(0,10000,3,sizeEdit));
    QAction* sizeEd = new QAction(sizeEdit);
    connect(sizeEd, SIGNAL(triggered()), sizeEdit, SIGNAL(editingFinished()));
    sizeEd->setShortcut(Qt::Key_Enter);
    hbxSize->addWidget(sizeLabel, Qt::AlignRight);
    hbxSize->setAlignment(sizeLabel, Qt::AlignRight);
    hbxSize->addWidget(sizeEdit, Qt::AlignRight);
    connect(sizeEdit, SIGNAL(textChanged(QString)), this, SLOT(priceSlot()));
    QLabel* textPriceLabel = new QLabel(tr("Цена: "),this);
    priceLabel = new QLabel("0.00", this);
    priceLabel->setMinimumSize(60,20);
    priceLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    hbxSize->addWidget(textPriceLabel);
    hbxSize->addWidget(priceLabel);

    QHBoxLayout* hbxBtn = new QHBoxLayout;
    vbx->addLayout(hbxBtn);
    vbx->setAlignment(hbxBtn, Qt::AlignHCenter);
    QHBoxLayout* hbxBtn1 = new QHBoxLayout;
    QHBoxLayout* hbxBtn2 = new QHBoxLayout;
    QHBoxLayout* hbxBtn3 = new QHBoxLayout;
    hbxBtn->addLayout(hbxBtn1);
    hbxBtn->addLayout(hbxBtn2);
    hbxBtn->addLayout(hbxBtn3);
    hbxBtn->setAlignment(hbxBtn2, Qt::AlignHCenter);
    hbxBtn->setAlignment(hbxBtn3, Qt::AlignRight);
    QPushButton* btnOk = new QPushButton(tr("Ок"),this);
    btnOk->setMaximumSize(100,30);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    hbxBtn2->addWidget(btnOk);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    btnCancel->setMaximumSize(100,30);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    hbxBtn2->addWidget(btnCancel);
    btnOk->setFocusPolicy(Qt::NoFocus);
    btnCancel->setFocusPolicy(Qt::NoFocus);
    btnAddRow->setFocusPolicy(Qt::NoFocus);
    QPushButton* btnPrint = new QPushButton(tr("Печать"), this);
    btnPrint->setMaximumSize(70,30);
    hbxBtn3->addWidget(btnPrint, 0, Qt::AlignRight);
    btnPrint->setFocusPolicy(Qt::NoFocus);
    connect(btnPrint, SIGNAL(clicked()), this, SLOT(printSlot()));

}

CalcItem::~CalcItem(){
    qDebug()<<"CalcItem destroyed";
}

void CalcItem::dataChanged(int row, int column){
    if(column == 2 || column == 3){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        if(table->item(row,2) && table->item(row,3)){           
            item->setData(Qt::DisplayRole, QString::number((table->item(row,2)->data(Qt::DisplayRole).toDouble() * table->item(row,3)->data(Qt::DisplayRole).toDouble()),'f',2));
        }
        table->setItem(row, 4, item);
        double sum = 0;
        for(int i = 0; i < table->rowCount(); ++i){
            if(table->item(i,4)){
                sum+=table->item(i,4)->data(Qt::DisplayRole).toDouble();
            }
        }

        sumLabel->setText(QString::number(sum, 'f', 2));
        emit portionEdit->textChanged("");
    }
    if(row == -1){
        double sum = 0;
        for(int i = 0; i < table->rowCount(); ++i){
            if(table->item(i,4)){
                sum+=table->item(i,4)->data(Qt::DisplayRole).toDouble();
            }
        }

        sumLabel->setText(QString::number(sum, 'f', 2));
        emit portionEdit->textChanged("");
    }
}

void CalcItem::priceSlot(){
    bool ok;
    double portion = portionEdit->text().toDouble(&ok);
    if(ok && portion){
        priceLabel->setText(QString::number(sumLabel->text().toDouble()*sizeEdit->text().toDouble()/portion/1000, 'f',2));
    }else{
        priceLabel->setText("0.00");
    }
}

void CalcItem::printSlot(){
    /*QPrinter printer;
    QPrintDialog print(&printer, this);
    if(print.exec()){
        QPainter painter;
        if(!painter.begin(&printer)){
            qDebug()<<"error";
        }
        painter.setFont(QFont("Airal", 14)); //шрифт
        painter.setPen(QPen(2)); // толщина линий таблицы
        int y_pdf = 30;
        int WtabPdf=350;
        int X_oneTab = 20;
        int X_twoTab = 370;
        int H_tab = 25;
        painter.drawText(230,y_pdf, "Протокол № " + table->model()->data(table->indexFromItem(table->item(0, 2))).toString());
        y_pdf +=H_tab;
        QRect rectfont = QRect(X_oneTab,y_pdf,WtabPdf,H_tab);
        painter.drawRect(rectfont);
        painter.drawText(rectfont, Qt::AlignLeft ,"Заводской номер");
        rectfont.setRect(X_twoTab,y_pdf,WtabPdf,H_tab);
        painter.drawRect(rectfont);
        painter.drawText(rectfont, Qt::AlignLeft ,table->model()->data(table->indexFromItem(table->item(0, 2))).toString());
                // и так далее рисуем
        painter.end(); // завершаем рисование
    }
    */
    QList<QStringList> data;
    QStringList list;
    list<<nameEdit->text()<<portionEdit->text()<<dateEdit->date().toString("dd.MM.yyyy")<<sumLabel->text()<<sizeEdit->text()<<priceLabel->text();
    data<<list;
    for(int i = 0; i<table->rowCount(); ++i){
        QStringList row;
        row<<static_cast<CellWidget*>(table->cellWidget(i, 0))->text()
           <<table->item(i,1)->data(Qt::DisplayRole).toString()
           <<table->item(i,2)->data(Qt::DisplayRole).toString()
           <<table->item(i,3)->data(Qt::DisplayRole).toString()
           <<table->item(i,4)->data(Qt::DisplayRole).toString();
        data<<row;
    }
    qDebug()<<data;
    PrintWidget* print = new PrintWidget(data, this);
    print->exec();
    qDebug()<<"hello";
}
