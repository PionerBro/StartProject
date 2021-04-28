#include "calcitem2delegate.h"
#include <QLineEdit>
#include <QDoubleValidator>

CalcItem2Delegate::CalcItem2Delegate(QObject* parent):QItemDelegate(parent)
{    
}

QWidget* CalcItem2Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
    QLineEdit* l = new QLineEdit(parent);
    l->setValidator(new QDoubleValidator(0,999,2,l));
    return l;
}

void CalcItem2Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    static_cast<QLineEdit* >(editor)->setText(index.model()->data(index).toString());
}

void CalcItem2Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    QString text = static_cast<QLineEdit*>(editor)->text();
    int count = 0;
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

    model->setData(index,text);
}

void CalcItem2Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const{
    editor->setGeometry(option.rect);
}
