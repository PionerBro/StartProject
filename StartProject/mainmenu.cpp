#include "mainmenu.h"

#include <QPushButton>
#include <QVBoxLayout>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    QPushButton* btnPlay = new QPushButton("Начать упражнения");
    btnPlay->setMinimumSize(300,40);
    connect(btnPlay, SIGNAL(clicked()), SLOT(toStartPage()));

    QPushButton* btnAddWords = new QPushButton("Добавить слова");
    btnAddWords->setMinimumSize(300,40);
    connect(btnAddWords, SIGNAL(clicked()), SLOT(toAddWords()));

    QPushButton* btnSettings = new QPushButton("Настройки");
    btnSettings->setMinimumSize(300,40);
    //connect(btnSettings, SIGNAL(clicked()), SLOT());

    QPushButton* btnQuit = new QPushButton("Выход");
    btnQuit->setMinimumSize(300,40);
    connect(btnQuit, SIGNAL(clicked()), SLOT(close()));

    QVBoxLayout* bxLayout = new QVBoxLayout;
    bxLayout->setContentsMargins(100,70,100,70);
    bxLayout->setSpacing(10);
    bxLayout->addWidget(btnPlay, 0,Qt::AlignHCenter);
    bxLayout->addWidget(btnAddWords, 0, Qt::AlignHCenter);
    bxLayout->addWidget(btnSettings, 0, Qt::AlignHCenter);
    bxLayout->addWidget(btnQuit, 0, Qt::AlignHCenter);

    setLayout(bxLayout);
}

void MainMenu::toAddWords(){
    emit crAddWords();
}

void MainMenu::toStartPage(){
    emit crStartPage();
}
