#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>


namespace QXlsx {
    class Worksheet;
    class Cell;
}

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    QGraphicsScene *loadInformation(int row, const QXlsx::Worksheet *sheet);
    QGraphicsScene *loadInformation(int row, const QXlsx::Worksheet *sheet, const Settings &settings);

    QGraphicsScene *blank();

    void setupScene(const Settings &settings);
    void setupBlankScene(const Settings &settings);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);

    void setupScene();
    void setupBlankScene();

    void setupRankingScene();
    void setupArea(QGraphicsScene &scene, const QSize &size);
    void updatePlayersPosition(const QList<QString> &players, QGraphicsTextItem *item);

    void updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemStringCenter(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item, const QString &prefix);
    void updateItemFloat(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemPlayers(QXlsx::Cell *cell, QGraphicsTextItem *item);

    const int KEY_COLUMN = 1;
    const int KEY_ALIGNMENT = 2;
    const int KEY_POSITION = 3;

private:
    QGraphicsScene m_sceneInfo;
    QGraphicsScene m_sceneBlank;
    QGraphicsScene m_sceneRanking;

};

#endif // MAINWINDOW_H
