#ifndef SCORESETTING_H
#define SCORESETTING_H

#include <QObject>
#include <QPoint>

class LabelInfo: public QObject
{
Q_OBJECT
public:
    LabelInfo(const QPoint &position, const QString &text, int size,
              QString color, QString style, QObject *parent = nullptr);

    LabelInfo(const LabelInfo &label);

    QString getText() const;
    QPoint getPosition() const;
    int getSize() const;
    int getColumn() const;
    void setColumn(int value);
    QString getColor() const;
    QString getStyle() const;
    QString getAlignment() const;
    void setAlignment(const QString &value);

    LabelInfo& operator=(const LabelInfo &label);


private:
    QPoint position;
    QString text;
    int size;
    QString color;
    QString style;
    int column;
    QString alignment;
};

#endif // SCORESETTING_H
