#ifndef CALCITEM_H
#define CALCITEM_H

#include <QDialog>


class QTableWidget;
class QLabel;
class CalcTableWidget;

class CalcItem : public QDialog
{
    Q_OBJECT
public:
    CalcItem(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CalcItem();

private:
    CalcTableWidget* table;
    QLabel* sumLabel;

public slots:
    void dataChanged(int,int);
};

#endif // CALCITEM_H
