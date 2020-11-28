#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QPushButton;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    QList<QPushButton* >& getButtonList();
private:
    QList<QPushButton*> btnList;
private:
    QPushButton* createButton(const QString& btnName);
private slots:
    void showMess();
signals:

};

#endif // MAINWIDGET_H
