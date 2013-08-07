#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include <QSignalMapper>
#include "scale.h"
#include "effect/scalePreferences.h"
#include "photo.h"

EffectScale::EffectScale(QObject *parent) :
AbstractEffect(parent)
{
}

void EffectScale::addToToolBar(QToolBar &toolBar)
{
    signalMapper = new QSignalMapper(this);

    QToolButton *button = new QToolButton();
    button->setIcon(QIcon(":/media/icons/scale.png"));
    button->setText("800");
    button->setFocusPolicy(Qt::NoFocus);
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar.addWidget(button);

    Q_ASSERT(connect(button, SIGNAL(clicked()), signalMapper, SLOT(map())));
    signalMapper->setMapping(button, "800");

    connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(doEffect(const QString &)));
}

QWidget* EffectScale::getPreferencesWidget()
{
    ScalePreferences* scalePreferences = new ScalePreferences();
    scalePreferences->setMinimumSize(scalePreferences->size());
    return scalePreferences;
}

void EffectScale::doEffect(const QString &value)
{
    qDebug() << "scale effect QString" << value;
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
    photo.addUndoInformation();

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
