#ifndef MYPRINTWIDGET_H
#define MYPRINTWIDGET_H

#include <QWidget>

class MyPrintWidgetTable;
class QTableWidgetItem;

class MyPrintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyPrintWidget(const QList<QStringList>& data, QWidget *parent = nullptr);
private:
    MyPrintWidgetTable* tableView;
    void setupPrintList(const QList<QStringList>& data);
    QTableWidgetItem* setupTableItem(const QVariant& data, bool border = false, Qt::Alignment align = Qt::AlignBottom, QFont font = QFont("TimesNewRoman", 12));
private slots:
    void printDoc();
signals:

};

#endif // MYPRINTWIDGET_H
