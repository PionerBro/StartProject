#ifndef ADDWORDS_H
#define ADDWORDS_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QFile;


class AddWords : public QWidget
{
    Q_OBJECT
public:
    AddWords(QWidget *parent = 0);
private:
    QLineEdit* plneEngWord;
    QLineEdit* plneRusWord;
    QLabel* plblInfo;
signals:
    void returnToMenu();
private slots:
    void addWords();
    void retToMenu();
};

#endif // ADDWORDS_H
