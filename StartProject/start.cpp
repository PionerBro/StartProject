#include "start.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#define ENGWORDS "EngWords.txt"
#define RUSWORDS "RusWords.txt"

Start::Start(QWidget *parent, int num) : QWidget(parent), m_num(num)
{
    plblEng = new QLabel;
    plblRus = new QLabel;
    QFile fe(ENGWORDS);
    QFile fr(RUSWORDS);
    if(!fe.open(QIODevice::ReadOnly | QIODevice::Text)
            || !fr.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0, "Erorr", "Отсутсвует файлы: EngWords.txt или RusWords.txt");
        exit(1);
    }
    QTextStream ein(&fe);
    QTextStream rin(&fr);
    int count;
    for (count = 0; count < m_num; ++count) {
        ein.readLine();
        rin.readLine();
    }
    QString strE = "";
    QString strR = "";

    for (p_num = 0; p_num<10; ++p_num){
        if(ein.atEnd()){
            break;
        }
        strE += ein.readLine()+"\n";
        strR += rin.readLine()+"\n";
    }
    plblEng->setText(strE);
    plblRus->setText(strR);
    fe.close();
    fr.close();

    QHBoxLayout* hbxLabel = new QHBoxLayout;
    hbxLabel->addWidget(plblEng);
    hbxLabel->addWidget(plblRus);

    QPushButton* btnPrev = new QPushButton("<<<");
    connect(btnPrev, SIGNAL(clicked()), SLOT(prevPage()));
    QPushButton* btnNext = new QPushButton(">>>");
    connect(btnNext, SIGNAL(clicked()), SLOT(nextPage()));
    QHBoxLayout* hbxBut1 = new QHBoxLayout;
    hbxBut1->addWidget(btnPrev);
    hbxBut1->addWidget(btnNext);

    QPushButton* btnExercise = new QPushButton("Практика");

    QPushButton* btnCancel = new QPushButton("Меню");
    connect(btnCancel, SIGNAL(clicked()), SLOT(toMenu()));
    QHBoxLayout* hbxBut2 = new QHBoxLayout;
    hbxBut2->addWidget(btnExercise);
    hbxBut2->addWidget(btnCancel);

    QVBoxLayout* vbx = new QVBoxLayout;
    vbx->addLayout(hbxLabel);
    vbx->addLayout(hbxBut1);
    vbx->addLayout(hbxBut2);

    setLayout(vbx);
}

void Start::prevPage(){
    if (m_num == 0){
        return;
    }
    if ((m_num-=10)<= 0){
        m_num = 0;
    }
    QFile fe(ENGWORDS);
    QFile fr(RUSWORDS);
    if(!fe.open(QIODevice::ReadOnly | QIODevice::Text)
            || !fr.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0, "Erorr", "Отсутсвует файлы: EngWords.txt или RusWords.txt");
        exit(1);
    }
    QTextStream ein(&fe);
    QTextStream rin(&fr);
    int count;
    for (count = 0; count < m_num; ++count) {
        ein.readLine();
        rin.readLine();
    }
    QString strE = "";
    QString strR = "";

    for (p_num = 0; p_num<10; ++p_num){
        if(ein.atEnd()){
            break;
        }
        strE += ein.readLine()+"\n";
        strR += rin.readLine()+"\n";
    }
    plblEng->setText(strE);
    plblRus->setText(strR);
    fe.close();
    fr.close();
}

void Start::nextPage(){
    if (p_num < 10){
        QMessageBox::information(0, "information", "Добавьте больше слов");
        return;
    }
    m_num += p_num;
    QFile fe(ENGWORDS);
    QFile fr(RUSWORDS);
    if(!fe.open(QIODevice::ReadOnly | QIODevice::Text)
            || !fr.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0, "Erorr", "Отсутсвует файлы: EngWords.txt или RusWords.txt");
        exit(1);
    }
    QTextStream ein(&fe);
    QTextStream rin(&fr);
    int count;
    for (count = 0; count < m_num; ++count) {
        ein.readLine();
        rin.readLine();
    }
    QString strE = "";
    QString strR = "";

    for (p_num = 0; p_num<10; ++p_num){
        if(ein.atEnd()){
            break;
        }
        strE += ein.readLine()+"\n";
        strR += rin.readLine()+"\n";
    }
    plblEng->setText(strE);
    plblRus->setText(strR);
    fe.close();
    fr.close();
}

void Start::toMenu(){
    emit crMainMenu();
}
