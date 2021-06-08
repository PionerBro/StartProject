#ifndef CALCULATORHISTORYITEM_H
#define CALCULATORHISTORYITEM_H

#include <QVariant>

class CalculatorHistoryItem
{
public:
    CalculatorHistoryItem(const QVector<QVariant>& data, CalculatorHistoryItem* parent = 0);
    ~CalculatorHistoryItem();

    QVariant data(int column) const;
    QVector<QVariant> rowData() const;
    CalculatorHistoryItem* child(int row);
    CalculatorHistoryItem* parent() const;
    int row() const;
    int columnCount()const;
    int childCount()const;
    void setRowData(const QVector<QVariant>& data);
    void setData(int column, const QVariant& data = QVariant());
    void appendChild(CalculatorHistoryItem* child);
    void setSortCol(int col);
    int getSortCol()const;
    void sortItem();

protected:
    QVector<QVariant> itemData;
    QList<CalculatorHistoryItem*> childItems;
    CalculatorHistoryItem* parentItem;
private:
   int itemType;
   int sortCol;
};

bool compare(const CalculatorHistoryItem* first, const CalculatorHistoryItem* second);

#endif // CALCULATORHISTORYITEM_H
