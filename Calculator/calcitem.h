#ifndef CALCITEM_H
#define CALCITEM_H

#include <QDialog>


class QTableWidget;
class QLabel;
class CalcTableWidget;
class QLineEdit;
class QDateEdit;

class MyTreeItem;

class CalcItem : public QDialog
{
    Q_OBJECT
public:
    enum CalcType{
        Element,
        Folder
    };
    Q_ENUM(CalcType)


    CalcItem(MyTreeItem* itemP, int type, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CalcItem();

private:
    void setupCalcItem();
    void setupCalcFolder();
private:
    CalcTableWidget* table;
    MyTreeItem* parentItem;
    qlonglong num;
    qlonglong parentNum;
    qlonglong dirNum;
    QDateEdit* dateEdit;
    QLineEdit* nameEdit;
    QLabel* priceLabel;
    QLineEdit* outputEdit;
    QLineEdit* portionEdit;
    QLabel* sumLabel;

public slots:
    void dataChanged(int,int);
    void printSlot();
    void priceSlot();
    void slotOkClicked();
signals:
    void sendData(QList<QVariant>&, MyTreeItem*);
};

#endif // CALCITEM_H
