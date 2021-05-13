#include "myprintwidgettable.h"

#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QTableWidgetItem>
#include <QKeyEvent>

#include <QDebug>

MyPrintWidgetTable::MyPrintWidgetTable(QWidget* parent):QTableWidget(100,30,parent), editBuffer(QVariant())
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList headers;
    for (int i=1; i <= 30; ++i) {
        headers<<QString::number(i);
    }

    setContextMenuPolicy(Qt::CustomContextMenu);
    menu = new QMenu(this);

    QAction* copyAct = new QAction(tr("Копировать"),this);
    QAction* pasteAct = new QAction(tr("Вставить"),this);
    QAction* rowInsAct = new QAction(tr("Вставить строку"),this);
    QAction* columnInsAct = new QAction(tr("Вставить столбец"),this);
    QAction* propertiesAct = new QAction(tr("Свойства ячейки"), this);
    copyAct->setShortcut(Qt::CTRL|Qt::Key_C);
    pasteAct->setShortcut(Qt::CTRL|Qt::Key_V);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySlot()));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteSlot()));
    connect(rowInsAct, SIGNAL(triggered()), this, SLOT(addRow()));
    connect(columnInsAct, SIGNAL(triggered()), this, SLOT(addColumn()));
    connect(propertiesAct, SIGNAL(triggered()), this, SLOT(propertiesSlot()));
    menu->addAction(copyAct);
    menu->addAction(pasteAct);
    menu->addAction(rowInsAct);
    menu->addAction(columnInsAct);
    menu->addAction(propertiesAct);
    //QHeaderView* view = new QHeaderView(Qt::Horizontal, this);
    //setHorizontalHeader(view);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(slotCellChanged(QTableWidgetItem*)));
}

void MyPrintWidgetTable::slotCustomContextMenuRequested(QPoint pos){
    menu->popup(viewport()->mapToGlobal(pos));
}

//copy item.data from cell to Table Buffer
void MyPrintWidgetTable::copySlot(){
    if(currentItem())
        buffer = currentItem()->data(Qt::DisplayRole);
    else
        buffer = "";
}

//paste data to item.data from Table Buffer
void MyPrintWidgetTable::pasteSlot(){
    if(!buffer.isNull()){
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setData(Qt::EditRole, buffer);
        setItem(currentRow(),currentColumn(),item);
    }
}

void MyPrintWidgetTable::propertiesSlot(){

}

/* Bind CTRL+Key_C to copySlot();
 * Bind CTRL+Key_V to pasteSlot();
 * Bind CTRL+Key_Z to undoSlot();
 * Bind CTRL+Key_Y to redoSlot();
 * Bind Key_Delete to deleteSlot();
*/
void MyPrintWidgetTable::keyPressEvent(QKeyEvent *event){
    if(event->modifiers() == Qt::ControlModifier && event->key()== Qt::Key_C)
        copySlot();
    else if(event->modifiers() == Qt::ControlModifier && event->key()== Qt::Key_V)
        pasteSlot();
    else if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_Z)
        undoSlot();
    else if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_Y)
        redoSlot();
    else if(event->modifiers() == Qt::NoModifier && event->key()==Qt::Key_Delete)
        deleteSlot();
    QTableWidget::keyPressEvent(event);
}

bool MyPrintWidgetTable::edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event){
    if(index.isValid()){
        isHandleEdit = true;
        editBuffer = index.model()->data(index, Qt::EditRole);
    }

    return QTableWidget::edit(index, trigger, event);
}

void MyPrintWidgetTable::undoSlot(){
    if(!undoList.isEmpty()){
        isHandleEdit = false;
        URFuncItem undoItem = undoList.takeLast();
        URFuncItem redoItem;
        QTableWidgetItem* itemT = item(undoItem.row, undoItem.column);
        if(itemT)
            redoItem.data = itemT->data(Qt::EditRole);
        else
            redoItem.data = QVariant();
        redoItem.row = undoItem.row;
        redoItem.column = undoItem.column;
        redoList<<redoItem;
        itemT = new QTableWidgetItem;
        itemT->setData(Qt::EditRole, undoItem.data);
        setItem(undoItem.row, undoItem.column, itemT);
    }
}

void MyPrintWidgetTable::redoSlot(){
    if(!redoList.isEmpty()){
        isHandleEdit = false;
        URFuncItem redoItem = redoList.takeLast();
        URFuncItem undoItem;
        QTableWidgetItem* itemT = item(redoItem.row, redoItem.column);
        if(itemT)
            undoItem.data = itemT->data(Qt::EditRole);
        else
            undoItem.data = QVariant();
        undoItem.row = redoItem.row;
        undoItem.column = redoItem.column;
        undoList<<undoItem;
        itemT = new QTableWidgetItem;
        itemT->setData(Qt::EditRole, redoItem.data);
        setItem(redoItem.row, redoItem.column, itemT);
    }
}

void MyPrintWidgetTable::deleteSlot(){
    if(currentItem()){
        QTableWidgetItem* item = takeItem(currentRow(),currentColumn());
        URFuncItem undoItem;
        undoItem.data = item->data(Qt::EditRole);
        undoItem.row = currentRow();
        undoItem.column = currentColumn();
        undoList<<undoItem;
        delete item;
    }

}

void MyPrintWidgetTable::addRow(){
    int row = currentRow();
    insertRow(row);
    for (int i = 0; i<undoList.count(); ++i) {
        if(undoList[i].row >= row)
            ++undoList[i].row;
    }
    for (int i = 0; i<redoList.count(); ++i) {
        if(redoList[i].row >= row)
            ++redoList[i].row;
    }
}

void MyPrintWidgetTable::addColumn(){

}

void MyPrintWidgetTable::slotCellChanged(QTableWidgetItem* itemT){
    if(isHandleEdit){
        if(itemT){
                URFuncItem item;
                item.data = editBuffer;
                item.row = itemT->row();
                item.column = itemT->column();
                undoList<<item;
        }
        if(!redoList.isEmpty())
            redoList.clear();
    }
    qDebug()<<"cell changed";
    qDebug()<<undoList.count();
}

void MyPrintWidgetTable::setEditable(bool checked){
    if(!checked)
        setEditTriggers(QAbstractItemView::NoEditTriggers);
    else
        setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::AnyKeyPressed);
}

void MyPrintWidgetTable::clearBuffers(){
    buffer.clear();
    editBuffer.clear();
    undoList.clear();
    redoList.clear();
}
