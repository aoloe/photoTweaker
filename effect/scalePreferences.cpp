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

    // make the items in the list editable and numeric (necessary for sorting)
    // TODO: find a way to remove the spinbox and to improve the way the items are shown. (ale/20130807)
    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(activateItem(QListWidgetItem *, QListWidgetItem *)));
    listWidget->setEditTriggers(QAbstractItemView::AnyKeyPressed);
}

/**
 * @short add an item from the scale effect
 */
void ScalePreferences::addItem(int value)
{
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, value);
    listWidget->addItem(item);
}

/**
 * @short add an empty item when reacting to a click on the add button
 */
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

QList<int> ScalePreferences::getItemList()
{
    QList<int>result;
    // foreach does not work, because there is no way to get all the QListWidgetItems in listWidget
    const int n = listWidget->count();
    for (int i = 0; i < n; i++)
    {
        result << listWidget->item(i)->text().toInt();
    }
    return result;
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

void ScalePreferences::accept()
{
    emit accepted();
}
