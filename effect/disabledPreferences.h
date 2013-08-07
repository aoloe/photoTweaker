#ifndef DISABLEDSCALEPREFERENCES_H
#define DISABLEDSCALEPREFERENCES_H

#include <QDebug>

#include "ui_disabledPreferences.h"

class EffectDisabled;

class DisabledPreferences : public QWidget, public Ui::DisabledPreferences
{
        Q_OBJECT

public:
    DisabledPreferences( QWidget * parent = 0);
public:
    // for some reason, enabled can't directly be used as boolean (ale/20130807)
    void setEnabled(bool enabled) {if (enabled) enabled = true; enabledCheckBox->setChecked(enabled);}
    void setEffectName(QString name) {groupBox->setTitle(name);}

public slots:
    void accept();
signals: 
    void accepted(bool enabled);
private:
    EffectDisabled* effect;
};

#endif // DISABLEDSCALEPREFERENCES_H
