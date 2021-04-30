#include "printwidget.h"
#include <QPainter>

#include <QDebug>

PrintWidget::PrintWidget(const QList<QStringList>& p_data, QWidget *parent) : QDialog(parent), data(p_data)
{
    resize(740, 700);
    setAttribute(Qt::WA_DeleteOnClose);
    //qp = new QPainter(this);
}

void PrintWidget::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
   QPainter qp(this);
   drawDocum(&qp);
}

void PrintWidget::drawDocum(QPainter* qp){
    qp->setPen(QPen(Qt::black, 2));
    qp->setFont(QFont("TimesNewRoman", 10));
    qDebug()<<"Paint";
    int xCoord = 20;
    int yCoord = 30;
    int width = 700;
    int hTab = 25;
    qp->drawText(500, yCoord, "УТВЕРЖДАЮ");
    yCoord+=hTab;
    qp->drawText(450, yCoord, "Директор ЧПТУП \"В.П.Бутковский\"");
    yCoord+=hTab;
    qp->drawText(450, yCoord, "_______________Бутквоский В.П.");
    yCoord+=2*hTab;
    qp->setFont(QFont("TimesNewRoman", 14));
    qp->drawText(270, yCoord, "КАЛЬКУЛЯЦИЯ");
    yCoord+=hTab;
    qp->setFont(QFont("TimesNewRoman", 12));
    QRect rect1 = QRect(xCoord, yCoord, 300, hTab);
    qp->drawRect(rect1);
    qp->drawText(rect1, "Наименование");
    QRect rect2 = QRect(xCoord + 300, yCoord, 100, hTab);
    qp->drawRect(rect2);
    qp->drawText(rect2, "Ед.Изм.");
    QRect rect3 = QRect(xCoord + 400, yCoord, 100, hTab);
    qp->drawRect(rect3);
    qp->drawText(rect3, "Кол-во");
    QRect rect4 = QRect(xCoord + 500, yCoord, 100, hTab);
    qp->drawRect(rect4);
    qp->drawText(rect4, "Цена");
    QRect rect5 = QRect(xCoord + 600, yCoord, 100, hTab);
    qp->drawRect(rect5);
    qp->drawText(rect5, "Сумма");
    yCoord+=hTab;
    for(int i = 0; i<data.count(); ++i){
        QRect rect1 = QRect(xCoord, yCoord, 300, hTab);
        qp->drawRect(rect1);
        qp->drawText(rect1, data.value(i).value(0));
        QRect rect2 = QRect(xCoord + 300, yCoord, 100, hTab);
        qp->drawRect(rect2);
        qp->drawText(rect2, data.value(i).value(1));
        QRect rect3 = QRect(xCoord + 400, yCoord, 100, hTab);
        qp->drawRect(rect3);
        qp->drawText(rect3, data.value(i).value(2));
        QRect rect4 = QRect(xCoord + 500, yCoord, 100, hTab);
        qp->drawRect(rect4);
        qp->drawText(rect4, data.value(i).value(3));
        QRect rect5 = QRect(xCoord + 600, yCoord, 100, hTab);
        qp->drawRect(rect5);
        qp->drawText(rect5, data.value(i).value(4));
        yCoord+=hTab;
    }
}
