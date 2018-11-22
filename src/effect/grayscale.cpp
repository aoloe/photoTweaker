#include <QDebug>
#include <QSettings>
#include <QAction>
#include <QToolBar>
#include "grayscale.h"
#include "photo.h"

EffectGrayscale::EffectGrayscale(QObject *parent) :
AbstractEffect(parent)
{
}

void EffectGrayscale::addToToolBar(QToolBar &toolBar)
{
    QAction *action = new QAction(tr("Grayscale"), this);
    action->setIcon(QIcon(":/icons/grayscale.png"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(apply()));
    toolBar.addAction(action);
}

void EffectGrayscale::apply()
{
    Photo* photo = mainApp->getPhoto();
    photo->addUndoInformation();
    // photo.clearSelection();
    // makeUndoCommand(imageArea);
    QImage image = photo->getImage();
    // qDebug() << "width" << image->width();
    // qDebug() << "height" << image->height();

    for(int i(0); i < image.width(); i++)
     {
         for(int y(0); y < image.height(); y++)
        {
            QRgb pixel(image.pixel(i, y));
            int rgb = (int)(0.299 * qRed(pixel) + 0.587 * qGreen(pixel) + 0.114 * qBlue(pixel));
            pixel = qRgb(rgb, rgb, rgb);
            image.setPixel(i, y, pixel);
        }
    }
    photo->setEdited(true);
    photo->setImage(image);
    photo->updateImageView();
}
