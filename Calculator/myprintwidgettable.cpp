#include "myprintwidgettable.h"

#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QTableWidgetItem>
#include <QKeyEvent>

#include <QDebug>

MyPrintWidgetTable::MyPrintWidgetTable(QWidget* parent):QTableWidget(100,30,parent), editBuffer(QVariant()), lastEditColumn(-1,-1), lastEditRow(-1,-1)
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
    QAction* rowDelAct = new QAction(tr("Удалить строку"),this);
    QAction* columnInsAct = new QAction(tr("Вставить столбец"),this);
    QAction* columnDelAct = new QAction(tr("Удалить столбец"), this);
    QAction* propertiesAct = new QAction(tr("Свойства ячейки"), this);
    copyAct->setShortcut(Qt::CTRL|Qt::Key_C);
    pasteAct->setShortcut(Qt::CTRL|Qt::Key_V);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySlot()));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteSlot()));
    connect(rowInsAct, SIGNAL(triggered()), this, SLOT(addRowSlot()));
    connect(columnInsAct, SIGNAL(triggered()), this, SLOT(addColumnSlot()));
    connect(rowDelAct, SIGNAL(triggered()), this, SLOT(deleteRowSlot()));
    connect(columnDelAct, SIGNAL(triggered()), this, SLOT(deleteColumnSlot()));
    connect(propertiesAct, SIGNAL(triggered()), this, SLOT(propertiesSlot()));
    menu->addAction(copyAct);
    menu->addAction(pasteAct);
    menu->addAction(rowInsAct);
    menu->addAction(rowDelAct);
    menu->addAction(columnInsAct);
    menu->addAction(columnDelAct);
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
        URFuncItem undoItem = undoList.takeLast();
        URFuncItem redoItem;
        if(undoItem.type == URFType::Edit){
            isHandleEdit = false;
            QTableWidgetItem* itemT = item(undoItem.row, undoItem.column);
            redoItem.data = (itemT) ? itemT->data(Qt::EditRole) : QVariant();
            redoItem.type = undoItem.type;
            redoItem.row = undoItem.row;
            redoItem.column = undoItem.column;
            redoItem.lEditRow = undoItem.lEditRow;
            redoItem.lEditColumn = undoItem.lEditColumn;
            if(undoItem.data==QVariant()){
                deleteItem(undoItem.row, undoItem.column);
            }else{
                itemT = new QTableWidgetItem;
                itemT->setData(Qt::EditRole, undoItem.data);
                setItem(undoItem.row, undoItem.column, itemT);
            }
            //if(undoItem.lEditRow == lastEditRow)
              //  lastEditRow = (undoList.isEmpty() ? QPoint(-1,-1) : undoList.last().lEditRow);
            //if(undoItem.lEditColumn == lastEditColumn)
              //  lastEditColumn = (undoList.isEmpty() ? QPoint(-1,-1) : undoList.last().lEditColumn);
        }else if(undoItem.type == URFType::InsertRow){
            redoItem.row = undoItem.row;
            redoItem.type = URFType::DeleteRow;
            deleteRow(undoItem.row);
        }else if (undoItem.type == URFType::InsertColumn) {
            redoItem.column = undoItem.column;
            redoItem.type = URFType::DeleteColumn;
            deleteColumn(undoItem.column);
        }else if(undoItem.type == URFType::DeleteRow){
            redoItem.row = undoItem.row;
            redoItem.type = URFType::InsertRow;
            addRow(undoItem.row);
        }else if(undoItem.type == URFType::DeleteColumn){
            redoItem.column = undoItem.column;
            redoItem.type = URFType::InsertColumn;
            addColumn(undoItem.column);
        }
        redoList<<redoItem;
    }
}

void MyPrintWidgetTable::redoSlot(){
    if(!redoList.isEmpty()){
        URFuncItem redoItem = redoList.takeLast();
        URFuncItem undoItem;
        if(redoItem.type == URFType::Edit){
            isHandleEdit = false;
            QTableWidgetItem* itemT = item(redoItem.row, redoItem.column);
            undoItem.data = (itemT) ? itemT->data(Qt::EditRole) : QVariant();
            undoItem.type = redoItem.type;
            undoItem.row = redoItem.row;
            undoItem.column = redoItem.column;
            undoItem.lEditRow = lastEditRow;
            undoItem.lEditColumn = lastEditColumn;
            if(redoItem.data == QVariant()){
                deleteItem(redoItem.row, redoItem.column);
            }else{
                itemT = new QTableWidgetItem;
                itemT->setData(Qt::EditRole, redoItem.data);
                setItem(redoItem.row, redoItem.column, itemT);
            }
        }else if(redoItem.type == URFType::InsertRow){
            undoItem.row = redoItem.row;
            undoItem.type = URFType::DeleteRow;
            deleteRow(redoItem.row);
        }else if (redoItem.type == URFType::InsertColumn) {
            undoItem.column = redoItem.column;
            undoItem.type = URFType::DeleteColumn;
            deleteColumn(redoItem.column);
        }else if(redoItem.type == URFType::DeleteRow){
            undoItem.row = redoItem.row;
            undoItem.type = URFType::InsertRow;
            addRow(redoItem.row);
        }else if(redoItem.type == URFType::DeleteColumn){
            undoItem.column = redoItem.column;
            undoItem.type = URFType::InsertColumn;
            addColumn(redoItem.column);
        }
        undoList<<undoItem;
    }
}

void MyPrintWidgetTable::deleteItem(int cRow, int cColumn){
    QTableWidgetItem* item = takeItem(cRow, cColumn);
    delete item;
    if(lastEditRow == QPoint(cRow, cColumn)){
        bool isFind = false;
        for(int row = lastEditRow.x(); row >= 0; --row){
            for(int column = lastEditColumn.y(); column >= 0; --column){
                if(this->item(row, column)){
                    lastEditRow = QPoint(row, column);
                    isFind = true;
                    break;
                }
            }
            if(isFind)
                break;
        }
        if(!isFind)
            lastEditRow = QPoint(-1, -1);
    }
    if(lastEditColumn == QPoint(cRow, cColumn)){
        bool isFind = false;
        for (int column = lastEditColumn.y(); column >= 0; --column) {
            for(int row = lastEditRow.x(); row >=0; --row){
                if(this->item(row,column)){
                    lastEditColumn = QPoint(row, column);
                    isFind = true;
                    break;
                }
            }
            if(isFind)
                break;
        }
        if(!isFind)
            lastEditColumn = QPoint(-1, -1);
    }
}

void MyPrintWidgetTable::deleteSlot(){
    int cRow = currentRow();
    int cColumn = currentColumn();
    QTableWidgetItem* item = this->item(cRow,cColumn);
    if(currentItem()){
        URFuncItem undoItem;
        undoItem.data = item->data(Qt::EditRole);
        undoItem.row = cRow;
        undoItem.column = cColumn;
        undoItem.type = URFType::Edit;
        undoItem.lEditRow = lastEditRow;
        undoItem.lEditColumn = lastEditColumn;
        undoList<<undoItem;
        deleteItem(cRow,cColumn);
    }
}

void MyPrintWidgetTable::addRow(int row){
    insertRow(row);
/*    for (int i = 0; i<undoList.count(); ++i) {
        if(undoList[i].row > row)
            ++undoList[i].row;
    }
    for (int i = 0; i<redoList.count(); ++i) {
        if(redoList[i].row > row)
            ++redoList[i].row;
    }*/
    if(lastEditRow.x() >= row)
        ++lastEditRow.rx();
}

void MyPrintWidgetTable::addRowSlot(){
    int row = currentRow();
    addRow(row);
    URFuncItem item;
    item.type = URFType::InsertRow;
    item.row = row;
    undoList<<item;
    if(!redoList.isEmpty())
        redoList.clear();
}

void MyPrintWidgetTable::deleteRow(int row){
    removeRow(row);
/*    for (int i = 0; i<undoList.count(); ++i) {
        if(undoList[i].row > row)
            --undoList[i].row;
    }
    for (int i = 0; i<redoList.count(); ++i) {
        if(redoList[i].row > row)
            --redoList[i].row;
    }*/
    if(lastEditRow.x() > row)
        --lastEditRow.rx();
    else if(lastEditRow.x() == row){
        bool isFind = false;
        for(int row = lastEditRow.x(); row >= 0; --row){
            for(int column = lastEditColumn.y(); column >= 0; --column){
                if(this->item(row, column)){
                    lastEditRow = QPoint(row, column);
                    isFind = true;
                    break;
                }
            }
            if(isFind)
                break;
        }
        if(!isFind)
            lastEditRow = QPoint(-1, -1);
    }
    if(lastEditColumn.x() == row){
        bool isFind = false;
        for (int column = lastEditColumn.y(); column >= 0; --column) {
            for(int row = lastEditRow.x(); row >=0; --row){
                if(this->item(row,column)){
                    lastEditColumn = QPoint(row, column);
                    isFind = true;
                    break;
                }
            }
            if(isFind)
                break;
        }
        if(!isFind)
            lastEditColumn = QPoint(-1, -1);
    }
}

void MyPrintWidgetTable::deleteRowSlot(){
    int row = currentRow();
    deleteRow(row);
    URFuncItem item;
    item.type = URFType::DeleteRow;
    item.row = row;
    undoList<<item;
    if(!redoList.isEmpty())
        redoList.clear();
}

void MyPrintWidgetTable::addColumn(int column){
    insertColumn(column);
/*    for (int i = 0; i<undoList.count(); ++i) {
        if(undoList[i].column > column)
            ++undoList[i].column;
    }
    for (int i = 0; i<redoList.count(); ++i) {
        if(redoList[i].column > column)
            ++redoList[i].column;
    }*/
    if(lastEditColumn.y() >= column)
        ++lastEditColumn.ry();
}

void MyPrintWidgetTable::addColumnSlot(){
    int column = currentColumn();
    addColumn(column);
    URFuncItem item;
    item.type = URFType::InsertColumn;
    item.column = column;
    undoList<<item;
    if(!redoList.isEmpty())
        redoList.clear();
}

void MyPrintWidgetTable::deleteColumn(int column){
    removeColumn(column);
/*    for (int i = 0; i<undoList.count(); ++i) {
        if(undoList[i].column > column)
            --undoList[i].column;
    }
    for (int i = 0; i<redoList.count(); ++i) {
        if(redoList[i].column > column)
            --redoList[i].column;
    }*/
    if(lastEditColumn.y() > column)
        --lastEditColumn.ry();
    else if(lastEditColumn.y() == column){
        bool isFind = false;
        for (int column = lastEditColumn.y(); column >= 0; --column) {
            for(int row = lastEditRow.x(); row >=0; --row){
                if(this->item(row,column)){
                    lastEditColumn = QPoint(row, column);
                    isFind = true;
                    break;
                }
            }
            if(isFind)
                break;
        }
        if(!isFind)
            lastEditColumn = QPoint(-1, -1);
    }
    if(lastEditRow.y() == column){
        bool isFind = false;
        for(int row = lastEditRow.x(); row >= 0; --row){
            for(int column = lastEditColumn.y(); column >= 0; --column){
                if(this->item(row, column)){
                    lastEditRow = QPoint(row, column);
                    isFind = true;
                    break;
                }
            }
            if(isFind)
                break;
        }
        if(!isFind)
            lastEditRow = QPoint(-1, -1);
    }
}

void MyPrintWidgetTable::deleteColumnSlot(){
    int column = currentColumn();
    deleteColumn(column);
    URFuncItem item;
    item.type = URFType::DeleteColumn;
    item.column = column;
    undoList<<item;
    if(!redoList.isEmpty())
        redoList.clear();
}

void MyPrintWidgetTable::slotCellChanged(QTableWidgetItem* itemT){
    if(itemT){
        lastEditRow = (itemT->row() > lastEditRow.x() ? QPoint(itemT->row(),itemT->column()) : lastEditRow);
        lastEditColumn = (itemT->column() > lastEditColumn.y() ? QPoint(itemT->row(), itemT->column()) : lastEditColumn);
        if(isHandleEdit){
                URFuncItem item;
                item.data = editBuffer;
                item.row = itemT->row();
                item.column = itemT->column();
                item.type = URFType::Edit;
                item.lEditRow = lastEditRow;
                item.lEditColumn = lastEditColumn;
                undoList<<item;
                if(!redoList.isEmpty())
                    redoList.clear();
        }
    }
    qDebug()<<"cell changed";
    qDebug()<<lastEditRow<<lastEditColumn;

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

QPoint MyPrintWidgetTable::getLastEditRow() const{
    return lastEditRow;
}

QPoint MyPrintWidgetTable::getLastEditColumn() const{
    return lastEditColumn;
}
