#include "printwidget.h"

#include <QPainter>
#include <QPushButton>
#include <QPrintDialog>
#include <QPrinter>

#include <QDebug>

PrintWidget::PrintWidget(const QList<QStringList>& p_data, QWidget *parent) : QDialog(parent), data(p_data)
{
    resize(740, 700);
    setAttribute(Qt::WA_DeleteOnClose);
}

void PrintWidget::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
   QPainter qp(this);
   drawDocum(&qp);
}

void PrintWidget::drawDocum(QPainter* qp){
    qp->setPen(QPen(Qt::black, 2));
    QFont textFont = QFont("TimesNewRoman", 10);
    qp->setFont(textFont);
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
    textFont = QFont("TimesNewRoman", 14);
    qp->setFont(textFont);
    qp->drawText(270, yCoord, "КАЛЬКУЛЯЦИЯ");
    yCoord+=hTab;
    qp->drawText(xCoord, yCoord, data.value(0).value(0));
    QFontMetrics fm(textFont);
    int textSize = fm.horizontalAdvance(data.value(0).value(2));
    qp->drawText(width+xCoord - textSize, yCoord, data.value(0).value(2));
    yCoord+=hTab;
    qp->drawText(xCoord, yCoord, "Выход: " + data.value(0).value(1) + " кг.");
    yCoord+=hTab;
    textFont = QFont("TimesNewRoman", 12);
    qp->setFont(textFont);
    QRect rect1 = QRect(xCoord, yCoord, 300, hTab);
    qp->drawRect(rect1);
    rect1.setX(rect1.x()+2);
    rect1.setY(rect1.y()+3);
    qp->drawText(rect1, "Наименование");
    rect1 = QRect(xCoord + 300, yCoord, 100, hTab);
    qp->drawRect(rect1);
    rect1.setX(rect1.x()+2);
    rect1.setY(rect1.y()+3);
    qp->drawText(rect1, "Ед.Изм.");
    rect1 = QRect(xCoord + 400, yCoord, 100, hTab);
    qp->drawRect(rect1);
    rect1.setX(rect1.x()+2);
    rect1.setY(rect1.y()+3);
    qp->drawText(rect1, "Кол-во");
    rect1 = QRect(xCoord + 500, yCoord, 100, hTab);
    qp->drawRect(rect1);
    rect1.setX(rect1.x()+2);
    rect1.setY(rect1.y()+3);
    qp->drawText(rect1, "Цена");
    rect1 = QRect(xCoord + 600, yCoord, 100, hTab);
    qp->drawRect(rect1);
    rect1.setX(rect1.x()+2);
    rect1.setY(rect1.y()+3);
    qp->drawText(rect1, "Сумма");
    yCoord+=hTab;
    for(int i = 1; i<data.count(); ++i){
        QRect rect1 = QRect(xCoord, yCoord, 300, hTab);
        qp->drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp->drawText(rect1, data.value(i).value(0));
        rect1 = QRect(xCoord + 300, yCoord, 100, hTab);
        qp->drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp->drawText(rect1, data.value(i).value(1));
        rect1 = QRect(xCoord + 400, yCoord, 100, hTab);
        qp->drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp->drawText(rect1, data.value(i).value(2));
        rect1 = QRect(xCoord + 500, yCoord, 100, hTab);
        qp->drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp->drawText(rect1, data.value(i).value(3));
        rect1 = QRect(xCoord + 600, yCoord, 100, hTab);
        qp->drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp->drawText(rect1, data.value(i).value(4));
        yCoord+=hTab;
    }
    rect1 = QRect(xCoord+600, yCoord, 100, hTab);
    qp->drawRect(rect1);
    rect1.setX(rect1.x()+2);
    rect1.setY(rect1.y()+3);
    qp->drawText(rect1, data.value(0).value(3));
    yCoord+=2*hTab;
    textFont = QFont("TimesNewRoman", 14);
    qp->setFont(textFont);
    fm = QFontMetrics(textFont);
    QString text = "Цена за порцию " + data.value(0).value(4) + "г. = " + data.value(0).value(5) + "р.";
    textSize = fm.horizontalAdvance(text);
    qp->drawText(xCoord+width - textSize, yCoord, text);
    QPushButton* printBtn = new QPushButton(tr("Печать"),this);
    printBtn->setGeometry(0,0,80,20);
    printBtn->show();
    connect(printBtn, SIGNAL(clicked()), this, SLOT(printText()));
}

void PrintWidget::printText(){
    QPrinter qPrinter;

    QPrintDialog dlg(&qPrinter, this);
    if(dlg.exec()){
        QPainter qp(&qPrinter);
        qp.setPen(QPen(Qt::black, 2));
        QFont textFont = QFont("TimesNewRoman", 10);
        qp.setFont(textFont);
        qDebug()<<"Paint";
        int xCoord = 20;
        int yCoord = 30;
        int width = 700;
        int hTab = 25;
        qp.drawText(500, yCoord, "УТВЕРЖДАЮ");
        yCoord+=hTab;
        qp.drawText(450, yCoord, "Директор ЧПТУП \"В.П.Бутковский\"");
        yCoord+=hTab;
        qp.drawText(450, yCoord, "_______________Бутквоский В.П.");
        yCoord+=2*hTab;
        textFont = QFont("TimesNewRoman", 14);
        qp.setFont(textFont);
        qp.drawText(270, yCoord, "КАЛЬКУЛЯЦИЯ");
        yCoord+=hTab;
        qp.drawText(xCoord, yCoord, data.value(0).value(0));
        QFontMetrics fm(textFont);
        int textSize = fm.horizontalAdvance(data.value(0).value(2));
        qp.drawText(width+xCoord - textSize, yCoord, data.value(0).value(2));
        yCoord+=hTab;
        qp.drawText(xCoord, yCoord, "Выход: " + data.value(0).value(1) + " кг.");
        yCoord+=hTab;
        textFont = QFont("TimesNewRoman", 12);
        qp.setFont(textFont);
        QRect rect1 = QRect(xCoord, yCoord, 300, hTab);
        qp.drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp.drawText(rect1, "Наименование");
        rect1 = QRect(xCoord + 300, yCoord, 100, hTab);
        qp.drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp.drawText(rect1, "Ед.Изм.");
        rect1 = QRect(xCoord + 400, yCoord, 100, hTab);
        qp.drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp.drawText(rect1, "Кол-во");
        rect1 = QRect(xCoord + 500, yCoord, 100, hTab);
        qp.drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp.drawText(rect1, "Цена");
        rect1 = QRect(xCoord + 600, yCoord, 100, hTab);
        qp.drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp.drawText(rect1, "Сумма");
        yCoord+=hTab;
        for(int i = 1; i<data.count(); ++i){
            QRect rect1 = QRect(xCoord, yCoord, 300, hTab);
            qp.drawRect(rect1);
            rect1.setX(rect1.x()+2);
            rect1.setY(rect1.y()+3);
            qp.drawText(rect1, data.value(i).value(0));
            rect1 = QRect(xCoord + 300, yCoord, 100, hTab);
            qp.drawRect(rect1);
            rect1.setX(rect1.x()+2);
            rect1.setY(rect1.y()+3);
            qp.drawText(rect1, data.value(i).value(1));
            rect1 = QRect(xCoord + 400, yCoord, 100, hTab);
            qp.drawRect(rect1);
            rect1.setX(rect1.x()+2);
            rect1.setY(rect1.y()+3);
            qp.drawText(rect1, data.value(i).value(2));
            rect1 = QRect(xCoord + 500, yCoord, 100, hTab);
            qp.drawRect(rect1);
            rect1.setX(rect1.x()+2);
            rect1.setY(rect1.y()+3);
            qp.drawText(rect1, data.value(i).value(3));
            rect1 = QRect(xCoord + 600, yCoord, 100, hTab);
            qp.drawRect(rect1);
            rect1.setX(rect1.x()+2);
            rect1.setY(rect1.y()+3);
            qp.drawText(rect1, data.value(i).value(4));
            yCoord+=hTab;
        }
        rect1 = QRect(xCoord+600, yCoord, 100, hTab);
        qp.drawRect(rect1);
        rect1.setX(rect1.x()+2);
        rect1.setY(rect1.y()+3);
        qp.drawText(rect1, data.value(0).value(3));
        yCoord+=2*hTab;
        textFont = QFont("TimesNewRoman", 14);
        qp.setFont(textFont);
        fm = QFontMetrics(textFont);
        QString text = "Цена за порцию " + data.value(0).value(4) + "г. = " + data.value(0).value(5) + "р.";
        textSize = fm.horizontalAdvance(text);
        qp.drawText(xCoord+width - textSize, yCoord, text);
    }
}
