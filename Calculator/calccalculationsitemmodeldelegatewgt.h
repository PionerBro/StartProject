#ifndef CALCCALCULATIONSITEMMODELDELEGATEWGT_H
#define CALCCALCULATIONSITEMMODELDELEGATEWGT_H

#include <QWidget>
#include <QVariant>
#include <QModelIndex>

class QLabel;
class QPushButton;


class CalcCalculationsItemModelDelegateWgt : public QWidget
{
    Q_OBJECT
public:
    explicit CalcCalculationsItemModelDelegateWgt(const QModelIndex& index, QWidget *parent = nullptr);

    void setLabelText(const QString&);
    const QVector<QVariant>& data() const;
private:
    QVector<QVariant> m_data;
    QLabel* lbl;
    QPushButton* btn;
    QModelIndex m_index;
private slots:
    void btnClicked();
    void receiveData(QVector<QVariant>&);
signals:

};

#endif // CALCCALCULATIONSITEMMODELDELEGATEWGT_H
