#ifndef CALCULATORMATERIALSDELEGATE_H
#define CALCULATORMATERIALSDELEGATE_H

#include <QItemDelegate>

class CalculatorMaterialsDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CalculatorMaterialsDelegate(QObject* parent =nullptr);
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CALCULATORMATERIALSDELEGATE_H
