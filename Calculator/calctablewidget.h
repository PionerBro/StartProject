#ifndef CALCTABLEWIDGET_H
#define CALCTABLEWIDGET_H

#include <QTableWidget>

class CalcTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    CalcTableWidget(QWidget* parent = nullptr);
    CalcTableWidget(int rows, int columns, QWidget* parent = nullptr);
    qlonglong getItemNum(int pos)const;
    void setDataAtIndex(int row, int col, const QVariant& data);
protected:
    virtual bool event(QEvent* e) override;
private:
    QVector<qlonglong> itemNums;
public slots:
    void addNewRow();
    void cellWidgetButtonClicked();
    void setRowData(QList<QVariant>&);
};

#endif // CALCTABLEWIDGET_H
