#include <QDebug>
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

    QListWidgetItem* item;
    item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, 600);
    listWidget->addItem(item);
    item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, 900);
    listWidget->addItem(item);

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


