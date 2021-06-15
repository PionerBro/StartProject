#ifndef CALCCALCULATIONSITEMMODELDELEGATE_H
#define CALCCALCULATIONSITEMMODELDELEGATE_H

#include <QItemDelegate>

class QLabel;

class CalcCalculationsItemModelDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CalcCalculationsItemModelDelegate(QObject* parent = nullptr);
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
signals:
    void sumDataChanged() const;
};

#endif // CALCCALCULATIONSITEMMODELDELEGATE_H
