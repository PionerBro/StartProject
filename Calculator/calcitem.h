#ifndef CALCITEM_H
#define CALCITEM_H

#include <QDialog>


class QTableWidget;
class QLabel;
class CalcTableWidget;
class QLineEdit;
class QDateEdit;

class CalcItem : public QDialog
{
    Q_OBJECT
public:
    CalcItem(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CalcItem();

private:
    CalcTableWidget* table;
    QLabel* sumLabel;
    QLabel* priceLabel;
    QLineEdit* nameEdit;
    QLineEdit* portionEdit;
    QLineEdit* sizeEdit;
    QDateEdit* dateEdit;

public slots:
    void dataChanged(int,int);
    void printSlot();
    void priceSlot();
};

#endif // CALCITEM_H
