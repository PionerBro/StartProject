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

    CalculatorMaterialsTreeModel* getModel();
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
    QAction* historiAct;
private slots:
    void createItem();
    void createFolder();
    void editItem();
    //void deleteItem();
    void selectItem();
    void treeTypeModelSlot(bool);
    void editListActSlot(bool);
    void reserveDataChangedSlot();
    void acceptActTriggeredSlot();
    void acceptActSlot();
    void rejectActSlot();
    void historyActClicked();
signals:
    void acceptActTr(const QVariant&);
    void histActTr(qlonglong);
    void crHistItem(QVector<QVariant>& data);
};

#endif // CALCULATORMATERIALSDIALOG_H
