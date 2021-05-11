#ifndef MYPRINTWIDGETTABLE_H
#define MYPRINTWIDGETTABLE_H

#include <QTableWidget>


class QHeaderView;


class MyPrintWidgetTable : public QTableWidget
{
    Q_OBJECT
public:
    MyPrintWidgetTable(QWidget* parent = nullptr);
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual bool edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) override;
private:
    struct URFuncItem{
        QVariant data;
        int row;
        int column;
    };
    QList<URFuncItem> undoList;
    QList<URFuncItem> redoList;
    bool isURF;

    QVariant buffer;
    QMenu* menu;
public slots:
    void copySlot();
    void pasteSlot();
    void propertiesSlot();
    void undoSlot();
private slots:
    void slotCustomContextMenuRequested(QPoint);
};

#endif // MYPRINTWIDGETTABLE_H
