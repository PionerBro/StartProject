#ifndef ORDER_H
#define ORDER_H

#include <QWidget>

class QDateTime;
class QTableWidget;
class QPushButton;
class QLineEdit;
class QDateTimeEdit;
class QComboBox;

class Order : public QWidget
{
    Q_OBJECT
private:
    double sum = 0;
    QLineEdit* lne_number;
    QDateTimeEdit* date;
    QLineEdit* lne_name;
    QComboBox* list_source;
    QLineEdit* lne_destination;
    int t_row = 10;
    const int t_col = 7;
    QTableWidget* table;
    QPushButton* btn_ok;
    QPushButton* btn_close;
public:
    explicit Order(QWidget *parent = nullptr);
private slots:
    void slotOkClicked();
signals:
    void signalNewDoc(QStringList);
};

#endif // ORDER_H
