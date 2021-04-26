#ifndef CALCTABLEWIDGET_H
#define CALCTABLEWIDGET_H

#include <QTableWidget>

class CalcTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    CalcTableWidget(QWidget* parent = nullptr);
    CalcTableWidget(int rows, int columns, QWidget* parent = nullptr);

protected:
    virtual bool event(QEvent* e) override;
public slots:
    void addNewRow();
    void cellWidgetButtonClicked();
    void setRowData(QList<QVariant>&);
};

#endif // CALCTABLEWIDGET_H
