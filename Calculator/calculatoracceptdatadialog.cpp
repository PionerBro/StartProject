#include "calculatoracceptdatadialog.h"

#include <QDateEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDate>

#include <QDebug>

CalculatorAcceptDataDialog::CalculatorAcceptDataDialog(QVariant& data, QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f), m_data(data)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QLabel* lbl = new QLabel(tr("Укажите дату принятия цен"), this);
    dateEdit = new QDateEdit(QDate().currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setFixedSize(80,20);
    QPushButton* btnOk = new QPushButton(tr("Ок"), this);
    QPushButton* btnCancel = new QPushButton(tr("Отмена"), this);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    QHBoxLayout* hbx2 = new QHBoxLayout;
    vbx->addLayout(hbx1);
    vbx->addLayout(hbx2);
    hbx1->addWidget(lbl);
    hbx1->addWidget(dateEdit);
    hbx2->addWidget(btnOk);
    hbx2->addWidget(btnCancel);

    connect(btnOk, SIGNAL(clicked()), this, SLOT(btnOkClicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void CalculatorAcceptDataDialog::btnOkClicked(){
    m_data = dateEdit->date();
    accept();
}
