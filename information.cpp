#include "information.h"

Information::Information():
    m_scoreExecution(0), m_scoreArtistic(0), m_teamIcon(Q_NULLPTR)
{
}

Information::Information(const QString &team, const QList<QString> &players):
    Information(team, players, Q_NULLPTR)
{
}

Information::Information(const QString &team, const QList<QString> &players, QImage *teamIcon):
    m_team(team), m_players(players), m_scoreExecution(0), m_scoreArtistic(0),
    m_teamIcon(teamIcon)
{
}

float Information::finalScore() const
{
    return m_scoreArtistic + m_scoreExecution + m_penalization;
}


QImage *Information::teamIcon() const
{
    return m_teamIcon;
}

void Information::setTeamIcon(QImage *teamIcon)
{
    m_teamIcon = teamIcon;
}

QList<QString> Information::players() const
{
    return m_players;
}

void Information::setPlayers(const QList<QString> &players)
{
    m_players = players;
}

void Information::addPlayer(const QString player)
{
    m_players.append(player);
}

QString Information::team() const
{
    return m_team;
}

void Information::setTeam(const QString &team)
{
    m_team = team;
}

float Information::scoreExecution() const
{
    return m_scoreExecution;
}

void Information::setScoreExecution(float scoreExecution)
{
    m_scoreExecution = scoreExecution;
}

float Information::penalization() const
{
    return m_penalization;
}

void Information::setPenalization(float penalization)
{
    m_penalization = penalization;
}

float Information::scoreArtistic() const
{
    return m_scoreArtistic;
}

void Information::setScoreArtistic(float scoreArtistic)
{
    m_scoreArtistic = scoreArtistic;
}
