#ifndef CALCCALCULATIONSMODELITEM_H
#define CALCCALCULATIONSMODELITEM_H

#include <QVariant>

class CalcCalculationsModelItem
{
public:
    CalcCalculationsModelItem(const QVector<QVariant>& data, CalcCalculationsModelItem* parent = nullptr);
    ~CalcCalculationsModelItem();

    QVariant data(int column) const;
    CalcCalculationsModelItem* child(int row) const;
    CalcCalculationsModelItem* parent() const;
    int row() const;
    int columnCount()const;
    int childCount()const;
    void appendChild(CalcCalculationsModelItem* child);
    void setData(int column, const QVariant& data);
    void setRowData(const QVector<QVariant> &data);
    QVector<QVariant> rowData() const;
    void sortItem();
    int getSortCol()const;
private:
    QVector<QVariant> m_itemData;
    QVector<CalcCalculationsModelItem*> m_childItems;
    CalcCalculationsModelItem* m_parent;
    int m_sortCol;


};

bool compare(const CalcCalculationsModelItem* first, const CalcCalculationsModelItem* second);
#endif // CALCCALCULATIONSMODELITEM_H
