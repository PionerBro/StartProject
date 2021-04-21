#ifndef CALCITEM_H
#define CALCITEM_H

#include <QDialog>

class CalcItem : public QDialog
{
    Q_OBJECT
public:
    CalcItem(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CalcItem();

};

#endif // CALCITEM_H
