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
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QGraphicsScene *loadInformation(int row, const QXlsx::Worksheet *sheet);

    QGraphicsScene *blank();

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);

    void setupScene();
    void setupScene(const Settings &settings);
    void setupBlankScene();
    void setupRankingScene();
    void setItem(QGraphicsTextItem *item, int x, int y);
    void updatePlayersPosition(const QList<QString> &players);

    void paintEvent(QPaintEvent *pQEvent);
    void updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemStringCenter(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item, const QString &prefix);
    void updateItemFloat(QXlsx::Cell *cell, QGraphicsTextItem *item);
    void updateItemPlayers(QXlsx::Cell *cell);
private:
    QGraphicsScene m_sceneInfo;
    QGraphicsScene m_sceneBlank;
    QGraphicsScene m_sceneRanking;

    // Score
    QGraphicsTextItem *m_number;
    QGraphicsTextItem *m_players[3];
    QGraphicsTextItem *m_scoreExecution;
    QGraphicsTextItem *m_scoreArtistic;
    QGraphicsTextItem *m_difficulty;
    QGraphicsTextItem *m_penalization;
    QGraphicsTextItem *m_finalScore;
    QGraphicsTextItem *m_level;
    QGraphicsTextItem *m_type;
    QGraphicsTextItem *m_club;
    QGraphicsTextItem *m_team;
    QGraphicsPixmapItem *m_teamIcon;

    // Ranking
    QGraphicsTextItem *m_rankingPlayers[4];
    QGraphicsTextItem *m_rankingTitle;
    QGraphicsTextItem *m_rankingPosition[4];
    QGraphicsTextItem *m_rankingScore[4];
    QGraphicsTextItem *m_rankingClub[4];
    QGraphicsTextItem *m_rankingLevel;
    QGraphicsTextItem *m_rankingType;

    // Blank
    QGraphicsPixmapItem *acm;
    QGraphicsPixmapItem *pub;

};

#endif // MAINWINDOW_H
