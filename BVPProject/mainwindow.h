#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainJournal;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    /*void slotNewFile();
    void slotReadFile();
    void slotUpdateFile();
    void slotShowMainJournal();
    void slotShowHelp();
    */
};
#endif // MAINWINDOW_H
