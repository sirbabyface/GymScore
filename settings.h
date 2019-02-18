#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "labelinfo.h"
#include "imageinfo.h"

class Settings : public QObject
{
Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    enum Column {
        Players,
        Team,
        Level,
        Type,
        Club,
        Number
    };

    bool isLoaded() const;

    QSize size() const;
    QString fontName() const;

    QList<LabelInfo> labels() const;
    QList<int> editableColumns() const;
    QString competitionName() const;
    QDate competitionDate() const;
    ImageInfo competitionLogo() const;
    void setCompetitionName(const QString &name);
    void setCompetitionDate(const QDate &date);
    QList<QPair<int, QString> > formulas() const;
    QList<ImageInfo> blankImages() const;
    int column(Column type) const;


signals:

public slots:
    void load(const QString &filename);
    void save();

private:
    static bool sortNumericStrings(const QString &s1, const QString &s2);

private:
    QSettings *m_settings;

    const QString GROUP_COMPETITION = "Competition";
    const QString GROUP_LABELS = "Labels";
    const QString GROUP_FORMULAS = "Formulas";
    const QString GROUP_COLUMNS = "Columns";
    const QString GROUP_BLANK = "Blank";
};

#endif // SETTINGS_H
