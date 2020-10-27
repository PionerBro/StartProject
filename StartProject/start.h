#ifndef START_H
#define START_H

#include <QWidget>

class QLabel;

class Start : public QWidget
{
    Q_OBJECT
public:
    Start(QWidget *parent = 0, int num = 0);
private:
    int p_num;
    int m_num;
    QLabel* plblEng;
    QLabel* plblRus;
signals:
    void crMainMenu();
private slots:
    void prevPage();
    void nextPage();
    void toMenu();
};

#endif // START_H
