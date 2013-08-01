#include <QDebug>
#include "scale.h"
#include "photo.h"

EffectScale::EffectScale(QObject *parent) :
AbstractEffect(parent)
{
}

/**
 * @brief EffectScale::apply
 * @param photo
 * - Get the image from photo
 * - Find out if the image is wide or high
 * - If wide, scale to width 900, if high scale to height 900
 * - Store the image in photo
 */
void EffectScale::apply(Photo &photo)
{
    QImage image = photo.getImage();
    if(image.width() >= image.height())
    {
        image = image.scaledToWidth(900);
    }
    else
    {
        image = image.scaledToHeight(900);
    }
    photo.setImage(image);
    photo.updateImageView();
    photo.setEdited(true);
}
