#ifndef TREEITEM_H
#define TREEITEM_H

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

class TreeItem
{
public:
    TreeItem(const QVector<QVariant>& rowData, TreeItem* parent = 0);
    ~TreeItem();


    QVariant data(int column) const;
    QVector<QVariant> rowData() const;
    TreeItem* child(int row);
    TreeItem* parent() const;
    int row() const;
    int columnCount()const;
    int childCount()const;
    void setRowData(const QVector<QVariant>& data);
    void setData(int column, const QVariant& data = QVariant());
    void appendChild(TreeItem* child);
    void setSortCol(int col);
    int getSortCol()const;
    void sortItem();
    int getItemType()const;
protected:
    QVector<QVariant> itemData;
    QList<TreeItem*> childItems;
    TreeItem* parentItem;
private:
   int itemType;
   int sortCol;
};

bool compare(const TreeItem* first, const TreeItem* second);


#endif // TREEITEM_H
