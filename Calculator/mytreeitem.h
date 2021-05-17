#ifndef MYTREEITEM_H
#define MYTREEITEM_H

#include <QVariant>
#include <QList>

class MyTreeItem
{
public:
    MyTreeItem(const QList<QVariant> &data, MyTreeItem* parent = 0);
    ~MyTreeItem();

    QVariant data(int column) const;
    QList<QVariant> rowData() const;
    MyTreeItem* child(int row);
    MyTreeItem* parent();
    int row() const;
    int columnCount()const;
    int childCount()const;
    void sortItem();
    void setRowData(const QList<QVariant>& data);

    void appendChild(MyTreeItem* child);
    bool folder;
    bool isOpen;
private:

    QList<QVariant> itemData;
    QList<MyTreeItem*> childItems;
    MyTreeItem* parentItem;
};

#endif // MYTREEITEM_H
