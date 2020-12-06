#include "item.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

Item::Item(QStringList& data, QWidget *parent) : QDialog(parent), m_data(data)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMinimumSize(178,152);

    QLabel* lblVendorCode = new QLabel(tr("Артикул"),this);
    lneVendorCode = new QLineEdit(this);
    QHBoxLayout* hbxVendorCode = new QHBoxLayout;
    hbxVendorCode->addWidget(lblVendorCode);
    hbxVendorCode->addWidget(lneVendorCode);

    QLabel* lblName = new QLabel(tr("Наименование"), this);
    lneName = new QLineEdit(this);
    QHBoxLayout* hbxName = new QHBoxLayout;
    hbxName->addWidget(lblName);
    hbxName->addWidget(lneName);

    QLabel* lblMeasure = new QLabel(tr("..."),this);
    QPushButton* btnMeasure = new QPushButton(tr("Ед. Изм."), this);
    QHBoxLayout* hbxMeasure = new QHBoxLayout;
    connect(btnMeasure, SIGNAL(clicked()), this, SLOT(btnMeasureClicked()));
    hbxMeasure->addWidget(lblMeasure);
    hbxMeasure->addWidget(btnMeasure);

    QLabel* lblPrice = new QLabel(tr("Цена"), this);
    lnePrice = new QLineEdit(this);
    QHBoxLayout* hbxPrice = new QHBoxLayout;
    hbxPrice->addWidget(lblPrice);
    hbxPrice->addWidget(lnePrice);

    QPushButton* btnOk = new QPushButton(tr("Ок"), this);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    QHBoxLayout* hbxButtons = new QHBoxLayout;
    connect(btnOk, SIGNAL(clicked()), this, SLOT(btnOkClicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    hbxButtons->addWidget(btnOk);
    hbxButtons->addWidget(btnCancel);


    QVBoxLayout* vbx = new QVBoxLayout(this);
    vbx->addLayout(hbxVendorCode);
    vbx->addLayout(hbxName);
    vbx->addLayout(hbxMeasure);
    vbx->addLayout(hbxPrice);
    vbx->addLayout(hbxButtons);

    lneVendorCode->setText(m_data[0]);
    lneName->setText(m_data[1]);
    measure = data[2];
    lnePrice->setText(data[3]);
}

void Item::btnOkClicked(){
    m_data.clear();
    m_data<<lneVendorCode->text()<<lneName->text()<<"2"<<lnePrice->text();
    this->accept();
}

void Item::btnMeasureClicked(){

}















