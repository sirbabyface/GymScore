#ifndef INFORMATION_H
#define INFORMATION_H

#include <QString>
#include <QList>
#include <QImage>

class Information
{
public:
    Information();
    Information(const QString &team, const QList<QString> &players);
    Information(const QString &team, const QList<QString> &players, QImage *teamIcon);

    float finalScore() const;

    QImage *teamIcon() const;
    void setTeamIcon(QImage *teamIcon);

    QList<QString> players() const;
    void setPlayers(const QList<QString> &players);
    void addPlayer(const QString player);

    QString team() const;
    void setTeam(const QString &team);

    float scoreExecution() const;
    void setScoreExecution(float scoreExecution);

    float penalization() const;
    void setPenalization(float penalization);

    float scoreArtistic() const;
    void setScoreArtistic(float scoreArtistic);

private:
    uint number;
    QString m_team;
    QList<QString> m_players;
    float m_scoreExecution;
    float m_penalization;
    float m_scoreArtistic;
    QImage *m_teamIcon;
};

#endif // INFORMATION_H
