#include "mainwindow.h"
#include "formtable.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QSettings>
#include <qscreen.h>

Q_DECLARE_METATYPE(QList<int>)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Required to read List of int from ini files
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");

    MainWindow w;

    FormTable f;
    f.setInfoWindow(&w);




    if(a.screens().size() >= 2) {
        QRect rect =  a.screens()[0]->geometry();
        int x = rect.x() + rect.width() / 2 - f.size().width() / 2;
        int y = rect.y() + rect.height() / 2 - f.size().height() / 2;
        f.move(x, y);
        f.show();

        rect = a.screens()[1]->geometry();
        x = rect.x() + rect.width() / 2 - f.size().width() / 2;
        y = rect.y() + rect.height() / 2 - f.size().height() / 2;
        w.move(x, y);
        w.showFullScreen();
    }
    else {
        f.show();
    }

    return a.exec();
}
