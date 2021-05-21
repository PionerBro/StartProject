#include "myitemdelegate.h"
#include "mydoublevalidator.h"
#include "mytreemodel.h"

#include <QLineEdit>
#include <QDoubleValidator>


MyItemDelegate::MyItemDelegate(QObject* parent):QItemDelegate(parent)
{

}

QWidget* MyItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
    QLineEdit* l = new QLineEdit(parent);
    l->setValidator(new MyDoubleValidator(0,999,2,l,MyDoubleValidator::DigitalPoint::Dot, MyDoubleValidator::DecVersion::Expanded));
    return l;
}

void MyItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    //static_cast<QLineEdit* >(editor)->setText(index.model()->data(index).toString());
    static_cast<QLineEdit* >(editor)->setText(index.model()->data(index).toString());
}

void MyItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    QString text = static_cast<QLineEdit*>(editor)->text();
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
    static_cast<MyTreeModel*>(model)->reserveDataChanged(index.row(), text);
}

void MyItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const{
    editor->setGeometry(option.rect);
}
