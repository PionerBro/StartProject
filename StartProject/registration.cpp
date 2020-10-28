#include "registration.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <stdio.h>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#define PASSWORDFILE "password.txt"
#define LOGINFILE "login.txt"

Registration::Registration(QWidget* parent):QWidget(parent)
{
    QLabel* plblLogin = new QLabel("&Введите логин:");
    plneLogin = new QLineEdit;
    plblLogin->setBuddy(plneLogin);

    QLabel* plblPassword = new QLabel("Введите пароль");
    plnePassword = new QLineEdit;
    plnePassword->setEchoMode(QLineEdit::Password);
    plblPassword->setBuddy(plnePassword);

    QLabel* plblPasswordRep = new QLabel("Повторите пароль");
    plnePasswordRep = new QLineEdit;
    plnePasswordRep->setEchoMode(QLineEdit::Password);
    plblPasswordRep->setBuddy(plnePasswordRep);

    plblWarning = new QLabel;

    QPushButton* btnReg = new QPushButton("Регистрация");
    btnReg->setMinimumSize(150,20);
    connect(btnReg, SIGNAL(clicked()), SLOT(createUser()));

    QPushButton* btnCancel = new QPushButton("Отмена");
    btnCancel->setMinimumSize(150,20);
    connect(btnCancel, SIGNAL(clicked()), SLOT(retPage()));
    connect(btnCancel, SIGNAL(clicked()), SLOT(close()));

    QHBoxLayout* hbx = new QHBoxLayout;
    hbx->setContentsMargins(0,20,0,0);
    hbx->addWidget(btnReg);
    hbx->addWidget(btnCancel);

    QVBoxLayout* vbx = new QVBoxLayout;
    vbx->addWidget(plblLogin);
    vbx->addWidget(plneLogin);
    vbx->addWidget(plblPassword);
    vbx->addWidget(plnePassword);
    vbx->addWidget(plblPasswordRep);
    vbx->addWidget(plnePasswordRep);
    vbx->addWidget(plblWarning);
    vbx->addLayout(hbx);

    setLayout(vbx);

}

void Registration::createUser(){
    QString login = plneLogin->text();
    int count = login.size();
    if(count > 15 || count == 0){
        plblWarning->setText("Логин должен иметь длину 1-15 символов");
        return;
    }
    if (login.contains(" ")){
        plblWarning->setText("Данные не должны содержать пробелов");
        return;
    }

    QString password = plnePassword->text();
    QString password2 = plnePasswordRep->text();
    if (password.compare(password2)){
            plblWarning->setText("Пароли должны совпадать");
            return;
        }
    if (password.size() > 15){
        plblWarning->setText("Слишком длинный пароль");
        return;
    }
    if (password.contains(" ")){
        plblWarning->setText("Данные не должны содержать пробелов");
        return;
    }

    QFile f(LOGINFILE);
    if(!f.open(QIODevice::ReadWrite | QIODevice::Text)){
        plblWarning->setText("ошибка открытия файла");
        return;
    }
    QString line;
    QTextStream in(&f);
    while(!in.atEnd()){
        line = in.readLine();
        if(!line.compare(login)){
            plblWarning->setText("Такой логин существует");
            return;
        }
    }
    QTextStream out(&f);
    out<<login<<"\n";
    f.close();

    f.setFileName(PASSWORDFILE);
    if(!f.open(QIODevice::Append | QIODevice::Text)){
        plblWarning->setText("ошибка открытия файла");
        return;
    }
    out<<password<<"\n";
    f.close();

    f.setFileName(login+".txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        plblWarning->setText("ошибка открытия файла");
        return;
    }
    out<<0<<"\n";
    f.close();


    QMessageBox::information(0, "Information", "Аккаунт успешно зарегистрирован");
    emit returntoWelcomePage();
    //hide();

}

void Registration::retPage(){
    emit returntoWelcomePage();
}
