#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include <QModelIndex>

class TestItemTableDatabase;
class QTreeView;
class QStandardItemModel;

class Test : public QDialog
{
    Q_OBJECT
public:
    Test(QWidget* parent = nullptr);
private:
    enum ViewHeaders{
        Name = 0,
        id = 1,
        Measure = 2,
        Price = 3,
        Folder = 4,
        Parent = 5,
        ViewHeadersCount = 6
    };

    QList<QModelIndex> itemId;
    TestItemTableDatabase* db;
    QTreeView* view;
    QStandardItemModel* model;
 private:
    void setupModel(const QStringList& headers);
    void createView();
    int branch = 0;
private slots:
    void createItem();
    void editRecords();
    void deleteRecords();
    void createFolder();
    void itemExpanded(const QModelIndex&);
    void itemCollapsed(const QModelIndex&);
    void updateTree();
};


#endif // TEST_H
