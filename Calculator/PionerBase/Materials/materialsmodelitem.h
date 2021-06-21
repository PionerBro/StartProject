#ifndef MATERIALSMODELITEM_H
#define MATERIALSMODELITEM_H

#include <QVector>
#include <QVariant>

class MaterialsModelItem
{
public:
    enum DataName{
        Id = 0,
        ParentId = 1,
        FolderId = 2,
        Name = 3,
        Unit = 4,
        Price = 5,
        EnumCount = 6
    };

    enum ItemType{
        Element = 0,
        FolderClose = 1,
        FolderOpen = 2
    };

    MaterialsModelItem(const QVector<QVariant>& data, MaterialsModelItem* parent);
    ~MaterialsModelItem();


private:
    MaterialsModelItem* m_parent;
    QVector<MaterialsModelItem*> m_children;
    QVector<QVariant> m_data;
};

#endif // MATERIALSMODELITEM_H
