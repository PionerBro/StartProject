#include "calccalculationsmodelitem.h"
#include "calculatormaterialstreemodel.h"
#include "calccalculationsitemmodeldelegate.h"
#include "calccalculationsitemmodeldelegatewgt.h"
#include "calculatormaterialsdialog.h"
#include "mydoublevalidator.h"

#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

extern CalculatorMaterialsDialog* calcMaterialsWidget;

CalcCalculationsItemModelDelegate::CalcCalculationsItemModelDelegate(QObject* parent) : QItemDelegate(parent)
{

}

QWidget* CalcCalculationsItemModelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const{
    if(index.column() == 1){
        CalcCalculationsItemModelDelegateWgt* wgt = new CalcCalculationsItemModelDelegateWgt(index,parent);
        return wgt;
    }else{
        QLineEdit* l = new QLineEdit(parent);
        l->setValidator(new MyDoubleValidator(0,999,2,l,MyDoubleValidator::DigitalPoint::Dot, MyDoubleValidator::DecVersion::Expanded));
        return l;
    }
}

void CalcCalculationsItemModelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    if(index.column()!=1){
        //static_cast<CalcCalculationsItemModelDelegateWgt*>(editor)->setLabelText(index.model()->data(index).toString());
        //static_cast<CalcCalculationsModelItem*>(index.internalPointer())->setData(0,"");
        static_cast<QLineEdit* >(editor)->setText(index.model()->data(index).toString());
    }
}

void CalcCalculationsItemModelDelegate::setModelData(QWidget *editor, QAbstractItemModel*, const QModelIndex &index) const{
    /*int count = 0;
    int size = text.size();
    int num = size - 1;
    if(size!=0){
        for(; num>=0; --num){
            if(text[num] == ',')
                break;
            ++count;
        }
        if(count == size)
            text.append(".00");
        else if(count == 0){
            text[num] = '.';
            text.append("00");
        }
        else if(count == 1){
           text[num] = '.';
           text.append("0");
        }
        else if(count == 2)
           text[num] = '.';
    }
    */
    CalcCalculationsModelItem* item =static_cast<CalcCalculationsModelItem*>(index.internalPointer());
    if(index.column()!=1){
        QString text = static_cast<QLineEdit*>(editor)->text();
        item->setData(index.column(), text);
    }
    if(item->data(3) != QVariant() && item->data(4) != QVariant()){
        item->setData(5, QString::number(item->data(3).toDouble() * item->data(4).toDouble(), 'f', 2));
        emit sumDataChanged();
    }
    //model->setData(index,text);
}

void CalcCalculationsItemModelDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const{
    editor->setGeometry(option.rect);
}
