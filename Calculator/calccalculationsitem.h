#ifndef CALCCALCULATIONSITEM_H
#define CALCCALCULATIONSITEM_H

#include <QDialog>

class QDateEdit;
class QLineEdit;
class QLabel;
class QTableView;
class CalcCalculationsItemModel;

class CalcCalculationsItem : public QDialog
{
    Q_OBJECT
public:
    CalcCalculationsItem(QVector<QVariant>& data, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void setNum(qlonglong num);
private:
    QVector<QVariant>& m_data;
    CalcCalculationsItemModel* model;
    QTableView* table;
    QDateEdit* dateEdit;
    QLineEdit* nameEdit;
    QLabel* priceLabel;
    QLineEdit* outputEdit;
    QLineEdit* portionEdit;
    QLabel* sumLabel;
public slots:
    void slotSumDataChanged();
    void slotPriceChanged();
private slots:
    void slotOkClicked();
};

#endif // CALCCALCULATIONSITEM_H
