#ifndef CALCULATORMATERIALSITEM_H
#define CALCULATORMATERIALSITEM_H

#include <QDialog>

class QLineEdit;
class QComboBox;

class CalculatorMaterialsItem : public QDialog
{
    Q_OBJECT
public:
    CalculatorMaterialsItem(QVector<QVariant> &data, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    void setupFolder();
    void setupElement();
private:
    QVector<QVariant>& m_data;
    QLineEdit* nameEdit;
    QComboBox* unitBox;
    QLineEdit* priceEdit;
private slots:
    void slotOkClicked();
};

#endif // CALCULATORMATERIALSITEM_H
