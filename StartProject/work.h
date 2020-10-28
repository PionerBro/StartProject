#ifndef WORK_H
#define WORK_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class Work : public QWidget
{
    Q_OBJECT
public:
    Work(int pos, QWidget *parent = 0);
private:
    int m_pos;
    QLabel* lbl1[10];
    QLineEdit* lne[10];
    QLabel* lbl2[10];
    QLabel* lbl3[10];
    QPushButton* btnOk;
signals:
    void btStart();
public slots:
    void practice();
private slots:
    void checkResult();
    void backToStart();
};

#endif // WORK_H
