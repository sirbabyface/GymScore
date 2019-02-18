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

QGraphicsScene *MainWindow::loadInformation(int row, const QXlsx::Worksheet *sheet,
                                            const Settings &settings)
{
    if(!m_sceneInfo.isActive()) {
        setScene(&m_sceneInfo);
    }

    if(isHidden()) {
        show();
    }

    foreach(auto item, m_sceneInfo.items()) {
        // check if item need to be updated with info from a column
        QVariant columnData = item->data(KEY_COLUMN);
        if(columnData.isNull()) {
            continue;
        }

        // Get column number
        bool ok;
        int column = columnData.toInt(&ok);
        if(!ok) {
            continue;
        }
        QGraphicsTextItem* text = dynamic_cast<QGraphicsTextItem*>(item);
        QXlsx::Cell *cell = sheet->cellAt(row, column);

        // Check if column is from players
        if(settings.column(Settings::Players) == column) {
            updateItemPlayers(cell, text);
            continue;
        }

        // Check if column is from the team
        if(settings.column(Settings::Team) == column) {
            updateItemString(cell, text, tr("Equipa"));
            continue;
        }

        // Check if column is number
        if(settings.column(Settings::Number) == column) {
            updateItemString(cell, text);
            continue;
        }

        // Update column text
        if(cell == Q_NULLPTR) {
            // if cell is empty/not set
            text->setPlainText("0,00");
            continue;
        }

        if(cell->cellType() == QXlsx::Cell::NumberType) {
            updateItemFloat(cell, text);
        }
        else {
            updateItemString(cell, text);
        }
    }

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

void MainWindow::updateItemPlayers(QXlsx::Cell *cell, QGraphicsTextItem *text)
{
    if(!cell) return;

    QList<QString> players;
    QString playersNames = cell->value().toString();
    if(playersNames.indexOf("/") != -1) {
        players = playersNames.split("/", QString::SkipEmptyParts);
    }
    else if(playersNames.indexOf(";") != -1) {
        players = playersNames.split(";", QString::SkipEmptyParts);
    }
    else if(playersNames.indexOf(",") != -1) {
        players = playersNames.split(",", QString::SkipEmptyParts);
    }
    else {
        players.append(playersNames);
    }

    updatePlayersPosition(players, text);
}

QGraphicsScene *MainWindow::blank()
{
    setScene(&m_sceneBlank);
    fitInView(m_sceneBlank.sceneRect(), Qt::KeepAspectRatio);
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

        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::resizeEvent(e);
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::setupScene()
{
    m_sceneInfo.clear();
    m_sceneInfo.setBackgroundBrush(QBrush(Qt::black));
}

void MainWindow::setupScene(const Settings &settings)
{
    m_sceneInfo.clear();
    m_sceneInfo.setBackgroundBrush(QBrush(Qt::black));
    setupArea(m_sceneInfo, settings.size());

    QString fontName = settings.fontName();
    foreach (auto label, settings.labels()) {
        QFont font = QFont(fontName, label.getSize());
        if(label.getStyle() == "bold") {
            font.setBold(true);
        }

        QGraphicsTextItem *item = m_sceneInfo.addText(label.getText(), font);
        item->setDefaultTextColor(QColor(label.getColor()));
        item->setPos(label.getPosition());
        if(label.getColumn() > 0) {
            item->setData(KEY_COLUMN, label.getColumn());
        }

        if(label.getAlignment() == "center") {
            // Save position, to be used to center item
            item->setData(KEY_POSITION, label.getPosition());
        }
    }

    ImageInfo logoInfo = settings.competitionLogo();
    QGraphicsPixmapItem *logo = m_sceneInfo.addPixmap(QPixmap(logoInfo.getImage()));
    logo->setScale(logoInfo.getScale());
    logo->setPos(logoInfo.getPosition());
}

void MainWindow::setupBlankScene()
{
    m_sceneBlank.setBackgroundBrush(QBrush(Qt::black));
    setupArea(m_sceneBlank, QSize(640, 480));
}

void MainWindow::setupBlankScene(const Settings &settings)
{
    m_sceneBlank.clear();
    setupArea(m_sceneBlank, settings.size());

    QList<ImageInfo> images = settings.blankImages();
    foreach (auto info, images) {
        QGraphicsPixmapItem *image = m_sceneBlank.addPixmap(QPixmap(info.getImage()));
        image->setScale(info.getScale());
        image->update();
        image->setPos(info.getPosition());
    }
}


void MainWindow::setupRankingScene()
{
    // TODO
    m_sceneBlank.setBackgroundBrush(QBrush(Qt::black));
    //setupArea(m_sceneBlank, settings.size());

    // Rectangle to set the area
    QGraphicsRectItem *block = m_sceneRanking.addRect(0, 0, 640, 480);
    block->setPen(Qt::NoPen);
    block->setBrush(QColor(102, 153, 51));
    block->setZValue(-1);
//    block->hide();
}

void MainWindow::setupArea(QGraphicsScene &scene, const QSize &size)
{
    QGraphicsRectItem *block = scene.addRect(QRect(QPoint(0, 0), size));
    block->setPen(Qt::NoPen);
    block->setBrush(QColor(102, 153, 51));
    block->setZValue(-1);
    block->hide();
}

void MainWindow::updatePlayersPosition(const QList<QString> &players,
                                       QGraphicsTextItem *item)
{
    QString text;
    switch(players.size()) {
        case 1:
            text = players[0].trimmed();
            break;
        case 2:
        case 3:
            foreach(auto player, players) {
                text += player.trimmed() + "<br>";
            }
            break;
        case 4:
            // 2 players per line
            text += players[0].trimmed() + ", ";
            text += players[1].trimmed() + "<br>";
            text += players[2].trimmed() + ", ";
            text += players[3].trimmed() + "";
    }

    // Check if it should be centered
    if(!item->data(KEY_POSITION).isNull()) {
        text = "<p align='center'>" + text + "</p>";
        item->setHtml(text);
        QPoint point = item->data(KEY_POSITION).toPoint();
        item->adjustSize();
        point.setX(point.x() - item->boundingRect().width() / 2);
        point.setY(point.y() - item->boundingRect().height() / 2);
        item->setPos(point);
    }
    else {
        text = "<p>" + text + "</p>";
        item->setHtml(text);
    }
}
