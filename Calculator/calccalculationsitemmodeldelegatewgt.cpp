#include "calccalculationsitemmodeldelegatewgt.h"
#include "calculatormaterialsdialog.h"
#include "calculatormaterialstreemodel.h"
#include "calccalculationsmodelitem.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

extern CalculatorMaterialsDialog* calcMaterialsWidget;


CalcCalculationsItemModelDelegateWgt::CalcCalculationsItemModelDelegateWgt(const QModelIndex& index, QWidget *parent) : QWidget(parent)
{
    m_index = index;
    setFixedHeight(25);
    QHBoxLayout* hbx = new QHBoxLayout(this);
    hbx->setContentsMargins(0,0,0,0);
    lbl = new QLabel(this);
    btn = new QPushButton(".", this);
    btn->setFixedSize(25,25);
    hbx->addWidget(lbl);
    hbx->addWidget(btn);
    connect(btn, SIGNAL(clicked()), this, SLOT(btnClicked()));
}

void CalcCalculationsItemModelDelegateWgt::setLabelText(const QString &text){
    lbl->setText(text);
}

const QVector<QVariant>& CalcCalculationsItemModelDelegateWgt::data() const{
    return m_data;
}

void CalcCalculationsItemModelDelegateWgt::btnClicked(){
    connect(calcMaterialsWidget->getModel(), SIGNAL(sendData(QVector<QVariant>&)), this, SLOT(receiveData(QVector<QVariant>&)));
    connect(calcMaterialsWidget->getModel(), SIGNAL(sendData(QVector<QVariant>&)), calcMaterialsWidget, SLOT(accept()));
    calcMaterialsWidget->exec();
    disconnect(calcMaterialsWidget->getModel(), SIGNAL(sendData(QVector<QVariant>&)), this, SLOT(receiveData(QVector<QVariant>&)));
    disconnect(calcMaterialsWidget->getModel(), SIGNAL(sendData(QVector<QVariant>&)), calcMaterialsWidget, SLOT(accept()));

}

void CalcCalculationsItemModelDelegateWgt::receiveData(QVector<QVariant>& data){
    //lbl->setText(data.value(3).toString());
    CalcCalculationsModelItem* item =static_cast<CalcCalculationsModelItem*>(m_index.internalPointer());
    item->setData(1, data.value(3));
    item->setData(2, data.value(4));
    item->setData(4, data.value(5));
}
