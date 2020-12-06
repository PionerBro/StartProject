#ifndef ITEM_H
#define ITEM_H

#include <QDialog>


class QLineEdit;

class Item : public QDialog
{
    Q_OBJECT
public:
    explicit Item(QStringList& data, QWidget *parent = nullptr);

private:
    QStringList& m_data;
    int code;
    QString measure = "";
    QLineEdit* lneName;
    QLineEdit* lneVendorCode;
    QLineEdit* lnePrice;
private slots:
    void btnOkClicked();
    void btnMeasureClicked();
signals:

};

#endif // ITEM_H
