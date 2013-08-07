#ifndef SCALEPREFERENCES_H
#define SCALEPREFERENCES_H

#include "ui_scalePreferences.h"

class EffectScale;

class ScalePreferences : public QWidget, public Ui::ScalePreferences
{
        Q_OBJECT

public:
    ScalePreferences( QWidget * parent = 0);
public:
    void addItem(int value);
    QList<int> getItemList();

public slots:
    void addItem();
    void removeItem();
    void activateItem(QListWidgetItem * itemClicked, QListWidgetItem *itemPrevious);
    void accept();
signals: 
    void accepted();
private:
    EffectScale* effect;
};

#endif // SCALEPREFERENCES_H
