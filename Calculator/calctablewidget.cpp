#include "calctablewidget.h"
#include "cellwidget.h"
#include "mytreemodel.h"
#include "directorywidget.h"

#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>

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

    QTableWidgetItem* wItem = new QTableWidgetItem();
    setItem(rowNum,1,wItem);

    wItem = new QTableWidgetItem();
    setItem(rowNum,2,wItem);

    wItem = new QTableWidgetItem();
    setItem(rowNum,3,wItem);

    wItem = new QTableWidgetItem();
    setItem(rowNum,4,wItem);

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
    cellWidget->setText(data.value(3).toString());

    QTableWidgetItem* item = new QTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setData(Qt::DisplayRole, data.value(4));
    setItem(rowNum,1,item);

    item = new QTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setData(Qt::DisplayRole, QString::number(data.value(5).toDouble(),'f',2));
    setItem(rowNum,3,item);
}

bool CalcTableWidget::event(QEvent* e){
    if(e->type() == QEvent::KeyPress){
        int eKey = static_cast<QKeyEvent*>(e)->key();
        if(eKey == Qt::Key_Return || eKey == Qt::Key_Enter){
            if(currentColumn()==0){
                emit static_cast<CellWidget*>(cellWidget(currentRow(), currentColumn()))->buttonWidget()->clicked();
            }
            editItem(currentItem());

        }else if(eKey == Qt::Key_Delete){
            int row = currentIndex().row();
            if(row > -1){
                removeRow(row);
                emit cellChanged(-1,-1);
            }
        }
    }
    return QTableWidget::event(e);
}

