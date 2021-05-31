#include "calculatormaterialsitem.h"
#include "calcultatorunitstablemodel.h"
#include "mydoublevalidator.h"

#include <QVariant>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>

#include <QDebug>

extern CalcultatorUnitsTableModel* unitsModel;

CalculatorMaterialsItem::CalculatorMaterialsItem(QVector<QVariant> &data, QWidget* parent, Qt::WindowFlags f):QDialog(parent, f), m_data(data)
{
    setAttribute(Qt::WA_DeleteOnClose);
    if(m_data.value(2).toInt()){
        setupFolder();
    }else{
        setupElement();
    }
}

void CalculatorMaterialsItem::setupFolder(){
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    QLabel* nameLabel = new QLabel(tr("Наименование"), this);
    nameEdit = new QLineEdit(this);
    nameEdit->setText(m_data.value(3).toString());
    hbx1->addWidget(nameLabel);
    hbx1->addWidget(nameEdit);
    vbx->addLayout(hbx1);


    QHBoxLayout* hbx4 = new QHBoxLayout;
    QPushButton* btnOk = new QPushButton(tr("Ок"), this);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    hbx4->addWidget(btnOk);
    hbx4->addWidget(btnCancel);
    vbx->addLayout(hbx4);

    connect(btnOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void CalculatorMaterialsItem::setupElement(){
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    QLabel* nameLabel = new QLabel(tr("Наименование"), this);
    nameEdit = new QLineEdit(this);
    nameEdit->setText(m_data.value(3).toString());
    hbx1->addWidget(nameLabel);
    hbx1->addWidget(nameEdit);
    vbx->addLayout(hbx1);

    QHBoxLayout* hbx2 = new QHBoxLayout;
    QLabel* unitLabel = new QLabel(tr("Ед.Изм."),this);
    //QList<QString> unitList;
    //unitList<<tr("шт.")<<tr("кг.")<<tr("меш.");
    unitBox = new QComboBox(this);
    unitBox->setModel(unitsModel);
    unitBox->setModelColumn(1);
    //unitBox->addItems(unitList);
    unitBox->setCurrentIndex(0);
    hbx2->addWidget(unitLabel);
    hbx2->addWidget(unitBox);
    vbx->addLayout(hbx2);

    QHBoxLayout* hbx3 = new QHBoxLayout;
    QLabel* priceLabel = new QLabel(tr("Цена"), this);
    priceEdit = new QLineEdit(this);
    priceEdit->setText(QString::number(m_data.value(5).toDouble(),'f', 2));
    priceEdit->setValidator(new MyDoubleValidator(0, 100000, 2, priceEdit, MyDoubleValidator::Dot, MyDoubleValidator::Expanded));
    hbx3->addWidget(priceLabel);
    hbx3->addWidget(priceEdit);
    vbx->addLayout(hbx3);

    QHBoxLayout* hbx4 = new QHBoxLayout;
    QPushButton* btnOk = new QPushButton(tr("Ок"), this);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    hbx4->addWidget(btnOk);
    hbx4->addWidget(btnCancel);
    vbx->addLayout(hbx4);

    connect(btnOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void CalculatorMaterialsItem::slotOkClicked(){
    QVector<QVariant> rData;
    rData<<m_data.value(0)<<m_data.value(1)<<m_data.value(2);
    QString name = nameEdit->text();
    if(!m_data.value(2).toInt()){
        QString unit = unitBox->currentData(Qt::DisplayRole).toString();
        QString price = priceEdit->text();
        if(name.isEmpty() || unit.isEmpty() || price == "0.00"){
            QMessageBox::information(0, "Error", tr("Заполните все позиции"));
            return;
        }
        rData<<name<<unit<<price;
    }else{
        if(name.isEmpty()){
            QMessageBox::information(0, "Error", tr("Заполните все позиции"));
            return;
        }
        rData<<name<<QVariant()<<QVariant();
    }
    m_data.clear();
    m_data<<rData;

    accept();
}
