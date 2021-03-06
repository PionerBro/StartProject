#include "calcitem.h"
#include "directorywidget.h"
#include "mytreemodel.h"
#include "cellwidget.h"
#include "calctablewidget.h"
#include "calcitem2delegate.h"
#include "printwidget.h"
#include "mydoublevalidator.h"
#include "myprintwidget.h"
#include "mytreeitem.h"
#include "mydatabase.h"
#include <cellwidget.h>

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

extern MyDataBase db;

CalcItem::CalcItem(MyTreeItem* itemP, int type, QWidget* parent, Qt::WindowFlags f):QDialog(parent,f), parentItem(itemP)
{
    if(type == Element){
        dirNum = 0;
        setupCalcItem();
    }else{
        dirNum = -1;
        setupCalcFolder();
    }
    if(!itemP->data(0).toLongLong() || itemP->data(2).toLongLong()){ //если ItemP это каталог
        num = 0;
        parentNum = parentItem->data(2).toLongLong();                //то создаем новый элемент или каталог где ItemP это parent
    }else{
        num = parentItem->data(0).toLongLong();                      //иначе это уже созданный объект, которы мы изменяем
        parentNum = parentItem->data(1).toLongLong();
        dirNum = parentItem->data(2).toLongLong();
        nameEdit->setText(parentItem->data(4).toString());
        if(type == Element){
            dateEdit->setDate(parentItem->data(3).toDate());
            outputEdit->setText(parentItem->data(6).toString());
            portionEdit->setText(parentItem->data(7).toString());
            for(int i = 8, j = 0; i<parentItem->rowData().count(); i+=2, ++j){
                QList<QList<QVariant>> list;
                if(!db.selectAtNum(parentItem->data(i).toLongLong(), TABLE_MATERIALS, list))
                    close();
                QList<QVariant> listData = list.value(0);
                table->addNewRow();
                table->setRowData(listData);
                table->setDataAtIndex(j, 2, parentItem->data(i+1));
            }
        }
    }
}

CalcItem::~CalcItem(){
    qDebug()<<"CalcItem destroyed";
}

void CalcItem::setupCalcItem(){
    setWindowFlag(Qt::WindowMaximizeButtonHint,true);
    setWindowFlag(Qt::WindowMinimizeButtonHint,true);
    resize(800,400);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(false);
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
    outputEdit = new QLineEdit(this);
    outputEdit->setText("");
    outputEdit->setMaximumSize(60, 25);
    MyDoubleValidator* validator = new MyDoubleValidator(0,10000,3,outputEdit,MyDoubleValidator::DigitalPoint::Dot);
    outputEdit->setValidator(validator);
    hbx21->addWidget(portionsLabel,0,Qt::AlignLeft);
    hbx21->addWidget(outputEdit,0,Qt::AlignLeft);
    hbx21->addSpacing(100);
    connect(outputEdit, SIGNAL(textChanged(QString)), this, SLOT(priceSlot()));

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
    connect(btnAddRow, SIGNAL(clicked()), this, SLOT(btnAddRowClicked()));
    connect(table, SIGNAL(cellChanged(int,int)), this, SLOT(dataChanged(int,int)));
    vbx->addWidget(table);
    QHBoxLayout* hbxSize = new QHBoxLayout;
    vbx->addLayout(hbxSize);
    vbx->setAlignment(hbxSize, Qt::AlignRight);
    QLabel* sizeLabel = new QLabel(tr("Порция, г.: "),this);
    portionEdit = new QLineEdit("",this);
    portionEdit->setMaximumSize(100, 25);
    //MyDoubleValidator* sizeEditValidator = new MyDoubleValidator(0,10000,3,sizeEdit);
    portionEdit->setValidator(validator);
    hbxSize->addWidget(sizeLabel, Qt::AlignRight);
    hbxSize->setAlignment(sizeLabel, Qt::AlignRight);
    hbxSize->addWidget(portionEdit, Qt::AlignRight);
    connect(portionEdit, SIGNAL(textChanged(QString)), this, SLOT(priceSlot()));
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
    connect(btnOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
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

void CalcItem::setupCalcFolder(){
    setWindowFlag(Qt::WindowMaximizeButtonHint,true);
    resize(800,400);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(false);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    vbx->addLayout(hbx1);
    QLabel* nameLabel = new QLabel(tr("Наименование: "), this);
    nameEdit = new QLineEdit(this);
    nameEdit->setText("");
    hbx1->addWidget(nameLabel);
    hbx1->addWidget(nameEdit);

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
    connect(btnOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
    hbxBtn2->addWidget(btnOk);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    btnCancel->setMaximumSize(100,30);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    hbxBtn2->addWidget(btnCancel);
    btnOk->setFocusPolicy(Qt::NoFocus);
    btnCancel->setFocusPolicy(Qt::NoFocus);
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
        emit outputEdit->textChanged("");
    }
    if(row == -1){
        double sum = 0;
        for(int i = 0; i < table->rowCount(); ++i){
            if(table->item(i,4)){
                sum+=table->item(i,4)->data(Qt::DisplayRole).toDouble();
            }
        }

        sumLabel->setText(QString::number(sum, 'f', 2));
        emit outputEdit->textChanged("");
    }
}

void CalcItem::priceSlot(){
    bool ok;
    double portion = outputEdit->text().toDouble(&ok);
    if(ok && portion){
        priceLabel->setText(QString::number(sumLabel->text().toDouble()*portionEdit->text().toDouble()/portion/1000, 'f',2));
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
    list<<nameEdit->text()<<outputEdit->text()<<dateEdit->date().toString("dd.MM.yyyy")<<sumLabel->text()<<portionEdit->text()<<priceLabel->text();
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
    //PrintWidget* print = new PrintWidget(data, this);
    //print->exec();
    MyPrintWidget* print = new MyPrintWidget(data);
    print->show();
}

void CalcItem::slotOkClicked(){
    QList<QVariant> data;
    QString name = nameEdit->text();
    data<<num<<parentNum<<dirNum;
    if(!dirNum){
        QString date = dateEdit->text();
        QString price = priceLabel->text();
        QString output = outputEdit->text();
        QString portion = portionEdit->text();
        data<<date<<name<<price<<output<<portion;
        for(int i = 0; i<table->rowCount(); ++i){
            data<<table->getItemNum(i)<<table->item(i, 2)->data(Qt::DisplayRole).toDouble();
        }
    }else{
        data<<QVariant()<<name<<QVariant()<<QVariant()<<QVariant();
    }
    emit sendData(data, parentItem);
    accept();
}

void CalcItem::btnAddRowClicked(){
    table->addNewRow();
    emit static_cast<CellWidget*>(table->cellWidget(table->rowCount()-1, 0))->buttonWidget()->clicked();
}
