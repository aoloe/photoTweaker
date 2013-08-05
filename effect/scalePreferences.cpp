#include <QDebug>
#include <QSettings>
#include "scalePreferences.h"

/**
 * |-- Scale ----------------------|
 * | [ ] Active                    |
 * |                               |
 * | |------------------------ -|-||
 * | |                          |^||
 * | |                          | ||
 * | |                          | ||
 * | |                          | ||
 * | |                          | ||
 * | |                          | ||
 * | |                          |v||
 * | |------------------------ -|-||
 * |                               |
 * | [ Add ] [ Remove]             |
 * |-------------------------------|
 * 
 * - The list items are editable
 * - The list items are ordered (?)
 * - The list items can only be numbers
 * - When clicking [ Add ] an empty (0?) enry is added and selected
 * - the [ Remove ] button is only activated when an entry is selected
 * - when clicking on an item it gets editable
 *
 */
ScalePreferences::ScalePreferences( QWidget * parent) : QWidget(parent)
{
    setupUi(this);

    connect (addButton, SIGNAL(clicked()), this, SLOT(addItem()));
    connect (removeButton, SIGNAL(clicked()), this, SLOT(removeItem()));
    removeButton->setEnabled(false);

    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(activateItem(QListWidgetItem *, QListWidgetItem *)));

    listWidget->setEditTriggers(QAbstractItemView::AnyKeyPressed);

    /*
    QListWidgetItem* item;
    item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, 600);
    listWidget->addItem(item);
    item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, 900);
    listWidget->addItem(item);
    */
}

void ScalePreferences::writeSettings()
{
// TODO: check, store and define in the window if it's floating: WM_WINDOW_TYPE property set to WINDOW_TYPE_NORMAL
// https://github.com/LaurentGomila/SFML/issues/368#issuecomment-15143196
// http://qt-project.org/doc/qt-4.8/widgets-windowflags.html or try setWindowFlags
    QSettings settings("graphicslab.org", "photoTweaker");

    settings.setValue("scale/active", activeCheckBox->isChecked());
    settings.beginWriteArray("scale/size");
    for (int i = 0; i < listWidget->count(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("value", listWidget->item(i)->text().toInt());
    }
    settings.endArray();
}

void ScalePreferences::readSettings()
{
    QSettings settings("graphicslab.org", "photoTweaker");
    activeCheckBox->setChecked(settings.value("scale/active", true).toBool());
    QListWidgetItem* item;
    int n = settings.beginReadArray("scale/size");
    for (int i = 0; i < n; i++)
    {
        settings.setArrayIndex(i);
        item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, settings.value("value").toInt());
        listWidget->addItem(item);
    }
    settings.endArray();
}

void ScalePreferences::addItem()
{
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, 0);
    listWidget->addItem(item);
    listWidget->sortItems();
    listWidget->item(0)->setSelected(true);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    listWidget->editItem(item);
}

void ScalePreferences::removeItem()
{
    QListWidgetItem* item = listWidget->currentItem();
    if (item)
    {
        qDebug() << "remove item";
        delete item;
    }
}

void ScalePreferences::activateItem(QListWidgetItem * itemClicked, QListWidgetItem *itemPrevious)
{
    qDebug() << "activate item";
    if (itemPrevious && (itemPrevious->text().toInt() == 0))
    {
        delete itemPrevious;
    }
    if (!removeButton->isEnabled())
    {
        qDebug() << "enable removeButton";
        removeButton->setEnabled(true);
    }

    if (listWidget->count() > 0)
    {
        if ((listWidget->count() > 1) && (listWidget->item(0)->text().toInt() == 0))
        {
            addButton->setEnabled(false);
        }
        else
        {
            addButton->setEnabled(true);
        }
        listWidget->sortItems();
        QListWidgetItem *item = listWidget->currentItem();
        if (item)
        {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            listWidget->editItem(item);
        }
    }
 
}


