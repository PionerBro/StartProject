#include "myprintwidgetdelegate.h"
#include <QPainter>

MyPrintWidgetDelegate::MyPrintWidgetDelegate()
{

}

void MyPrintWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const {
      QItemDelegate::paint(painter, option, index );
      if(index.data(Qt::UserRole+1).toBool()){
         painter->setPen(Qt::black) ;
         painter->drawRect(option.rect);
      }
}
