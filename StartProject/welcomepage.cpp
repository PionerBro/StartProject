#include "welcomepage.h"
#include "registration.h"

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


#define PASSWORDFILE "password.txt"
#define LOGINFILE "login.txt"

WelcomePage::WelcomePage(QWidget* parent): QWidget(parent)
{
    QLabel* plblName = new QLabel("&Имя:");
    plneName = new QLineEdit;
    plblName->setBuddy(plneName);

    QLabel* plblPass = new QLabel("&Пароль:");
    plnePass = new QLineEdit;
    plnePass->setEchoMode(QLineEdit::Password);
    plblPass->setBuddy(plnePass);

    QPushButton* btnEnter = new QPushButton("&Войти");
    connect(btnEnter, SIGNAL(clicked()), SLOT(userChecked()));

    QPushButton* btnQuit = new QPushButton("&Выход");
    connect(btnQuit,SIGNAL(clicked()), qApp, SLOT(quit()));

    QHBoxLayout* hbx = new QHBoxLayout;
    hbx->addWidget(btnEnter);
    hbx->addWidget(btnQuit);

    QLabel* plblSign = new QLabel("Нет Аккаунта?");
    QPushButton* btnSign = new QPushButton("&Регистрация");
    connect(btnSign, SIGNAL(clicked()), SLOT(regPage()));
    plblSign->setBuddy(btnSign);


    QVBoxLayout* vbx = new QVBoxLayout;
    vbx->addWidget(plblName);
    vbx->addWidget(plneName);
    vbx->addWidget(plblPass);
    vbx->addWidget(plnePass);
    vbx->addLayout(hbx);
    vbx->addWidget(plblSign,0, Qt::AlignHCenter);
    vbx->addWidget(btnSign);


    setLayout(vbx);
}

void WelcomePage::userChecked(){
    QFile f(LOGINFILE);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0, "Ошибка", "Ошибка открытия файла");
        exit(1);
    }
    QTextStream in(&f);
    QString strr = plneName->text();
    for (int pos = 0; !in.atEnd(); ++pos) {
        if(!in.readLine().compare(strr)){
            QFile fp(PASSWORDFILE);
            if(!fp.open(QIODevice::ReadOnly | QIODevice::Text)){
                QMessageBox::information(0, "Ошибка", "Ошибка открытия файла");
                exit(1);
            }
            QTextStream inPass(&fp);
            for(int i = 0; i<pos; ++i){
                inPass.readLine();
            }
            if(!inPass.readLine().compare(plnePass->text())){
                emit setId(strr);
                emit crMainApp();
                return;
            }
        }
    }
    QMessageBox::information(0, "information", "Неверное имя или пароль");
}

void WelcomePage::regPage(){
    wgtReg = new Registration;
    connect(wgtReg, SIGNAL(returntoWelcomePage()), SLOT(showPage()));
    wgtReg->show();
    hide();
}

void WelcomePage::showPage(){
    delete wgtReg;
    show();
}
