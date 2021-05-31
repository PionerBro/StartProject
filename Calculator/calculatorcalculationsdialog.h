#ifndef CALCULATORCALCULATIONSDIALOG_H
#define CALCULATORCALCULATIONSDIALOG_H

#include <QDialog>

class QTableView;
class CalculatorMaterialsTreeModel;

class CalculatorCalculationsDialog : public QDialog
{
    Q_OBJECT
public:
    CalculatorCalculationsDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
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
private slots:
    void createItem();
    void createFolder();
    void editItem();
    //void deleteItem();
    void selectItem();
    void treeTypeModelSlot(bool);
};

#endif // CALCULATORCALCULATIONSDIALOG_H
