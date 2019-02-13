#ifndef SCORESETTING_H
#define SCORESETTING_H

#include <QObject>
#include <QPoint>

class LabelInfo: public QObject
{
Q_OBJECT
public:
    LabelInfo(const QPoint &position, const QString &text, int size, QString color, QObject *parent = nullptr);
    LabelInfo(const QPoint &position, const QString &text, int size, QString color, int column, QObject *parent = nullptr);
    LabelInfo(const LabelInfo &label);

    QString getText() const;
    QPoint getPosition() const;
    int getSize() const;
    int getColumn() const;

    LabelInfo& operator=(const LabelInfo &label);

    QString getColor() const;

private:
    QPoint position;
    QString text;
    int size;
    QString color;
    int column;
};

#endif // SCORESETTING_H
