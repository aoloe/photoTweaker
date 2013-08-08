#include <QDebug>
#include <QSettings>
#include <QToolBar>
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
    QSettings settings("graphicslab.org", "photoTweaker");

    settings.beginWriteArray("scale/size");
    const int n = size.count();
    for (int i = 0; i < n; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("value", size[i]);
    }
    settings.endArray();
}

void EffectScale::readSettings()
{
    QSettings settings("graphicslab.org", "photoTweaker");
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

    QToolButton *button;
    foreach (int item, size)
    {
        button = new QToolButton();
        button->setIcon(QIcon(":/media/icons/scale.png"));
        button->setText(QString::number(item));
        button->setToolTip(tr("Scale")+ " " + QString::number(item));
        button->setFocusPolicy(Qt::NoFocus);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar.addWidget(button);

        Q_ASSERT(connect(button, SIGNAL(clicked()), signalMapper, SLOT(map())));
        signalMapper->setMapping(button, QString::number(item));
    }

    connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(apply(const QString &)));
}

QWidget* EffectScale::getPreferencesWidget()
{
    ScalePreferences* scalePreferences = new ScalePreferences();
    scalePreferences->setMinimumSize(scalePreferences->size());
    scalePreferences->setEnabled(this->enabled);
    QListWidgetItem* widgetItem;
    foreach (int item, size)
    {
        scalePreferences->addItem(item);
    }
    connect(scalePreferences, SIGNAL(accepted(bool, QList<int>)), this, SLOT(acceptPreferencesWidget(bool, QList<int>)));
    return scalePreferences;
}

void EffectScale::acceptPreferencesWidget(bool enabled, QList<int> size)
{
    this->enabled = enabled;
    this->size = size;
}

void EffectScale::apply(const QString &value)
{
    qDebug() << "scale effect QString" << value;

    const int size = value.toInt();

    Photo* photo = mainApp->getPhoto();
    photo->addUndoInformation();

    QImage image = photo->getImage();
    if(image.width() >= image.height())
    {
        image = image.scaledToWidth(size);
    }
    else
    {
        image = image.scaledToHeight(size);
    }
    photo->setImage(image);
    photo->updateImageView();
    photo->setEdited(true);
}
