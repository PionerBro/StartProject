#include "directorywidget.h"
#include "mytreemodel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QToolBar>
#include <QHeaderView>
#include <QAction>

#include <QDebug>

DirectoryWidget::DirectoryWidget(QWidget* parent, Qt::WindowFlags f):QDialog(parent, f)
{
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    QToolBar* toolBar = new QToolBar(this);
    QAction* newAct = new QAction("new", toolBar);
    newAct->setShortcut(tr("INSERT"));
    connect(newAct, SIGNAL(triggered()),this, SLOT(createItem()));
    QAction* editAct = new QAction("edit", toolBar);
    editAct->setShortcut(Qt::Key_Enter);
    connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));
    QAction* newFolder = new QAction("newFolder", toolBar);
    connect(newFolder, SIGNAL(triggered()), this, SLOT(createFolder()));
    toolBar->addAction(newAct);
    toolBar->addAction(editAct);
    toolBar->addAction(newFolder);
    view = new QTableView(this);
    vbx->addWidget(toolBar);
    vbx->addWidget(view);
    QList<QVariant> header;
    header<<"Num"
           <<"Name"
           <<"Folder"
           <<"Parent";
    model = new MyTreeModel(header, this);
    view->setModel(model);
    connect(view, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
    //connect(view, SIGNAL(entered(QModelIndex)), model, SLOT(rootItemChanged(QModelIndex)));
    viewSettings();

}

void DirectoryWidget::viewSettings(){
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setColumnHidden(0,true);
    view->setColumnHidden(2,true);
    view->setColumnHidden(3,true);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setCurrentIndex(model->index(0,0,QModelIndex()));
}

void DirectoryWidget::createItem(){
    qDebug()<<"createItem";
}

void DirectoryWidget::editItem(){
    qDebug()<<"editItem";
}

void DirectoryWidget::createFolder(){
    qDebug()<<"createFolder";
}
