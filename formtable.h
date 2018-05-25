#ifndef FORMTABLE_H
#define FORMTABLE_H

#include <QWidget>

namespace QXlsx {
    class Document;
    class Worksheet;
}

class MainWindow;

namespace Ui {
class FormTable;
}

class FormTable : public QWidget
{
    Q_OBJECT

public:
    explicit FormTable(QWidget *parent = 0);
    ~FormTable();

    void setInfoWindow(MainWindow *win);

public slots:
    void openFile();
    void loadFile(const QString &filePath);
    void saveFile();
    void moveLeft();
    void moveRight();
    void blankInfoWindow();
    void updateInfoWindow();


private:
    Ui::FormTable *ui;
    MainWindow *infoWindow;
    int currentRow;
    QXlsx::Document *doc;
    QXlsx::Worksheet *sheet;
};

#endif // FORMTABLE_H
