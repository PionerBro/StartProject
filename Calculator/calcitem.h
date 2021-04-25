#ifndef CALCITEM_H
#define CALCITEM_H

#include <QDialog>


class QTableWidget;
class QLabel;

class CalcItem : public QDialog
{
    Q_OBJECT
public:
    CalcItem(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CalcItem();

    int currRow()const;
    void setCurRow(int row);
private:
    int tableRows;
    int m_currentRow;
    QTableWidget* table;
    QLabel* sumLabel;

public slots:
    void addRow();
    void chooseElement();
    void receiveData(QList<QVariant>&);
    void dataChanged(int,int);
};

#endif // CALCITEM_H
