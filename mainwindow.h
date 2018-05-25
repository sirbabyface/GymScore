#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>


#include "information.h"
namespace QXlsx {
    class Worksheet;
    class Cell;
}

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadInformation(int row, const QXlsx::Worksheet *sheet);

    void blank();

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);

    void setupScene();
    void setupBlankScene();
    void setItem(QGraphicsTextItem *item, int x, int y);
    void updatePlayersPosition(const QList<QString> &players);

    void paintEvent(QPaintEvent *pQEvent);
    void updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemFloat(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemPlayers(QXlsx::Cell *cell);
private:
    QGraphicsScene m_sceneInfo;
    QGraphicsScene m_sceneBlank;
    QGraphicsTextItem *m_number;
    QGraphicsTextItem *m_players[3];
    QGraphicsTextItem *m_scoreExecution;
    QGraphicsTextItem *m_scoreArtistic;
    QGraphicsTextItem *m_difference;
    QGraphicsTextItem *m_penalization;
    QGraphicsTextItem *m_finalScore;
    QGraphicsTextItem *m_level;
    QGraphicsTextItem *m_type;
    QGraphicsTextItem *m_team;
    QGraphicsPixmapItem *m_teamIcon;

};

#endif // MAINWINDOW_H
