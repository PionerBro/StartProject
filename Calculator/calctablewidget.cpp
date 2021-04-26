#include "calctablewidget.h"
#include "cellwidget.h"
#include "mytreemodel.h"
#include "directorywidget.h"

#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>

#include <QDebug>

CalcTableWidget::CalcTableWidget(QWidget* parent):QTableWidget(parent)
{
}

CalcTableWidget::CalcTableWidget(int rows, int columns, QWidget* parent):QTableWidget(rows,columns,parent){
}

void CalcTableWidget::addNewRow(){
    /*create and set cell widget*/
    int rowNum = rowCount();
    CellWidget* cellWidget = new CellWidget(this);
    connect(cellWidget->buttonWidget(), SIGNAL(clicked()), this, SLOT(cellWidgetButtonClicked()));
    cellWidget->setRow(rowNum);
    cellWidget->setColumn(0);

    insertRow(rowNum);                             //insert row to the end
    setCellWidget(rowNum,0, cellWidget);           //set cell widget in position
    setRowHeight(rowNum,25);
    cellWidget->setText(QString("%1").arg(rowNum));
    //increment table rows
    emit cellWidget->buttonWidget()->clicked();
}

void CalcTableWidget::cellWidgetButtonClicked(){
    QPushButton* sender = static_cast<QPushButton*>(QObject::sender());         //get button object, which clicked
    setCurrentCell(static_cast<CellWidget*>(sender->parent())->row(), 0);       //setCurRow(static_cast<CellWidget*>(sender->parent())->row());               //set current row variable
    DirectoryWidget* dirWidget = new DirectoryWidget(sender);                   //create numenclature Dialog widget
    connect(dirWidget->getModel(), SIGNAL(sendData(QList<QVariant>&)), this, SLOT(setRowData(QList<QVariant>&))); //when element is chosen emit signal with element's data

    if(dirWidget->exec()){
        qDebug()<<"good";
    }else{
        qDebug()<<"bad";

    }
}

void CalcTableWidget::setRowData(QList<QVariant>& data){
    /*create and set cell widget*/
    int rowNum = currentRow();

    qDebug()<<data<<" "<<rowNum;
    CellWidget* cellWidget = new CellWidget(this);
    connect(cellWidget->buttonWidget(), SIGNAL(clicked()), this, SLOT(cellWidgetButtonClicked()));
    cellWidget->setRow(rowNum);
    cellWidget->setColumn(0);
    setCellWidget(rowNum, 0, cellWidget);         //set cell widget in current position
    cellWidget->setText(data.value(1).toString());

    QTableWidgetItem* item = new QTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setData(Qt::DisplayRole, data.value(2));
    setItem(rowNum,1,item);

    item = new QTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setData(Qt::DisplayRole, data.value(0));
    setItem(rowNum,3,item);
}

bool CalcTableWidget::event(QEvent* e){
    if(e->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(e)->key() == Qt::Key_Return){
        qDebug()<<"Enter";
        emit doubleClicked(currentIndex());
    }
    return QTableWidget::event(e);
}
