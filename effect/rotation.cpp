#include <QDebug>
#include <QToolBar>
#include "rotation.h"
#include "photo.h"

EffectRotation::EffectRotation(QObject *parent) :
AbstractEffect(parent)
{
}

void EffectRotation::addToToolBar(QToolBar &toolbar)
{
}

void EffectRotation::apply(Photo &photo)
{

    photo.addUndoInformation();

    // photo.clearSelection();
    // makeUndoCommand(imageArea);
    QImage image = photo.getImage();

    QTransform transform;
    transform.rotate(90);
    // transform.rotate(-90);

     photo.setImage(image.transformed(transform));
     /* mPImageArea->resize(mPImageArea->getImage()->rect().right() + 6,
                         mPImageArea->getImage()->rect().bottom() + 6); */
     photo.update();
     // mPImageArea->clearSelection();






    photo.setEdited(true);
    // photo.setImage(image);
    photo.updateImageView();
}
