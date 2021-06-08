#include "calccalculationsitem.h"
#include "mydoublevalidator.h"
#include "calcitem2delegate.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QAbstractItemView>
#include <QDateEdit>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>

CalcCalculationsItem::CalcCalculationsItem(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f)
{
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
    table = new QTableView(this);
    QStringList headers;
    headers<<tr("Наименование")
             <<tr("Ед.изм")
             <<tr("Кол-во")
             <<tr("Цена")
             <<tr("Сумма");
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
