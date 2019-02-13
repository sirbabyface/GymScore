#include "settings.h"

#include <QDateTime>
#include <QDir>
#include <QPoint>


Settings::Settings(QObject *parent) : QObject(parent),
                                      m_settings(nullptr)
{

}



bool Settings::isLoaded() const
{
    return m_settings != nullptr;
}

QString Settings::pubFolder() const
{
    if(!isLoaded()) {
        return QDir::currentPath() + "pub";
    }

    return m_settings->value(GROUP_COMPETITION + "/pubFolder", "pub").toString();
}

void Settings::setPubFolder(const QString &folder)
{
    if(!isLoaded()) {
        return;
    }

    // Convert folder to relative path
    QString relativeFolder = QDir::current().relativeFilePath(folder);

    m_settings->setValue(GROUP_COMPETITION + "/pubFolder", relativeFolder);
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
        if(m_settings->contains("column"))  {
            int column = m_settings->value("column").toInt();
            list.push_back(LabelInfo(position, text, size, color, column));
        }
        else {
            list.push_back(LabelInfo(position, text, size, color));
        }
        m_settings->endGroup();
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

    auto columns = m_settings->value(GROUP_EDITABLE + "/columns").toString();
    foreach (auto column, columns.split(",")) {
        bool ok;
        int number = column.toInt(&ok);
        if(ok) {
            list.push_back(number);
        }
    }

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

QList<QPair<QString, QString> > Settings::formulas() const
{
    QList<QPair<QString, QString>> list;
    if(!isLoaded()) {
        return list;
    }

    m_settings->beginGroup(GROUP_FORMULAS);
    QStringList keys = m_settings->childKeys();
    foreach (auto key, keys) {
        auto node = QPair<QString, QString>(key, m_settings->value(key).toString());
        list.push_back(node);
    }

    m_settings->endGroup();

    return list;
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

