#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class WelcomePage;
class Menu;
class Registration;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);


private:
    QWidget* wgt;

private slots:
    void createWelcomePage();
    void createMainApp();
   // void createRegApp();
};
#endif // MAINWINDOW_H
