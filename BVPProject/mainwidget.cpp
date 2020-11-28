#include "mainwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QAction>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{

    btnList<<createButton(trUtf8("Журналы"))
           <<createButton(trUtf8("Отчеты"));
    QVBoxLayout* but_vbx = new QVBoxLayout;
    but_vbx->setAlignment(Qt::AlignTop);
    for(int i = 0; i<btnList.count();++i){
        but_vbx->addWidget(btnList[i]);
    }

    QWidget* start_wgt = new QWidget(this);
    start_wgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    start_wgt->setMinimumSize(400,400);
    start_wgt->setStyleSheet("background-color: #345424");

    QHBoxLayout* m_hbx = new QHBoxLayout(this);
    m_hbx->setContentsMargins(0,0,0,0);
    m_hbx->setSpacing(0);
    m_hbx->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_hbx->addLayout(but_vbx);
    m_hbx->addWidget(start_wgt);

    connect(btnList[0], SIGNAL(clicked()), this, SLOT(showMess()));
    QAction* btnAct = new QAction("click",this);
    btnAct->setShortcut(trUtf8("CTRL+B"));
    connect(btnAct, SIGNAL(triggered()), btnList[0], SLOT(click()));
    this->addAction(btnAct);
    btnList[0]->setDisabled(true);
}

QPushButton* MainWidget::createButton(const QString& btnName){
    QPushButton* btn = new QPushButton(btnName, this);
    btn->setFixedWidth(100);
    btn->setMaximumHeight(60);
    btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setFlat(true);
    return btn;
}

void MainWidget::showMess(){
    QMessageBox::information(0,"2", "btnClicked");
}

QList<QPushButton*>& MainWidget::getButtonList(){
    return btnList;
}
