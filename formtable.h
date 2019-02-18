#ifndef FORMTABLE_H
#define FORMTABLE_H

#include "settings.h"

#include <QWidget>

namespace QXlsx {
    class Document;
    class Worksheet;
    class SheetModel;
}

class MainWindow;

namespace Ui {
class FormTable;
}

class FormTable : public QWidget
{
    Q_OBJECT

public:
    explicit FormTable(MainWindow *infoWindow, QWidget *parent = Q_NULLPTR);
    ~FormTable();

    void setInfoWindow(MainWindow *win);

public slots:
    void openFile();
    void loadFile(const QString &filePath);
    bool saveFile();
    void moveLeft();
    void moveRight();
    void blankInfoWindow();
    void updateInfoWindow();
    void loadRules();
    void loadRules(const QString &filePath);
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void tableClicked(const QModelIndex &index);
    void closeEvent(QCloseEvent *event);

private:
    void moveToNextEditableCell(int row, int column);


private:
    Ui::FormTable *m_ui;
    MainWindow *m_infoWindow;
    QString m_previousPath;
    int m_currentRow;
    QXlsx::Document *m_doc;
    QXlsx::Worksheet *m_sheet;
    QXlsx::SheetModel *m_model;
    Settings *m_settings;
};

#endif // FORMTABLE_H
