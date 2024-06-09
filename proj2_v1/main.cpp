#include "widget.h"
#include "Function.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Coor>("Coor");
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
