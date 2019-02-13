#include <QtWidgets>

#include "3rdparty/qtxlsx/xlsxworksheet.h"

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QGraphicsView (parent)
{
    setWindowIcon(QIcon(":/img/easel.png"));
    resize(640, 480);
    setScene(&m_sceneBlank);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle(tr("Projection"));

    setupScene();
    setupBlankScene();
    setupRankingScene();

    setRenderHint(QPainter::Antialiasing, true);
    setFrameStyle(QFrame::NoFrame);

    setBackgroundBrush(QBrush(Qt::black));

    // For debugging
    // setScene(&m_sceneRanking);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *pQEvent)
{
    fitInView(m_sceneInfo.sceneRect(), Qt::KeepAspectRatio);
    QGraphicsView::paintEvent(pQEvent);
}


QGraphicsScene *MainWindow::loadInformation(int row, const QXlsx::Worksheet *sheet)
{
    if(!m_sceneInfo.isActive()) {
        setScene(&m_sceneInfo);
    }

    if(isHidden()) {
        show();
    }

    updateItemString(sheet->cellAt(row, 1), m_type);
    updateItemString(sheet->cellAt(row, 2), m_number);
    updateItemPlayers(sheet->cellAt(row, 3));
    updateItemString(sheet->cellAt(row, 4), m_level);
    updateItemStringCenter(sheet->cellAt(row, 5), m_club);
    updateItemString(sheet->cellAt(row, 6), m_team, tr("Equipa"));
    updateItemFloat(sheet->cellAt(row, 13), m_difficulty);
    updateItemFloat(sheet->cellAt(row, 14), m_penalization);
    updateItemFloat(sheet->cellAt(row, 15), m_scoreExecution);
    updateItemFloat(sheet->cellAt(row, 16), m_scoreArtistic);
    updateItemFloat(sheet->cellAt(row, 17), m_finalScore);

    return &m_sceneInfo;
}

void MainWindow::updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item)
{
    if(cell) {
        item->setPlainText(cell->value().toString());
    }
}

void MainWindow::updateItemStringCenter(QXlsx::Cell *cell, QGraphicsTextItem *item)
{
    if(cell) {
        item->setPlainText(cell->value().toString());
        item->setPos(320 - item->boundingRect().width() / 2 * item->scale(), item->y());
    }
}

void MainWindow::updateItemString(QXlsx::Cell *cell, QGraphicsTextItem *item, const QString &prefix)
{
    QString value;
    if(!cell) {
        value = prefix + " -";
    }
    else {
        value = prefix + " " + cell->value().toString();
    }
    item->setPlainText(value);
}

void MainWindow::updateItemFloat(QXlsx::Cell *cell, QGraphicsTextItem *item)
{
    if(!cell) {
        item->setPlainText("0,00");
        return;
    }

    QString value = cell->value().toString();
    bool ok;
    float numberF = value.toFloat(&ok);
    if(!ok) {
        item->setPlainText("0,00");
        return;
    }

    int number = qRound(numberF * 100);
    bool isNegative = false;
    if(number < 0) {
        isNegative = true;
        number = number * -1;
    }
    QString decimalPart = QString::number(number % 100);
    if(decimalPart.size() == 1) {
        decimalPart = "0" + decimalPart;
    }
    value = QString::number(number / 100) + "," + decimalPart;

    if(isNegative) {
        value = "-" + value;
    }
    item->setPlainText(value);
}

void MainWindow::updateItemPlayers(QXlsx::Cell *cell)
{
    if(!cell) return;

    QList<QString> players = cell->value().toString().split("/");
    updatePlayersPosition(players);
}

QGraphicsScene *MainWindow::blank()
{
    setScene(&m_sceneBlank);
    if(isHidden()) {
        show();
    }
    return &m_sceneBlank;
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

    QGraphicsTextItem *labelDifficulty = m_sceneInfo.addText(tr("DIF"), fontLabel);
    setItem(labelDifficulty, 350, 265);

    QGraphicsTextItem *labelPenalty = m_sceneInfo.addText(tr("PEN"), fontLabel);
    setItem(labelPenalty, 350, 310);

    m_scoreExecution = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_scoreExecution, 150, 265);

    m_scoreArtistic = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_scoreArtistic, 150, 310);

    m_difficulty = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_difficulty, 460, 265);

    m_penalization = m_sceneInfo.addText("0,00", fontLabel);
    setItem(m_penalization, 460, 310);


    for (int i = 0; i < 3; ++i) {
        m_players[i] = m_sceneInfo.addText("", fontPlayers);
        m_players[i]->setDefaultTextColor(QColor(102, 255, 204));
    }

    QList<QString> players;
    players << "Tiago Correia" << "Mariana Correia";
    players << "Matilde Fonseca";
    updatePlayersPosition(players);

    m_level = m_sceneInfo.addText("NIVEL 1", fontLabel);
    setItem(m_level, 120, 75);

    m_type = m_sceneInfo.addText("PF", fontLabel);
    setItem(m_type, 40, 75);

    m_club = m_sceneInfo.addText("ACM", fontLabel);
    setItem(m_club, 240, 20);

    m_team = m_sceneInfo.addText("Equipa A", fontLabel);
    setItem(m_team, 460, 75);

    m_number = m_sceneInfo.addText("34", fontLabel);
    setItem(m_number, 120, 20);

    m_teamIcon = m_sceneInfo.addPixmap(QPixmap(":/img/acm.png"));
    m_teamIcon->setScale(0.1);
    m_teamIcon->setPos(40, 20);
}

void MainWindow::setupScene(const Settings &settings)
{

}

void MainWindow::setupBlankScene()
{
    m_sceneBlank.setBackgroundBrush(QBrush(Qt::black));

    // Rectangle to set the area
    QGraphicsRectItem *block = m_sceneBlank.addRect(0, 0, 640, 480);
    block->setPen(Qt::NoPen);
    block->setBrush(QColor(102, 153, 51));
    block->setZValue(-1);
    block->hide();


    acm = m_sceneBlank.addPixmap(QPixmap(":/img/acm.png"));
    acm->setScale(0.6);
    acm->update();
    acm->setPos(320 - acm->boundingRect().width() * 0.6 / 2, 200 - acm->boundingRect().height() * 0.6 / 2);
}

void MainWindow::setupRankingScene()
{
    m_sceneBlank.setBackgroundBrush(QBrush(Qt::black));

    // Rectangle to set the area
    QGraphicsRectItem *block = m_sceneRanking.addRect(0, 0, 640, 480);
    block->setPen(Qt::NoPen);
    block->setBrush(QColor(102, 153, 51));
    block->setZValue(-1);
//    block->hide();

    // Fonts
    QString fontFamily = "Calibri";
    QFont fontTitle;
    fontTitle.setFamily(fontFamily);
    fontTitle.setPointSize(42);
    fontTitle.setBold(true);

    QFont fontLabel;
    fontLabel.setFamily(fontFamily);
    fontLabel.setPointSize(32);
    fontLabel.setBold(true);

    QFont fontPlayers;
    fontPlayers.setFamily(fontFamily);
    fontPlayers.setPointSize(16);
    fontPlayers.setBold(true);

    // Logo on top
    QGraphicsPixmapItem *logo = m_sceneRanking.addPixmap(QPixmap(":/img/acm.png"));
    logo->setScale(0.1);
    logo->setPos(40, 20);

    // Create Items
    m_rankingTitle = m_sceneRanking.addText("RANKING", fontTitle);
    setItem(m_rankingTitle, 120, 10);

    m_rankingLevel = m_sceneRanking.addText("NIVEL 1", fontLabel);
    setItem(m_rankingLevel, 60, 75);

    m_rankingType = m_sceneRanking.addText("GF", fontLabel);
    setItem(m_rankingType, 520, 75);

    int firstRowY = 135;
    int interval = 80;
    for (int i = 0; i < 4; ++i) {
        m_rankingPosition[i] = m_sceneRanking.addText(QString::number(i + 1) + ".", fontPlayers);
        setItem(m_rankingPosition[i], 40, firstRowY + interval * i);

        m_rankingClub[i] = m_sceneRanking.addText("ACM", fontPlayers);
        setItem(m_rankingClub[i], 90, firstRowY + interval * i);

        m_rankingPlayers[i] = m_sceneRanking.addText("Francisca Sena, Sara Machado\nJoana Conceição", fontPlayers);
        setItem(m_rankingPlayers[i], 90, firstRowY + 25 + interval * i);

        m_rankingScore[i] = m_sceneRanking.addText("22,21", fontPlayers);
        setItem(m_rankingScore[i], 480, firstRowY + interval * i);
    }

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
