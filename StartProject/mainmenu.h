#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    MainMenu(QWidget *parent = 0);
signals:
    void crAddWords();
    void crStartPage();
private slots:
    void toAddWords();
    void toStartPage();

};

#endif // MAINMENU_H
