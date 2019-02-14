#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QObject>
#include <QPoint>

class ImageInfo: public QObject
{
    Q_OBJECT
public:
    ImageInfo(const QString &image, qreal scale, const QPoint &position, QObject *parent = nullptr);
    ImageInfo(const ImageInfo &image);

    QString getImage() const;
    QPoint getPosition() const;
    qreal getScale() const;

    ImageInfo& operator=(const ImageInfo &info);

private:
    QString image;
    qreal scale;
    QPoint position;
};

#endif // IMAGEINFO_H
