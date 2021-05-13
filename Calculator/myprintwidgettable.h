#ifndef MYPRINTWIDGETTABLE_H
#define MYPRINTWIDGETTABLE_H

#include <QTableWidget>


class QHeaderView;


class MyPrintWidgetTable : public QTableWidget
{
    Q_OBJECT
public:
    MyPrintWidgetTable(QWidget* parent = nullptr);

    void clearBuffers();
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual bool edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) override;
private:
    struct URFuncItem{
        QVariant data;
        int row;
        int column;
    };
    QVector<URFuncItem> undoList;
    QVector<URFuncItem> redoList;
    bool isHandleEdit;                    //true: item edited by handle. false: item edit by undo or redo slots.
    QVariant editBuffer;                  //use with undo and redo items.data.

    QVariant buffer;                      //use with copy and paste functions
    QMenu* menu;
public slots:
    void copySlot();
    void pasteSlot();
    void propertiesSlot();
    void undoSlot();
    void redoSlot();
    void deleteSlot();
    void addRow();
    void addColumn();
private slots:
    void slotCustomContextMenuRequested(QPoint);
    void slotCellChanged(QTableWidgetItem*);
    void setEditable(bool);
};

#endif // MYPRINTWIDGETTABLE_H
