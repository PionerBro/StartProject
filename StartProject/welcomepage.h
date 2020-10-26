#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>

class Registration;
class QLineEdit;

class WelcomePage:public QWidget
{
    Q_OBJECT
public:
    WelcomePage(QWidget* parent = 0);
signals:
    void crMainApp();
    void retPage();
    //void crRegApp();

private slots:
    void userChecked();
    void regPage();
    void showPage();
private:
    Registration* wgtReg;
    QLineEdit* plneName;
    QLineEdit* plnePass;

};

#endif // WELCOMEPAGE_H
