#ifndef SCALEPREFERENCES_H
#define SCALEPREFERENCES_H

#include <QDebug>

#include "ui_scalePreferences.h"

class EffectScale;

class ScalePreferences : public QWidget, public Ui::ScalePreferences
{
        Q_OBJECT

public:
    ScalePreferences( QWidget * parent = 0);
public:
    // for some reason, enabled can't directly be used as boolean (ale/20130807)
    void setEnabled(bool enabled) {if (enabled) enabled = true; enabledCheckBox->setChecked(enabled);}
    void addItem(int value);

public slots:
    void addItem();
    void removeItem();
    void activateItem(QListWidgetItem * itemClicked, QListWidgetItem *itemPrevious);
    void accept();
signals: 
    void accepted(bool enabled, QList<int> list);
private:
    EffectScale* effect;
};

#endif // SCALEPREFERENCES_H
