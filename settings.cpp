#include "settings.h"

#include <QDateTime>
#include <QDir>
#include <QPoint>
#include <QSize>
#include <algorithm>

Settings::Settings(QObject *parent) : QObject(parent),
                                      m_settings(nullptr)
{

}



bool Settings::isLoaded() const
{
    return m_settings != nullptr;
}

QSize Settings::size() const
{
    QSize size;
    if(!isLoaded()) {
        return size;
    }

    size.setWidth(m_settings->value(GROUP_COMPETITION + "/width", 640).toInt());
    size.setHeight(m_settings->value(GROUP_COMPETITION + "/height", 480).toInt());

    return size;
}

QString Settings::fontName() const
{
     if(!isLoaded()) {
         return "Arial";
     }
     return m_settings->value(GROUP_COMPETITION + "/font", "Arial").toString();
}


QList<LabelInfo> Settings::labels() const
{
    QList<LabelInfo> list;

    if(!isLoaded()) {
        return list;
    }

    int size = m_settings->beginReadArray(GROUP_LABELS);

    for (int i = 0; i < size; ++i) {

        m_settings->setArrayIndex(i);

        QPoint position = m_settings->value("position").toPoint();
        QString text = m_settings->value("text").toString();
        int size = m_settings->value("size").toInt();
        QString color = m_settings->value("color").toString();
        QString style = m_settings->value("style").toString();
        LabelInfo label(position, text, size, color, style);

        if(m_settings->contains("column"))  {
            int column = m_settings->value("column").toInt();
            label.setColumn(column);
        }

        if(m_settings->contains("alignment"))  {
            QString alignment = m_settings->value("alignment").toString();
            label.setAlignment(alignment);
        }
        list.push_back(label);
    }

    m_settings->endArray();

    return list;
}

/**
 * @brief Returns the list of columns which are editable in the table.
 * @return
 */
QList<int> Settings::editableColumns() const
{
    QList<int> list;
    if(!isLoaded()) {
        return list;
    }

    m_settings->beginGroup(GROUP_COLUMNS);
    auto columns = m_settings->value("editable").toStringList();

    foreach (auto column, columns) {
        bool ok;
        int number = column.toInt(&ok);
        if(ok) {
            list.push_back(number);
        }
    }
    m_settings->endGroup();

    return list;
}

/**
 * @brief Returns the competion name.
 * @return Competition name
 */
QString Settings::competitionName() const
{
    if(!isLoaded()) {
        return "COMPETION NAME";
    }

    return m_settings->value(GROUP_COMPETITION + "/name", tr("Undefined")).toString();
}

/**
 * @brief Returns the competition date. If not set uses the current date.
 * @return
 */
QDate Settings::competitionDate() const
{
    if(!isLoaded()) {
        return QDate::currentDate();
    }

    return m_settings->value(GROUP_COMPETITION + "/date", QDate::currentDate()).toDate();
}

ImageInfo Settings::competitionLogo() const
{
    if(!isLoaded()) {
        return ImageInfo("", 1, QPoint());
    }

    QString image = m_settings->value(GROUP_COMPETITION + "/logo/image").toString();
    qreal scale = m_settings->value(GROUP_COMPETITION + "/logo/scale").toReal();
    QPoint position = m_settings->value(GROUP_COMPETITION + "/logo/position").toPoint();

    return ImageInfo(image, scale, position);
}

void Settings::setCompetitionName(const QString &name)
{
    if(!isLoaded()) {
        return;
    }
    m_settings->setValue(GROUP_COMPETITION + "/name", name);
}

void Settings::setCompetitionDate(const QDate &date)
{
    if(!isLoaded()) {
        return;
    }
    m_settings->setValue(GROUP_COMPETITION + "/date", date);
}


QList<QPair<int, QString>> Settings::formulas() const
{
    QList<QPair<int, QString>> list;
    if(!isLoaded()) {
        return list;
    }

    m_settings->beginGroup(GROUP_FORMULAS);
    QStringList keys = m_settings->childKeys();
    // Sort the keys
    std::sort(keys.begin(), keys.end(), sortNumericStrings);

    foreach (auto key, keys) {
        bool ok;
        int column = key.toInt(&ok);
        if(!ok) {
            continue; // skip, since key is not valid integer
        }
        QPair<int, QString> node = QPair<int, QString>(column, m_settings->value(key).toString());
        list.push_back(node);
    }

    m_settings->endGroup();

    return list;
}

QList<ImageInfo> Settings::blankImages() const
{

    QList<ImageInfo> list;

    if(!isLoaded()) {
        return list;
    }

    m_settings->beginGroup(GROUP_BLANK);

    int size = m_settings->beginReadArray("images");

    for (int i = 0; i < size; ++i) {
        m_settings->setArrayIndex(i);
        QString image = m_settings->value("image").toString();
        qreal scale = m_settings->value("scale").toReal();
        QPoint position = m_settings->value("position").toPoint();
        list.push_back(ImageInfo(image, scale, position));
    }

    m_settings->endArray();
    m_settings->endGroup();

    return list;
}

/**
 * @brief Returns the column number used by the type
 */
int Settings::column(Settings::Column type) const
{
    switch (type) {
    case Players:
        return m_settings->value(GROUP_COLUMNS + "/players").toInt();
    case Team:
        return m_settings->value(GROUP_COLUMNS + "/team").toInt();
    case Level:
        return m_settings->value(GROUP_COLUMNS + "/level").toInt();
    case Type:
        return m_settings->value(GROUP_COLUMNS + "/type").toInt();
    case Club:
        return m_settings->value(GROUP_COLUMNS + "/club").toInt();
    case Number:
        return m_settings->value(GROUP_COLUMNS + "/number").toInt();
    }
    return 0;
}

/**
 * @brief Loads competition configurations from filename.
 *
 * TODO: Add list of configurations available
 *
 * @param filename Ini filename with competition configurations
 */
void Settings::load(const QString &filename)
{
    if(m_settings != nullptr) {
        delete m_settings;
    }
    m_settings = new QSettings(filename, QSettings::IniFormat, this);
    m_settings->setIniCodec("UTF-8");
}

/**
 * @brief Saves any change made to the settings in the file opened.
 */
void Settings::save()
{
    if(isLoaded()) {
        m_settings->sync();
    }
}

bool Settings::sortNumericStrings(const QString &s1, const QString &s2)
{
    int n1 = s1.toInt();
    int n2 = s2.toInt();
    return n1 < n2;
}

