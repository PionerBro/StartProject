#ifndef CALCULATORMATERIALSHISTORYDIALOG_H
#define CALCULATORMATERIALSHISTORYDIALOG_H

#include <QDialog>

class QTableView;
class CalculatorHistoryModel;

class CalculatorMaterialsHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    CalculatorMaterialsHistoryDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    CalculatorHistoryModel* getModel()const;
private:
    void viewSettings();
private:
    QTableView* tableView;
    CalculatorHistoryModel* model;
private slots:
    void showAtNum(qlonglong num);
    void createItem(QVector<QVariant>& data);

};

#endif // CALCULATORMATERIALSHISTORYDIALOG_H
