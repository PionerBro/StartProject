#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class WelcomePage;
class MainMenu;
class Registration;
class Start;
class AddWords;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
private:
    //QWidget* wgt;
    QString m_id;
    WelcomePage* wlcm;
    MainMenu* mnmu;
    AddWords* adws;
    Start* strt;
private slots:
    void createWelcomePage();
    void createMainApp();
    void createAddWordsPage();
    void createExercisePage();
    void createMainAppAdd();
    void createMainAppEx();
    void addId(QString&);
};
#endif // MAINWINDOW_H
