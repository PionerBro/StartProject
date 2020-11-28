#include "mainjournal.h"
#include "order.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDockWidget>
#include <QCloseEvent>

MainJournal::MainJournal(QWidget *parent) : QDockWidget(parent)
{
    m_journal = new QTableWidget(row,col, this);

    QStringList lst;
    lst << "Дата" << "Номер" << "ФИО" << "Доставка" << "Город" << "Сумма";
    m_journal->setHorizontalHeaderLabels(lst);

    /*for (int j = 0 ; j<8 ; ++j ) {
        for(int i = 0; i<6; ++i){
            m_journal->setCellWidget(j, i, new QLabel("1", this));
        }
    }*/

    connect(m_journal, SIGNAL(cellDoubleClicked(int, int)), SLOT(slotOpenDoc(int)));
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    setAllowedAreas(Qt::TopDockWidgetArea);
    setWidget(m_journal);

}

void MainJournal::slotOpenDoc(int row){
    QPushButton* btn = new QPushButton(QString("back%1").arg(row));
    btn->setAttribute(Qt::WA_DeleteOnClose);
    btn->show();
    connect(btn, SIGNAL(clicked()), btn, SLOT(close()));
}

void MainJournal::closeEvent(QCloseEvent* pe){
    this->hide();
    pe->ignore();
}

void MainJournal::slotNewDoc(QStringList list){
    m_journal->setRowCount(++row);
    for(int i = 0; i<6; ++i){
        m_journal->setCellWidget(row-1, i, new QLabel(list[i],this));
    }
}
