#ifndef CALCWIDGET_H
#define CALCWIDGET_H

#include <QWidget>

class QTableView;
class MyTreeModel;
class MyTreeItem;

class CalcWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalcWidget(QWidget *parent = nullptr);

private:
    void viewSettings();
private:
    QTableView* view;
    MyTreeModel* model;
public slots:
    void editItem();
private slots:
    void createItem();
    void createFolder();
    void deleteItem();
    void selectItem();
    void addNewElement(QList<QVariant>&,MyTreeItem*);
    void editElement(QList<QVariant>&,MyTreeItem*);
    void addNewFolder(QList<QVariant>&,MyTreeItem*);
signals:

};

#endif // CALCWIDGET_H
