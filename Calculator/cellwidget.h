#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;

class CellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CellWidget(QWidget *parent = nullptr);
    ~CellWidget();
    int row()const;
    int column()const;
    QPushButton* buttonWidget()const;
public slots:
    void setText(QString text);                     //set m_label`s text:
    void setRow(int);
    void setColumn(int);
private:
    int m_row;                                      //table index.row
    int m_column;                                   //table index.column
    QPushButton* m_btn;
    QLabel* m_label;
signals:
};

#endif // CELLWIDGET_H
