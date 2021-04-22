#include "cellwidget.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

#include <QDebug>

CellWidget::CellWidget(QWidget *parent) : QWidget(parent)
{
    resize(width(), 25);
    QHBoxLayout* cellHbx = new QHBoxLayout(this);
    cellHbx->setContentsMargins(0,0,0,0);
    m_label = new QLabel(this);
    m_btn = new QPushButton(tr("."),this);
    m_btn->setMaximumWidth(20);
    cellHbx->addWidget(m_label);
    cellHbx->addWidget(m_btn,0, Qt::AlignRight);
}

CellWidget::~CellWidget(){
    qDebug()<<"CellWidget destroyed";
}

void CellWidget::setText(QString text){
    m_label->setText(text);
}

int CellWidget::row()const{
    return m_row;
}

int CellWidget::column()const{
    return m_column;
}

void CellWidget::setRow(int row){
    m_row = row;
}

void CellWidget::setColumn(int column){
    m_column = column;
}

QPushButton* CellWidget::buttonWidget()const{
    return m_btn;
}
