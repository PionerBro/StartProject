#include "diritem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <QDebug>


DirItem::DirItem(QWidget* parent, Qt::WindowFlags f):QDialog(parent, f)
{
    QList<QVariant> data;
    setupDirItem(data);
}

DirItem::DirItem(const QList<QVariant>& data, QWidget* parent, Qt::WindowFlags f):QDialog(parent,f){
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
    QLineEdit* nameEdit = new QLineEdit(this);
    nameEdit->setText(data.value(1).toString());
    hbx1->addWidget(nameLabel);
    hbx1->addWidget(nameEdit);
    vbx->addLayout(hbx1);

    QHBoxLayout* hbx2 = new QHBoxLayout;
    QLabel* priceLabel = new QLabel(tr("Цена"), this);
    QLineEdit* priceEdit = new QLineEdit(this);
    hbx2->addWidget(priceLabel);
    hbx2->addWidget(priceEdit);
    vbx->addLayout(hbx2);

    QHBoxLayout* hbx3 = new QHBoxLayout;
    QPushButton* btnOk = new QPushButton(tr("Ок"), this);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    hbx3->addWidget(btnOk);
    hbx3->addWidget(btnCancel);
    vbx->addLayout(hbx3);

    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}
