#include "diritem.h"
#include "mydoublevalidator.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>

#include <QDebug>


DirItem::DirItem(qlonglong rootItemNum, QList<QVariant> &data, QWidget* parent, Qt::WindowFlags f):QDialog(parent, f), m_data(data)
{
    num = 0;
    parentNum = rootItemNum;
    dirNum = 0;
    setupDirItem({});
}

DirItem::DirItem(QList<QVariant>& data, QWidget* parent, Qt::WindowFlags f):QDialog(parent,f), m_data(data){
    num = data.value(0).toLongLong();
    parentNum = data.value(1).toLongLong();
    dirNum = 0;
    setupDirItem(data);
}

DirItem::~DirItem(){
    qDebug()<<"item destroyed";
}

void DirItem::setupDirItem(const QList<QVariant>& data){
    setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    QLabel* nameLabel = new QLabel(tr("Наименование"), this);
    nameEdit = new QLineEdit(this);
    nameEdit->setText(data.value(3).toString());
    hbx1->addWidget(nameLabel);
    hbx1->addWidget(nameEdit);
    vbx->addLayout(hbx1);

    QHBoxLayout* hbx2 = new QHBoxLayout;
    QLabel* unitLabel = new QLabel(tr("Ед.Изм."),this);
    QList<QString> unitList;
    unitList<<tr("шт.")<<tr("кг.")<<tr("меш.");
    unitBox = new QComboBox(this);
    unitBox->addItems(unitList);
    unitBox->setCurrentIndex(unitList.indexOf(data.value(4).toString()));
    hbx2->addWidget(unitLabel);
    hbx2->addWidget(unitBox);
    vbx->addLayout(hbx2);

    QHBoxLayout* hbx3 = new QHBoxLayout;
    QLabel* priceLabel = new QLabel(tr("Цена"), this);
    priceEdit = new QLineEdit(this);
    priceEdit->setText(QString::number(data.value(5).toDouble(),'f', 2));
    priceEdit->setValidator(new MyDoubleValidator(0, 100000, 2, priceEdit, MyDoubleValidator::Dot, MyDoubleValidator::Expanded));
    hbx3->addWidget(priceLabel);
    hbx3->addWidget(priceEdit);
    vbx->addLayout(hbx3);

    QHBoxLayout* hbx4 = new QHBoxLayout;
    btnOk = new QPushButton(tr("Ок"), this);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    hbx4->addWidget(btnOk);
    hbx4->addWidget(btnCancel);
    vbx->addLayout(hbx4);

    connect(btnOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void DirItem::slotOkClicked(){
    QString name = nameEdit->text();
    QString unit = unitBox->currentData(Qt::DisplayRole).toString();
    QString price = priceEdit->text();
    if(name.isEmpty() || unit.isEmpty() || price == "0.00"){
        QMessageBox::information(0, "Error", tr("Заполните все позиции"));
        return;
    }
    m_data.clear();
    m_data<<num<<parentNum<<dirNum<<name<<unit<<price;
    accept();
}
