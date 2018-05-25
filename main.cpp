#include "mainwindow.h"
#include "formtable.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    FormTable f;
    f.show();
    f.setInfoWindow(&w);
    return a.exec();
}
