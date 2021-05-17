#ifndef CALCWIDGET_H
#define CALCWIDGET_H

#include <QWidget>

class CalcWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalcWidget(QWidget *parent = nullptr);

private slots:
    void crDialog();
    void crCalc();
signals:

};

#endif // CALCWIDGET_H
