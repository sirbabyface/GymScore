#include <QtWidgets>

#include "3rdparty/qtxlsx/xlsxworksheet.h"

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QGraphicsView (parent)
{
    resize(640, 480);
    setScene(&m_sceneInfo);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle(tr("Gymnastic Score"));

    setupScene();
    setupBlankScene();

    setRenderHint(QPainter::Antialiasing, true);
    setFrameStyle(QFrame::NoFrame);

    setBackgroundBrush(QBrush(Qt::black));
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *pQEvent)
{
    fitInView(m_sceneInfo.sceneRect(), Qt::KeepAspectRatio);
    QGraphicsView::paintEvent(pQEvent);
}


void MainWindow::loadInformation(int row, const QXlsx::Worksheet *sheet)
{
    if(!m_sceneInfo.isActive()) {
        setScene(&m_sceneInfo);
    }

    //updateItem(sheet->cellAt(row, 1), m_type);
    updateItemString(sheet->cellAt(row, 2), m_number);
    updateItemPlayers(sheet->cellAt(row, 3));
    updateItemString(sheet->cellAt(row, 4), m_level);
    updateItemString(sheet->cellAt(row, 5), m_team);
    updateItemFloat(sheet->cellAt(row, 12), m_difference);
    updateItemFloat(sheet->cellAt(row, 13), m_penalization);
    updateItemFloat(sheet->cellAt(row, 14), m_scoreExecution);
    updateItemFloat(sheet->cellAt(row, 15), m_scoreArtistic);
    updateItemFloat(sheet->cellAt(row, 16), m_finalScore);
}

void MainWindow::updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item)
{
    if(!cell) return;

    QString value = cell->value().toString();
    item->setPlainText(value);
}

void MainWindow::updateItemFloat(QXlsx::Cell *cell, QGraphicsTextItem *item)
{
    if(!cell) return;

    QString value = cell->value().toString();
    bool ok;
    float numberF = value.toFloat(&ok);
    if(!ok) return;

    int number = qRound(numberF * 100);
    value = QString::number(number / 100) + "," + QString::number(number % 100);
    item->setPlainText(value);
}

void MainWindow::updateItemPlayers(QXlsx::Cell *cell)
{
    if(!cell) return;

    QList<QString> players = cell->value().toString().split("/");
    updatePlayersPosition(players);
}

void MainWindow::blank()
{
    setScene(&m_sceneBlank);
}

void MainWindow::mouseDoubleClickEvent( QMouseEvent * e )
{
    if(e->button() == Qt::LeftButton) {
        if(isFullScreen()) {
            showNormal();
        }
        else {
            showFullScreen();
        }
    }
}

void MainWindow::setupScene()
{
    QString fontFamily = "Calibri";
    QFont fontLabel;
    fontLabel.setFamily(fontFamily);
    fontLabel.setPointSize(32);
    fontLabel.setBold(true);

    QFont fontLabelFinalScore;
    fontLabelFinalScore.setFamily(fontFamily);
    fontLabelFinalScore.setPointSize(42);
    fontLabelFinalScore.setBold(true);

    QFont fontPlayers;
    fontPlayers.setPointSize(28);
    fontPlayers.setBold(true);

    QFont fontScore;
    fontScore.setFamily(fontFamily);
    fontScore.setPointSize(72);
    fontScore.setBold(true);

    // Rectangle to set the area
    QGraphicsRectItem *block = m_sceneInfo.addRect(0, 0, 640, 480);
    block->setPen(Qt::NoPen);
    block->setBrush(QColor(102, 153, 51));
    block->setZValue(-1);
    block->hide();

    QGraphicsTextItem *labelExecution = m_sceneInfo.addText(tr("EXE"), fontLabel);
    setItem(labelExecution, 40, 265);

    QGraphicsTextItem *labelArtistic = m_sceneInfo.addText(tr("ART"), fontLabel);
    setItem(labelArtistic, 40, 310);

    QGraphicsTextItem *labelFinalScore = m_sceneInfo.addText(tr("Nota Final"), fontLabelFinalScore);
    setItem(labelFinalScore, 40, 380);

    m_finalScore = m_sceneInfo.addText("0,00", fontScore);
    m_finalScore->setDefaultTextColor(QColor(255, 192, 0));
    m_finalScore->setPos(350, 340);

    QGraphicsTextItem *labelDifference = m_sceneInfo.addText(tr("DIF"), fontLabel);
    setItem(labelDifference, 350, 265);

    QGraphicsTextItem *labelPenalty = m_sceneInfo.addText(tr("PEN"), fontLabel);
    setItem(labelPenalty, 350, 310);

    m_scoreExecution = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_scoreExecution, 150, 265);

    m_scoreArtistic = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_scoreArtistic, 150, 310);

    m_difference = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_difference, 460, 265);

    m_penalization = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_penalization, 460, 310);


    for (int i = 0; i < 3; ++i) {
        m_players[i] = m_sceneInfo.addText("", fontPlayers);
        m_players[i]->setDefaultTextColor(QColor(102, 255, 204));
    }

    QList<QString> players;
    players << "Carolina Félix" << "Jorge";
    players << "Carolina Félix";
    updatePlayersPosition(players);

    m_level = m_sceneInfo.addText("NIVEL 1   PF", fontLabel);
    setItem(m_level, 40, 75);


    m_team = m_sceneInfo.addText("ACM", fontLabel);
    setItem(m_team, 200, 20);

    m_number = m_sceneInfo.addText("34", fontLabel);
    setItem(m_number, 100, 20);

    m_teamIcon = m_sceneInfo.addPixmap(QPixmap(":/img/acm.png"));
    m_teamIcon->setScale(0.1);
    m_teamIcon->setPos(40, 20);
}

void MainWindow::setupBlankScene()
{
    m_sceneBlank.setBackgroundBrush(QBrush(Qt::black));

    QGraphicsPixmapItem *acm = m_sceneBlank.addPixmap(QPixmap(":/img/acm.png"));
    acm->setScale(0.7);
    acm->setPos(320 - acm->boundingRect().width() / 2, 240 - acm->boundingRect().height() / 2);
}

void MainWindow::updatePlayersPosition(const QList<QString> &players)
{
    switch(players.size()) {
        case 2:
            m_players[0]->setPlainText(players[0]);
            m_players[0]->setPos(320 - m_players[0]->boundingRect().width() / 2, 140);
            m_players[1]->setPlainText(players[1]);
            m_players[1]->setPos(320 - m_players[1]->boundingRect().width() / 2, 180);
            m_players[2]->setPlainText("");
            break;
        case 3:
            m_players[0]->setPlainText(players[0]);
            m_players[0]->setPos(320 - m_players[0]->boundingRect().width() / 2, 125);
            m_players[1]->setPlainText(players[1]);
            m_players[1]->setPos(320 - m_players[1]->boundingRect().width() / 2, 165);
            m_players[2]->setPlainText(players[2]);
            m_players[2]->setPos(320 - m_players[2]->boundingRect().width() / 2, 205);
            break;
        case 4:
            m_players[0]->setPlainText(players[0] + ", " + players[1]);
            m_players[0]->setPos(320 - m_players[0]->boundingRect().width() / 2, 130);
            m_players[1]->setPlainText(players[2] + ", " + players[3]);
            m_players[1]->setPos(320 - m_players[1]->boundingRect().width() / 2, 170);
            m_players[2]->setPlainText("");
            break;
        default:
            break;
    }
}

void MainWindow::setItem(QGraphicsTextItem *item, int x, int y)
{
    item->setDefaultTextColor(Qt::white);
    item->setPos(x, y);
}
