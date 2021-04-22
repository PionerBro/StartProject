#ifndef DIRECTORYWIDGET_H
#define DIRECTORYWIDGET_H

#include <QDialog>

class QTableView;
class MyTreeModel;

class DirectoryWidget : public QDialog
{
    Q_OBJECT
public:
    DirectoryWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~DirectoryWidget();

    MyTreeModel* getModel() const;
private:
    QTableView* view;
    MyTreeModel* model;

    void viewSettings();

private slots:
    void createItem();
    void createFolder();
    void editItem();
    void deleteItem();
};

#endif // DIRECTORYWIDGET_H
