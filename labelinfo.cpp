#include "labelinfo.h"

LabelInfo::LabelInfo(const QPoint &position, const QString &text,
                     int size, QString color, QString style, QObject *parent)
    : QObject(parent), position(position), text(text), size(size), color(color),
      style(style), column(0), alignment("left")
{
}


LabelInfo::LabelInfo(const LabelInfo &label)
    : QObject(label.parent()), position(label.position), text(label.text),
      size(label.size), color(label.color), style(label.style),
      column(label.column), alignment(label.alignment)
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

QString LabelInfo::getColor() const
{
    return color;
}

QString LabelInfo::getStyle() const
{
    return style;
}

int LabelInfo::getColumn() const
{
    return column;
}

LabelInfo &LabelInfo::operator=(const LabelInfo &label)
{
    text = label.text;
    position = label.position;
    size = label.size;
    column = label.column;
    color = label.color;
    style = label.style;
    alignment = label.alignment;
    // Should parent be copied?
    //setParent(score.parent());
    return *this;
}

void LabelInfo::setColumn(int value)
{
    column = value;
}

void LabelInfo::setAlignment(const QString &value)
{
    alignment = value;
}

QString LabelInfo::getAlignment() const
{
    return alignment;
}
