#include "mainwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QAction>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{

    btnList<<createButton(tr("Журналы"))
           <<createButton(tr("Отчеты"))
           <<createButton(tr("Документы"));

    wgtList<<(new QWidget(this))
           <<(new QWidget(this))
           <<(new QWidget(this));
    QVBoxLayout* but_vbx = new QVBoxLayout;
    but_vbx->setAlignment(Qt::AlignTop);
    for(int i = 0; i<btnList.count();++i){
        but_vbx->addWidget(btnList[i]);
    }

    start_wgt = new QWidget(this);
    start_wgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    start_wgt->setMinimumSize(400,400);
    start_wgt->setStyleSheet("background-color: #345424");

    m_hbx = new QHBoxLayout(this);
    m_hbx->setContentsMargins(0,0,0,0);
    m_hbx->setSpacing(0);
    m_hbx->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_hbx->addLayout(but_vbx);
    m_hbx->addWidget(start_wgt);

    connect(btnList[0], SIGNAL(clicked()), this, SLOT(btn0_clicked()));
    connect(btnList[1], SIGNAL(clicked()), this, SLOT(btn1_clicked()));
    connect(btnList[2], SIGNAL(clicked()), this, SLOT(btn2_clicked()));

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

void MainWidget::btn0_clicked(){
    if(wgtNum == -1){
        m_hbx->removeWidget(start_wgt);
        start_wgt->hide();
    }else{
        m_hbx->removeWidget(wgtList[wgtNum]);
        wgtList[wgtNum]->hide();
        btnList[wgtNum]->setEnabled(true);
    }
    wgtNum = 0;
    m_hbx->addWidget(wgtList[wgtNum]);
    btnList[wgtNum]->setDisabled(true);
}

void MainWidget::btn1_clicked(){
    if(wgtNum == -1){
        m_hbx->removeWidget(start_wgt);
        start_wgt->hide();
    }else{
        m_hbx->removeWidget(wgtList[wgtNum]);
        wgtList[wgtNum]->hide();
        btnList[wgtNum]->setEnabled(true);
    }
    wgtNum = 1;
    m_hbx->addWidget(wgtList[wgtNum]);
    btnList[wgtNum]->setDisabled(true);
    QMessageBox::information(0,"1", "btnClicked");
}

void MainWidget::btn2_clicked(){
    if(wgtNum == -1){
        m_hbx->removeWidget(start_wgt);
        start_wgt->hide();
    }else{
        m_hbx->removeWidget(wgtList[wgtNum]);
        wgtList[wgtNum]->hide();
        btnList[wgtNum]->setEnabled(true);
    }
    wgtNum = 2;
    m_hbx->addWidget(wgtList[wgtNum]);
    btnList[wgtNum]->setDisabled(true);
    QMessageBox::information(0,"2", "btnClicked");
}

QList<QPushButton*>& MainWidget::getButtonList(){
    return btnList;
}
