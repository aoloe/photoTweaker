#include <QDebug>
#include <QSettings>
#include <QAction>
#include <QToolBar>
#include "rotate.h"
#include "effect/rotatePreferences.h"
#include "photo.h"

EffectRotate::EffectRotate(QObject *parent) :
AbstractEffect(parent)
{
}

void EffectRotate::writeSettings()
{
    QSettings settings("graphicslab.org", "photoTweaker");
    settings.setValue("rotate/clockwise", clockwise);

}

void EffectRotate::readSettings()
{
    QSettings settings("graphicslab.org", "photoTweaker");
    clockwise = settings.value("rotate/clockwise", true).toBool();
}

void EffectRotate::addToToolBar(QToolBar &toolBar)
{

    QAction *action = new QAction(tr("Rotate"), this);
    if (clockwise)
    {
        action->setIcon(QIcon(":/icons/rotate_clockwise.png"));
    }
    else
    {
        action->setIcon(QIcon(":/icons/rotate_anticlockwise.png"));
    }
    connect(action, SIGNAL(triggered(bool)), this, SLOT(apply()));
    toolBar.addAction(action);
}

QWidget* EffectRotate::getPreferencesWidget()
{
    RotatePreferences* preferences = new RotatePreferences();
    preferences->setMinimumSize(preferences->size());
    preferences->setEnabled(this->enabled);
    preferences->setClockwise(this->clockwise);
    connect(preferences, SIGNAL(accepted(bool, bool)), this, SLOT(acceptPreferencesWidget(bool, bool)));
    return preferences;
}

void EffectRotate::acceptPreferencesWidget(bool enabled, bool clockwise)
{
    this->enabled = enabled;
    this->clockwise = clockwise;
}

void EffectRotate::apply()
{

    Photo* photo = mainApp->getPhoto();
    photo->addUndoInformation();

    // photo.clearSelection();
    // makeUndoCommand(imageArea);
    QImage image = photo->getImage();

    QTransform transform;
    if (clockwise)
    {
    transform.rotate(90);
    }
    else
    {
    transform.rotate(-90);
    }

     photo->setImage(image.transformed(transform));
     /* mPImageArea->resize(mPImageArea->getImage()->rect().right() + 6,
                         mPImageArea->getImage()->rect().bottom() + 6); */
     photo->update();
     // mPImageArea->clearSelection();

    photo->setEdited(true);
    // photo.setImage(image);
    photo->updateImageView();
}
