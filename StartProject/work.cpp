#include "work.h"
#include <ctime>
#include <cstdlib>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#define ENGWORDS "EngWords.txt"
#define RUSWORDS "RusWords.txt"

Work::Work(int pos, QWidget *parent) : QWidget(parent), m_pos(pos)
{
    srand(static_cast<unsigned int>(time(0)));

    QHBoxLayout* hbx[10];
    QVBoxLayout* vbx = new QVBoxLayout;
    for(int i = 0; i <10; ++i){
        lbl1[i] = new QLabel;
        lne[i] = new QLineEdit;
        lbl2[i] = new QLabel;
        lbl3[i] = new QLabel;
        hbx[i] = new QHBoxLayout;
        hbx[i]->addWidget(lbl1[i]);
        hbx[i]->addWidget(lne[i]);
        hbx[i]->addWidget(lbl2[i]);
        hbx[i]->addWidget(lbl3[i]);
        vbx->addLayout(hbx[i]);
    }
    btnOk = new QPushButton("Проверить");
    connect(btnOk, SIGNAL(clicked()), SLOT(checkResult()));
    QPushButton* btnAgain = new QPushButton("Ещё");
    connect(btnAgain, SIGNAL(clicked()), SLOT(practice()));
    QPushButton* btnCancel = new QPushButton("Назад");
    connect(btnCancel, SIGNAL(clicked()), SLOT(backToStart()));
    QHBoxLayout* hbxBut = new QHBoxLayout;
    hbxBut->addWidget(btnOk);
    hbxBut->addWidget(btnAgain);
    hbxBut->addWidget(btnCancel);
    vbx->addLayout(hbxBut);

    setLayout(vbx);
    practice();
}

void Work::practice(){
    btnOk->setEnabled(true);
    QFile fe(ENGWORDS);
    QFile fr(RUSWORDS);

    if(!fe.open(QIODevice::ReadOnly | QIODevice::Text)|| !fr.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0, "Erorr", "Ошибка открытия файла");
        exit(1);
    }

    QTextStream ine(&fe);
    QTextStream inr(&fr);

    for (int i = 0; i<10 ; ++i) {
        int p_pos = rand()%m_pos;
        ine.seek(0);
        inr.seek(0);
        for (int j = 0; j<p_pos ; ++j) {
            ine.readLine();
            inr.readLine();
        }
        lne[i]->clear();
        lbl2[i]->clear();
        lbl1[i]->setText(ine.readLine());
        lbl3[i]->hide();
        lbl3[i]->setText(inr.readLine());
    }

    fe.close();
    fr.close();
}

void Work::checkResult(){
    btnOk->setDisabled(true);
    for (int i = 0; i < 10 ; ++i) {
        if(!((lne[i]->text().toUpper()).compare(lbl3[i]->text().toUpper()))){
            lbl2[i]->setText("Верно");
        }
        else
        {
            lbl2[i]->setText("Неверно");
        }
        lbl3[i]->show();
    }
}

void Work::backToStart(){
    emit btStart();
}
