#include "myprintwidgettable.h"

#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QTableWidgetItem>
#include <QKeyEvent>

#include <QDebug>

MyPrintWidgetTable::MyPrintWidgetTable(QWidget* parent):QTableWidget(100,30,parent), isURF(false)
{
    QStringList headers;
    for (int i=1; i <= 30; ++i) {
        headers<<QString::number(i);
    }
    setContextMenuPolicy(Qt::CustomContextMenu);
    menu = new QMenu(this);

    QAction* copyAct = new QAction(tr("Копировать"),this);
    QAction* pasteAct = new QAction(tr("Вставить"),this);
    QAction* propertiesAct = new QAction(tr("Свойства ячейки"), this);
    copyAct->setShortcut(Qt::CTRL+Qt::Key_C);
    pasteAct->setShortcut(Qt::CTRL+Qt::Key_V);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySlot()));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteSlot()));
    connect(propertiesAct, SIGNAL(triggered()), this, SLOT(propertiesSlot()));
    menu->addAction(copyAct);
    menu->addAction(pasteAct);
    menu->addAction(propertiesAct);
    //QHeaderView* view = new QHeaderView(Qt::Horizontal, this);
    //setHorizontalHeader(view);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
}

void MyPrintWidgetTable::slotCustomContextMenuRequested(QPoint pos){
    menu->popup(viewport()->mapToGlobal(pos));
}

void MyPrintWidgetTable::copySlot(){
    if(currentItem())
        buffer = currentItem()->data(Qt::DisplayRole);
    else
        buffer = "";
}

void MyPrintWidgetTable::pasteSlot(){
    if(!buffer.isNull()){
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setData(Qt::EditRole, buffer);
        setItem(currentRow(),currentColumn(),item);
    }
}

void MyPrintWidgetTable::propertiesSlot(){

}

void MyPrintWidgetTable::keyPressEvent(QKeyEvent *event){
    if(event->modifiers() == Qt::ControlModifier && event->key()== Qt::Key_C)
        copySlot();
    else if(event->modifiers() == Qt::ControlModifier && event->key()== Qt::Key_V)
        pasteSlot();
    else if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_Z)
        undoSlot();
    QTableWidget::keyPressEvent(event);
}

bool MyPrintWidgetTable::edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event){
    qDebug()<<"edit";
    if(index.isValid()){
        URFuncItem item;
        item.data = index.model()->data(index, Qt::EditRole);
        item.row = index.row();
        item.column = index.column();
        undoList<<item;
    }
    return QTableWidget::edit(index, trigger, event);
}

void MyPrintWidgetTable::undoSlot(){
    isURF = true;
    if(!undoList.isEmpty()){
        URFuncItem itemURF;
        QTableWidgetItem* item = currentItem();
        if(item)
            itemURF.data = item->data(Qt::EditRole);
        else
            itemURF.data = QVariant();
        itemURF.row = currentRow();
        itemURF.column = currentColumn();
        redoList<<itemURF;
        item = new QTableWidgetItem;
        itemURF = undoList.takeLast();
        item->setData(Qt::EditRole, itemURF.data);
        setItem(itemURF.row, itemURF.column, item);
    }
    isURF = false;
}
