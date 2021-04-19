#include "addwords.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRegularExpression>

#define ENGWORDS "EngWords.txt"
#define RUSWORDS "RusWords.txt"


AddWords::AddWords(QWidget *parent) : QWidget(parent)
{
    QLabel* plblEngWord = new QLabel("Ангийское слово");
    QLabel* plblRusWord = new QLabel("Перевод");

    plneEngWord = new QLineEdit;
    plneRusWord = new QLineEdit;

    QHBoxLayout* bxLabelLayout = new QHBoxLayout;

    bxLabelLayout->addWidget(plblEngWord);
    bxLabelLayout->addWidget(plblRusWord);

    QHBoxLayout* bxLineEditLayout = new QHBoxLayout;
    bxLineEditLayout->addWidget(plneEngWord);
    bxLineEditLayout->addWidget(plneRusWord);

    QPushButton* btnAdd = new QPushButton("Добавить");
    connect(btnAdd, SIGNAL(clicked()), SLOT(addWords()));
    QPushButton* btnCancel = new QPushButton("Отмена");
    connect(btnCancel, SIGNAL(clicked()),SLOT(retToMenu()));

    QHBoxLayout* bxButtonLayout = new QHBoxLayout;
    bxButtonLayout->addWidget(btnAdd);
    bxButtonLayout->addWidget(btnCancel);

    plblInfo = new QLabel;

    QVBoxLayout* vbxLayout = new QVBoxLayout;
    vbxLayout->addLayout(bxLabelLayout);
    vbxLayout->addLayout(bxLineEditLayout);
    vbxLayout->addWidget(plblInfo);
    vbxLayout->addLayout(bxButtonLayout);

    setLayout(vbxLayout);
}

void AddWords::addWords(){
    QString engstr = plneEngWord->text();
    QString russtr = plneRusWord->text();

    if(engstr.toUpper().contains(QRegularExpression("[^-A-Z\\s]")) ||
            russtr.toUpper().contains(QRegularExpression("[^-А-Я\\s]"))){
        plblInfo->setText("Неверный ввод данных");
        return;
    }
    plblInfo->setText("f");
    QFile ef(ENGWORDS);
    QFile rf(RUSWORDS);
    if(!ef.open(QIODevice::Append | QIODevice::Text) || !rf.open(QIODevice::Append | QIODevice::Text)){
        QMessageBox::information(0, "Erorr", "Ошибка открытия файла");
        return;
    }

    QTextStream outef(&ef);
    QTextStream outrf(&rf);

    outef<<engstr<<"\n";
    outrf<<russtr<<"\n";

    plblInfo->setText("Слова успешно добавлены");
    plneEngWord->clear();
    plneRusWord->clear();
    ef.close();
    rf.close();
}

void AddWords::retToMenu(){
    emit returnToMenu();
}
