#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QDialog>

class QPrinter;

class PrintWidget : public QDialog
{
    Q_OBJECT
public:
    explicit PrintWidget(const QList<QStringList>& data, QWidget *parent = nullptr);
protected:
    virtual void paintEvent(QPaintEvent* e)override;
    void drawDocum(QPainter *qp);
private:
    QList<QStringList> data;
private slots:
    void printText();
signals:

};

#endif // PRINTWIDGET_H
