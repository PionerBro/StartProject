#ifndef CALCCALCULATIONSITEM_H
#define CALCCALCULATIONSITEM_H

#include <QDialog>

class QDateEdit;
class QLineEdit;
class QLabel;
class QTableView;

class CalcCalculationsItem : public QDialog
{
    Q_OBJECT
public:
    CalcCalculationsItem(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:

    QTableView* table;
    QDateEdit* dateEdit;
    QLineEdit* nameEdit;
    QLabel* priceLabel;
    QLineEdit* outputEdit;
    QLineEdit* portionEdit;
    QLabel* sumLabel;
};

#endif // CALCCALCULATIONSITEM_H
