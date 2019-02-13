#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "labelinfo.h"

class Settings : public QObject
{
Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);


    bool isLoaded() const;

    QString pubFolder() const;
    void setPubFolder(const QString &folder);
    QList<LabelInfo> labels() const;
    QList<int> editableColumns() const;
    QString competitionName() const;
    QDate competitionDate() const;
    void setCompetitionName(const QString &name);
    void setCompetitionDate(const QDate &date);
    QList<QPair<QString, QString>> formulas() const;


signals:

public slots:
    void load(const QString &filename);
    void save();

private:
    QSettings *m_settings;

    const QString GROUP_COMPETITION = "Competition";
    const QString GROUP_LABELS = "Labels";
    const QString GROUP_FORMULAS = "Formulas";
    const QString GROUP_EDITABLE = "Editable";
};

#endif // SETTINGS_H
