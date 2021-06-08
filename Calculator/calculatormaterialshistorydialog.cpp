#include "calculatormaterialshistorydialog.h"
#include "calculatordatabase.h"
#include "calculatorhistorymodel.h"

#include <QTableView>
#include <QBoxLayout>
#include <QHeaderView>

#include <QDebug>

CalculatorMaterialsHistoryDialog::CalculatorMaterialsHistoryDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    QVBoxLayout* vbx = new QVBoxLayout(this);
    tableView = new QTableView(this);
    vbx->addWidget(tableView);
    QVector<QVariant> headers;
    headers<<tr("")
           <<tr("id")
           <<tr("Дата")
           <<tr("Цена");
    model = new CalculatorHistoryModel(TABLE_MATERIALS_PRICES, headers, this);
    model->setSortCol(2);
    tableView->setModel(model);
    viewSettings();
}

void CalculatorMaterialsHistoryDialog::viewSettings(){
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setColumnHidden(1,true);
    //tableView->setColumnHidden(2,true);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //tableView->setItemDelegateForColumn(5, new CalculatorMaterialsDelegate(this));
    tableView->resizeColumnsToContents();
    tableView->setColumnWidth(2, 100);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setCurrentIndex(model->index(0,1,QModelIndex()));
}

void CalculatorMaterialsHistoryDialog::showAtNum(qlonglong num){
    if(model->setRootNum(num))
        show();
}

CalculatorHistoryModel* CalculatorMaterialsHistoryDialog::getModel() const{
    return model;
}

void CalculatorMaterialsHistoryDialog::createItem(QVector<QVariant>& data){
    if(!model->createModelItem(data))
        return;
}
