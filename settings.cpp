#include "settings.h"

#include <QDateTime>
#include <QDir>
#include <QPoint>
#include <QSize>

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

