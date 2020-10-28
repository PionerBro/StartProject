#ifndef START_H
#define START_H

#include <QWidget>

class QLabel;

class Start : public QWidget
{
    Q_OBJECT
public:
    Start(QString id, QWidget *parent = 0);
private:
    QString m_id;
    int pos;
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
