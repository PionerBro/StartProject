#ifndef CALCITEM2DELEGATE_H
#define CALCITEM2DELEGATE_H

#include <QItemDelegate>

class CalcItem2Delegate : public QItemDelegate
{
    Q_OBJECT
public:
    CalcItem2Delegate(QObject* parent =nullptr);
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CALCITEM2DELEGATE_H
