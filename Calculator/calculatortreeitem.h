#ifndef CALCULATORTREEITEM_H
#define CALCULATORTREEITEM_H

#include <QVariant>

enum FieldName{
    Id = 0,
    ParentId = 1,
    FolderId = 2
};

enum ItemType{
    Element = 0,
    FolderClose = 1,
    FolderOpen = 2
};

class CalculatorTreeItem
{
public:
    CalculatorTreeItem(const QVector<QVariant>& rowData, CalculatorTreeItem* parent = 0);
    ~CalculatorTreeItem();


    QVariant data(int column) const;
    QVector<QVariant> rowData() const;
    CalculatorTreeItem* child(int row);
    CalculatorTreeItem* parent() const;
    int row() const;
    int columnCount()const;
    int childCount()const;
    void setRowData(const QVector<QVariant>& data);
    void setData(int column, const QVariant& data = QVariant());
    void appendChild(CalculatorTreeItem* child);
    void setSortCol(int col);
    int getSortCol()const;
    void sortItem();
    int getItemType()const;
protected:
    QVector<QVariant> itemData;
    QList<CalculatorTreeItem*> childItems;
    CalculatorTreeItem* parentItem;
private:
   int itemType;
   int sortCol;
};

bool compare(const CalculatorTreeItem* first, const CalculatorTreeItem* second);

#endif // CALCULATORTREEITEM_H
