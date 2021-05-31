#ifndef CALCULATORMATERIALSDIALOG_H
#define CALCULATORMATERIALSDIALOG_H

#include <QDialog>

class QTableView;
class CalculatorMaterialsTreeModel;


class CalculatorMaterialsDialog : public QDialog
{
    Q_OBJECT
public:
    CalculatorMaterialsDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    QTableView* view;
    CalculatorMaterialsTreeModel* model;

    void viewSettings();
    QAction* newAct;
    QAction* editAct;
    QAction* newFolder;
    QAction* delItem;
    QAction* selItem;
    QAction* treeTypeModel;
    QAction* editListAct;
    QAction* acceptAct;
    QAction* rejectAct;
private slots:
    void createItem();
    void createFolder();
    void editItem();
    //void deleteItem();
    void selectItem();
    void treeTypeModelSlot(bool);
    void editListActSlot(bool);
    void reserveDataChangedSlot();
    void acceptActSlot();
    void rejectActSlot();
};

#endif // CALCULATORMATERIALSDIALOG_H
