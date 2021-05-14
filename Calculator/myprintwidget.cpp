#include "myprintwidget.h"
#include "myprintwidgettable.h"
#include "myprintwidgetdelegate.h"


#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QHeaderView>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QScrollBar>
#include <QColor>
#include <QBrush>
#include <QDate>


MyPrintWidget::MyPrintWidget(const QList<QStringList>& data, QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    resize(740,600);
    QVBoxLayout* vbx = new QVBoxLayout(this);
    tableView = new MyPrintWidgetTable(this);
    tableView->setItemDelegate(new MyPrintWidgetDelegate);
    tableView->horizontalHeader()->setHidden(true);
    tableView->verticalHeader()->setHidden(true);
    tableView->setShowGrid(false);
    vbx->addWidget(tableView);
    QToolBar* toolBar = new QToolBar(this);
    vbx->addWidget(toolBar);
    QAction* viewAct = new QAction("Ch",this);
    QAction* gridAct = new QAction("grSt", this);
    QAction* printAct = new QAction("Print", this);
    viewAct->setCheckable(true);
    gridAct->setCheckable(true);
    //gridAct->setChecked(true);
    toolBar->addAction(viewAct);
    toolBar->addAction(gridAct);
    toolBar->addAction(printAct);
    connect(viewAct, SIGNAL(toggled(bool)), tableView, SLOT(setEditable(bool)));
    connect(gridAct, SIGNAL(toggled(bool)), this, SLOT(gridSlot(bool)));
    connect(printAct, SIGNAL(triggered()), this, SLOT(printDoc()));
    setupPrintList(data);
    tableView->clearBuffers();

}

void MyPrintWidget::gridSlot(bool checked){
    tableView->horizontalHeader()->setHidden(!checked);
    tableView->verticalHeader()->setHidden(!checked);
    tableView->setShowGrid(checked);
}

void MyPrintWidget::setupPrintList(const QList<QStringList>& data){
    QHeaderView *hHeader = tableView->horizontalHeader();

    hHeader->resizeSection(0, 300);
    hHeader->resizeSection(1, 100);
    hHeader->resizeSection(2, 100);
    hHeader->resizeSection(3, 100);
    hHeader->resizeSection(4, 100);

    int row = 0;

    QTableWidgetItem* item;
    QFont textFont = QFont("TimesNewRoman", 10);
    item = setupTableItem("УТВЕРЖДАЮ", false, Qt::AlignBottom, textFont);
    tableView->setItem(row,3,item);

    item = setupTableItem("Директор ЧПТУП \"В.П.Бутковский\"", false,Qt::AlignBottom, textFont);
    tableView->setItem(++row,2,item);
    tableView->setSpan(row,2,1,3);

    item = setupTableItem("_______________Бутковский В.П.", false, Qt::AlignBottom, textFont);
    tableView->setItem(++row,2,item);
    tableView->setSpan(row++,2,1,3);

    textFont = QFont("TimesNewRoman", 16);
    item = setupTableItem("КАЛЬКУЛЯЦИЯ", false, Qt::AlignBottom, textFont);
    tableView->setItem(++row,1,item);
    tableView->setSpan(row,1,1,2);

    textFont = QFont("TimesNewRoman", 14);
    item = setupTableItem(data.value(0).value(0), false, Qt::AlignBottom, textFont);
    tableView->setItem(++row,0,item);

    item = setupTableItem(data.value(0).value(2), false, Qt::AlignBottom|Qt::AlignRight, textFont);
    tableView->setItem(row,3,item);
    tableView->setSpan(row,3,1,2);

    item = setupTableItem("Выход: " + data.value(0).value(1) + " кг.", false,  Qt::AlignBottom, textFont);
    tableView->setItem(++row,0,item);

    ++row;
    textFont = QFont("TimesNewRoman", 12);
    item = setupTableItem("Наименование", true, Qt::AlignBottom, textFont);
    tableView->setItem(++row,0,item);
    item = setupTableItem("Ед.Изм.", true, Qt::AlignBottom, textFont);
    tableView->setItem(row,1,item);
    item = setupTableItem("Кол-во", true, Qt::AlignBottom, textFont);
    tableView->setItem(row,2,item);
    item = setupTableItem("Цена", true, Qt::AlignBottom, textFont);
    tableView->setItem(row,3,item);
    item = setupTableItem("Сумма", true, Qt::AlignBottom, textFont);
    tableView->setItem(row,4,item);

    for(int i = 1; i < data.count(); ++i){
        item = setupTableItem(data.value(i).value(0), true, Qt::AlignBottom, textFont);
        tableView->setItem(++row,0,item);
        item = setupTableItem(data.value(i).value(1), true, Qt::AlignBottom, textFont);
        tableView->setItem(row,1,item);
        item = setupTableItem(data.value(i).value(2), true, Qt::AlignBottom, textFont);
        tableView->setItem(row,2,item);
        item = setupTableItem(data.value(i).value(3), true, Qt::AlignBottom|Qt::AlignRight, textFont);
        tableView->setItem(row,3,item);
        item = setupTableItem(data.value(i).value(4), true, Qt::AlignBottom|Qt::AlignRight, textFont);
        tableView->setItem(row,4,item);
    }
    item = setupTableItem(data.value(0).value(3), true, Qt::AlignBottom|Qt::AlignRight, textFont);
    tableView->setItem(++row,4,item);

    ++row;
    item = setupTableItem("Цена за порцию " + data.value(0).value(4) + "г. = " + data.value(0).value(5) + "р.", false, Qt::AlignBottom|Qt::AlignRight, textFont);
    tableView->setItem(++row,2,item);
    tableView->setSpan(row, 2, 1, 3);
}

QTableWidgetItem* MyPrintWidget::setupTableItem(const QVariant& data, bool border, Qt::Alignment align, QFont font){
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setFont(font);
    item->setData(Qt::EditRole, data);
    item->setData(Qt::UserRole +1, border);
    item->setTextAlignment(align);
    return item;
}

void MyPrintWidget::printDoc(){
    QPrinter* printer = new QPrinter;
    QPrintDialog dlg(printer, this);
    if(dlg.exec()){
        float scaleX, scaleY;
        scaleX = scaleY = 1.0;


        QPainter painter(printer);
        painter.scale(scaleX,scaleY);
        //printer->getPageMargins(&leftMargin,&topMargin,&rightMargin,&bottomMargin,QPrinter::Inch);

        int dpiX = logicalDpiX();
        int dpiY = logicalDpiY();


        int pageHeight = qRound(printer->pageRect(QPrinter::Inch).height()*dpiY);// - pixelBottomMargin;
        int pageWidth = qRound(printer->pageRect(QPrinter::Inch).width()*dpiX);// - pixelRightMargin;

        pageHeight = qRound(pageHeight/scaleY);
        pageWidth = qRound(pageWidth/scaleX);

        pageHeight -= tableView->horizontalHeader()->height();
        pageWidth -= tableView->verticalHeader()->width();

        QList <QRect> hPages;
        QVector<QVector<QRect>> pages;

        QList <int> rightColumn;
        QList <int> bottomRow;
        QRect page;


        int currentPageWidth = 0;
        int currentPageHeight = 0;
        int logicalIndex = 0;
        int h = 0;

        for(; h < 5; ++h)
        {
            logicalIndex =tableView->horizontalHeader()->logicalIndex(h);
            if(!tableView->horizontalHeader()->isSectionHidden(logicalIndex))
            {
                if(pageWidth >= currentPageWidth + tableView->horizontalHeader()->sectionSize(logicalIndex))
                {
                    currentPageWidth += tableView->horizontalHeader()->sectionSize(logicalIndex);
                }
                else
                {

                    rightColumn.push_back(h-1); // сохраняем начало новой страницы по горизонтали
                    currentPageWidth = tableView->horizontalHeader()->sectionSize(logicalIndex);
                }
            }
        }
        rightColumn.push_back(h-1); // сохраняем начало новой страницы по горизонтали


        int v = 0;
        for(; v < 30; v++)
        {
            logicalIndex = tableView->verticalHeader()->logicalIndex(v);
            if(!tableView->verticalHeader()->isSectionHidden(logicalIndex))
            {
                if(pageHeight >= currentPageHeight + tableView->verticalHeader()->sectionSize(logicalIndex))
                {
                    currentPageHeight += tableView->verticalHeader()->sectionSize(logicalIndex);
                }
                else
                {
                    bottomRow.push_back(v-1); // сохраняем начало новой страницы по вертикали
                    currentPageHeight = tableView->verticalHeader()->sectionSize(logicalIndex);
                }
            }
        }
        bottomRow.push_back(v-1); // сохраняем начало новой страницы по горизонтали


        int logicalIndexV = 0;
        int logicalIndexH = 0;
        int totalPages = 0; // всего страниц

        QPoint topLeft(0,0);
        QPoint bottomRight;
        for(int v = 0; v < bottomRow.count(); v++)  // делим на страницы
        {
            for(int h = 0; h < rightColumn.count(); h++)
            {
                bottomRight.setX(rightColumn.at(h)); //
                bottomRight.setY(bottomRow.at(v));
                page.setTopLeft(topLeft);
                page.setBottomRight(bottomRight);
                hPages.push_back(page);  // загоняем поделенные страницы в список страниц
                topLeft.setX(rightColumn.at(h)+1);  // перейдем на следующий столбец
                totalPages++;
            }

            topLeft.setX(0);
            topLeft.setY(bottomRow.at(v)+1);

            pages.push_back(hPages);
            hPages.clear();
        }


                            /// Рисование на листе

        int currentSectionWidth = 0;    // ширина текущей горизонтальной секции
        int currentSectionHeight = 0;   // высота текущей вертикальной секции
        int translateWidth = 0; // общая ширина всех нарисованных секций для обратной трансляции (для возвращение в начало координат)
        int translateHeight = 0;    // общая высота всех нарисованных секций для обратной трансляции (для возвращение в начало координат)
        int counter = 1;
        int currentNum = 1;
        int tempNum = 0;
        int currentPage = 0;    // текущая печатаемая страница
        logicalIndexV = 0;
        logicalIndexH = 0;
        QColor color = QColor(Qt::lightGray);
        QBrush brush(color);
        QFont font;
        int horizontalHeaderAlign = Qt::AlignRight|Qt::AlignBottom;
        int tableAlign = Qt::AlignLeft|Qt::AlignBottom;

        for(int p = 0; p < pages.count(); p++)
        {
            for(int hP = 0; hP < pages.at(p).count(); hP++)
            {

                /// рисуем заголовок

/*                font = tableView->horizontalHeader()->font();
                painter.setFont(font);

                //Сначала нарисуем порядковый номер
                currentSectionWidth = tableView->verticalHeader()->width();
                painter.fillRect(0,0,currentSectionWidth,tableView->horizontalHeader()->height(),brush);
                painter.drawRect(0,0,currentSectionWidth,tableView->horizontalHeader()->height());
                painter.drawText(0,0,currentSectionWidth,tableView->horizontalHeader()->height()
                                 ,horizontalHeaderAlign,"№");
                painter.translate(currentSectionWidth,0);
                translateWidth += currentSectionWidth;

                //теперь все остальные части заголовка
                for(int h = pages.at(p).at(hP).left(); h <= pages.at(p).at(hP).right(); h++)
                {
                    logicalIndexH = tableView->horizontalHeader()->logicalIndex(h);
                    if(!tableView->horizontalHeader()->isSectionHidden(logicalIndexH))
                    {
                        currentSectionWidth = tableView->horizontalHeader()->sectionSize(logicalIndexH);
                        painter.fillRect(0,0,currentSectionWidth,tableView->horizontalHeader()->height(),brush);
                        painter.drawRect(0,0,currentSectionWidth,tableView->horizontalHeader()->height());
                        painter.drawText(0,0,currentSectionWidth,tableView->horizontalHeader()->height()
                                         ,horizontalHeaderAlign,tableView->horizontalHeaderItem(logicalIndexH)->text());
                        painter.translate(currentSectionWidth,0);
                        translateWidth += currentSectionWidth;
                    }
                }
                painter.translate(-translateWidth,tableView->horizontalHeader()->height());
                translateWidth = 0;
*/

                /// рисуем данные

                font = this->font();
                painter.setFont(font);

                for(int v = pages.at(p).at(hP).top(); v <= pages.at(p).at(hP).bottom(); v++)
                {
                    logicalIndexV = tableView->verticalHeader()->logicalIndex(v);
                    if(!tableView->verticalHeader()->isSectionHidden(logicalIndexV))
                    {
                        // рисуем порядковый номер
                        currentSectionWidth = tableView->verticalHeader()->width();
                        currentSectionHeight = tableView->verticalHeader()->sectionSize(logicalIndexV);

    //                    painter.fillRect(0,0,currentSectionWidth,currentSectionHeight,brush);
                        painter.drawRect(0,0,currentSectionWidth,currentSectionHeight);
                        painter.drawText(2,2,currentSectionWidth-4,currentSectionHeight-4
                                         ,tableAlign,QString::number(counter));
                        painter.translate(currentSectionWidth,0);
                        translateWidth += currentSectionWidth;
                        counter++;


                        // и всё остальное
                        for(int h = pages.at(p).at(hP).left(); h <= pages.at(p).at(hP).right(); h++)
                        {
                            logicalIndexH = tableView->horizontalHeader()->logicalIndex(h);
                            if(!tableView->horizontalHeader()->isSectionHidden(logicalIndexH))
                            {
                                currentSectionWidth = tableView->horizontalHeader()->sectionSize(logicalIndexH);
                                painter.drawRect(0,0,currentSectionWidth,currentSectionHeight);
                                if(tableView->item(logicalIndexV,logicalIndexH)){
                                    painter.drawText(2,2,currentSectionWidth-4,currentSectionHeight-4,tableAlign,
                                                     tableView->item(logicalIndexV,logicalIndexH)->text());
                                }
                                painter.translate(currentSectionWidth,0);
                                translateWidth += currentSectionWidth;
                            }
                        }
                        painter.translate(-translateWidth,currentSectionHeight);
                        translateHeight += currentSectionHeight;
                        translateWidth = 0;
                    }
                }

                currentPage++;
                if(currentPage < totalPages)
                {
                    printer->newPage();
                    painter.translate(-translateWidth,-(translateHeight+tableView->horizontalHeader()->height()));
                    translateHeight = 0;
                    translateWidth = 0;
                }

                tempNum = counter;      // сохранили счетчик во временную переменную
                counter = currentNum;   // сброс счетчика
            }
            currentNum = tempNum;
            counter = tempNum;
        }
    }
}
