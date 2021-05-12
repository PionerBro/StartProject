#ifndef MYPRINTWIDGET_H
#define MYPRINTWIDGET_H

#include <QWidget>

class MyPrintWidgetTable;

class MyPrintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyPrintWidget(QWidget *parent = nullptr);
private:
    MyPrintWidgetTable* tableView;

signals:

};

#endif // MYPRINTWIDGET_H
