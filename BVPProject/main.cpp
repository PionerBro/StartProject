#include "itemtable.h"
#include "order.h"
#include "test.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Test w;
    w.resize(800,500);
    w.show();
    return a.exec();
}
