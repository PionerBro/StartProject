#ifndef MYPRINTWIDGETDELEGATE_H
#define MYPRINTWIDGETDELEGATE_H

#include <QItemDelegate>

class MyPrintWidgetDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MyPrintWidgetDelegate();
    virtual void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;

};

#endif // MYPRINTWIDGETDELEGATE_H
