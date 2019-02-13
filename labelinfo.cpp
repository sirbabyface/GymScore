#include "labelinfo.h"

LabelInfo::LabelInfo(const QPoint &position, const QString &text,
                           int size, QString color, QObject *parent):
    QObject(parent), position(position), text(text), size(size), color(color), column(0)
{
}

LabelInfo::LabelInfo(const QPoint &position, const QString &text,
                     int size, QString color, int column, QObject *parent):
    QObject(parent), position(position), text(text), size(size), color(color), column(column)
{
}

LabelInfo::LabelInfo(const LabelInfo &label):
    QObject(label.parent()), position(label.position), text(label.text),
    size(label.size), color(label.color), column(label.column)
{
}

QString LabelInfo::getText() const
{
    return text;
}

QPoint LabelInfo::getPosition() const
{
    return position;
}

int LabelInfo::getSize() const
{
    return size;
}

LabelInfo &LabelInfo::operator=(const LabelInfo &label)
{
    text = label.text;
    position = label.position;
    size = label.size;
    column = label.column;
    color = label.color;
    // Should parent be copied?
    //setParent(score.parent());
    return *this;
}

QString LabelInfo::getColor() const
{
    return color;
}

int LabelInfo::getColumn() const
{
    return column;
}
