#ifndef CALCULATORACCEPTDATADIALOG_H
#define CALCULATORACCEPTDATADIALOG_H

#include <QDialog>

class QDateEdit;

class CalculatorAcceptDataDialog : public QDialog
{
    Q_OBJECT
public:
    CalculatorAcceptDataDialog(QVariant& data, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
private:
    QDateEdit* dateEdit;
    QVariant& m_data;
private slots:
    void btnOkClicked();
};

#endif // CALCULATORACCEPTDATADIALOG_H
