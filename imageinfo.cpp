#include "imageinfo.h"

ImageInfo::ImageInfo(const QString &image, qreal scale, const QPoint &position, QObject *parent)
    : QObject(parent), image(image), scale(scale), position(position)
{
}

ImageInfo::ImageInfo(const ImageInfo &image)
    : QObject(image.parent()), image(image.image),
      scale(image.scale), position(image.position)
{
}

QString ImageInfo::getImage() const
{
    return image;
}

QPoint ImageInfo::getPosition() const
{
    return position;
}

qreal ImageInfo::getScale() const
{
    return scale;
}

ImageInfo& ImageInfo::operator=(const ImageInfo &info)
{
    this->image = info.image;
    this->scale = info.scale;
    this->position = info.position;
    this->setParent(info.parent());
    return *this;
}
