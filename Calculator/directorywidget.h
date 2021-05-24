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
    QAction* newAct;
    QAction* editAct;
    QAction* newFolder;
    QAction* delItem;
    QAction* selItem;
    QAction* chTypeModel;
    QAction* editListAct;
    QAction* acceptAct;
    QAction* rejectAct;
private slots:
    void createItem();
    void createFolder();
    void editItem();
    void deleteItem();
    void selectItem();
    void chTypeModelSlot(bool);
    void editListActSlot(bool);
    void reserveDataChangedSlot(bool);
    void acceptActSlot();
    void rejectActSlot();
};

#endif // DIRECTORYWIDGET_H
