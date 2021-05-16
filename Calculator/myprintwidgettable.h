#ifndef MYPRINTWIDGETTABLE_H
#define MYPRINTWIDGETTABLE_H

#include <QTableWidget>


class QHeaderView;


class MyPrintWidgetTable : public QTableWidget
{
    Q_OBJECT
public:
    enum URFType{
        Edit,
        InsertRow,
        InsertColumn,
        DeleteRow,
        DeleteColumn
    };
    Q_ENUM(URFType)

    MyPrintWidgetTable(QWidget* parent = nullptr);

    QPoint getLastEditRow() const;
    QPoint getLastEditColumn() const;
    void clearBuffers();
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual bool edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) override;
private:

    struct URFuncItem{

        URFType type;
        QVariant data;
        QPoint lEditRow;
        QPoint lEditColumn;
        int row;
        int column;
    };
    QVector<URFuncItem> undoList;
    QVector<URFuncItem> redoList;
    bool isHandleEdit;                    //true: item edited by handle. false: item edit by undo or redo slots.
    bool isInsDelOp;
    QVariant editBuffer;                  //use with undo and redo items.data.

    QPoint lastEditColumn;
    QPoint lastEditRow;

    QVariant buffer;                      //use with copy and paste functions
    QMenu* menu;
private:
    void deleteRow(int);
    void deleteColumn(int);
    void addRow(int);
    void addColumn(int);
    void deleteItem(int, int);
public slots:
    void copySlot();
    void pasteSlot();
    void propertiesSlot();
    void undoSlot();
    void redoSlot();
    void deleteSlot();
    void addRowSlot();
    void addColumnSlot();
    void deleteRowSlot();
    void deleteColumnSlot();
private slots:
    void slotCustomContextMenuRequested(QPoint);
    void slotCellChanged(QTableWidgetItem*);
    void setEditable(bool);
};

#endif // MYPRINTWIDGETTABLE_H
