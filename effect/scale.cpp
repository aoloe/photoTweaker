#include <QDebug>
#include <QSettings>
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

void EffectScale::writeSettings()
{
// TODO: check, store and define in the window if it's floating: WM_WINDOW_TYPE property set to WINDOW_TYPE_NORMAL
// https://github.com/LaurentGomila/SFML/issues/368#issuecomment-15143196
// http://qt-project.org/doc/qt-4.8/widgets-windowflags.html or try setWindowFlags
    QSettings settings("graphicslab.org", "photoTweaker");

    // settings.setValue("scale/enabled", enabledCheckBox->isChecked()); // TODO: enabled should be set from photoTweaker
    settings.beginWriteArray("scale/size");
    const int n = size.count();
    for (int i = 0; i < n; i++)
    {
        settings.setArrayIndex(i);
        // settings.setValue("value", listWidget->item(i)->text().toInt());
        settings.setValue("value", size[i]);
    }
    settings.endArray();
}

void EffectScale::readSettings()
{
    QSettings settings("graphicslab.org", "photoTweaker");
    // enabledCheckBox->setChecked(settings.value("scale/enabled", true).toBool()); // TODO: pass the value of enabled to photoTweaker
    int const n = settings.beginReadArray("scale/size");
    for (int i = 0; i < n; i++)
    {
        settings.setArrayIndex(i);
        size << settings.value("value").toInt();
    }
    settings.endArray();
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

    // TODO: use apply() instead of doEffect() (ale/20130807)
    connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(doEffect(const QString &)));
}

QWidget* EffectScale::getPreferencesWidget()
{
    scalePreferences = new ScalePreferences();
    scalePreferences->setMinimumSize(scalePreferences->size());
    // enabledCheckBox->setChecked(settings.value("scale/enabled", true).toBool()); // TODO: pass the value of enabled to photoTweaker
    QListWidgetItem* widgetItem;
    foreach (int item, size)
    {
        scalePreferences->addItem(item);
    }
    connect(scalePreferences, SIGNAL(accepted()), this, SLOT(acceptPreferencesWidget()));
    return scalePreferences;
}

void EffectScale::acceptPreferencesWidget()
{
    qDebug() << "acceptedPreferencesWidget";
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
