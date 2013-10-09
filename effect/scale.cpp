#include <QDebug>
#include <QSettings>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
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
    buttonGroup = new QButtonGroup();
    buttonGroup->setExclusive(true);

    QToolButton *button;
    int id = 0; // id matches the i in the size vector
    foreach (int item, size)
    {
        button = new QToolButton();
        button->setIcon(QIcon(":/media/icons/scale.png"));
        button->setText(QString::number(item));
        button->setToolTip(tr("Scale")+ " " + QString::number(item));
        button->setFocusPolicy(Qt::NoFocus);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->setCheckable(true);
        toolBar.addWidget(button);
        buttonGroup->addButton(button, id);
        id++;

    }
    Q_ASSERT(connect(buttonGroup, SIGNAL(buttonPressed(QAbstractButton*)), this, SLOT(buttonPressed(QAbstractButton*))));
    Q_ASSERT(connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*))));
}

void EffectScale::buttonPressed(QAbstractButton* button)
{
    buttonUnchecking = button->isChecked();
}

/**
 * @brief If the button was already checked, uncheck it
 * Since button->isChecked() always returns true in buttonClicked(), we have to first check in
 * buttonPressed() if it was already checked and use that value.
 * Also, there is no way to clear the selection in an exclusive buttons group, so we have to temporarly
 * disable the exclusive flag to uncheck the button.
 */
void EffectScale::buttonClicked(QAbstractButton* button)
{
    if (buttonUnchecking)
    {
        buttonGroup->setExclusive(false);
        button->setChecked(false);
        buttonGroup->setExclusive(true);
    }
    Photo* photo = mainApp->getPhoto();
    photo->setEdited(true);
}

/**
 * @param int value the size to scale to
 */
void EffectScale::apply(QImage& image, int size)
{

    if(image.width() >= image.height())
    {
        image = image.scaledToWidth(size);
    }
    else
    {
        image = image.scaledToHeight(size);
    }
}

void EffectScale::onSave(QImage& image)
{
    // qDebug() << "checkedId" << buttonGroup->checkedId();
    int id = buttonGroup->checkedId();
    if (id > -1)
    {
        qDebug() << "size" << size[id];
        apply(image, size[id]);
    }
}

/*
void EffectScale::addToToolBar(QToolBar &toolBar)
{
    signalMapper = new QSignalMapper(this);

    QButtonGroup buttonGroup = new QButtonGroup()

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

void EffectScale::apply(const QString &value)
{
    // qDebug() << "scale effect QString" << value;

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
*/

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
