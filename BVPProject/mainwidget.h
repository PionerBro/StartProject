#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    QList<QPushButton* >& getButtonList();
private:
    QWidget* start_wgt;
    QHBoxLayout* m_hbx;
    int wgtNum = -1;
    QList<QPushButton*> btnList;
    QList<QWidget*> wgtList;
private:
    QPushButton* createButton(const QString& btnName);
private slots:
    void btn0_clicked();
    void btn1_clicked();
    void btn2_clicked();
signals:

};

#endif // MAINWIDGET_H
