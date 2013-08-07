#ifndef EFFECTPREFERENCES_H
#define EFFECTPREFERENCES_H

#include "ui_effectPreferences.h"

class EffectPreferences : public QWidget, public Ui::EffectPreferences
{
        Q_OBJECT

public:
    EffectPreferences( QWidget * parent = 0);
    void setEnabled(bool enabled) {if (enabled) enabled = true; enabledCheckBox->setChecked(enabled);}
    void setEffectName(QString name) {groupBox->setTitle(name);}
public slots:
    void accept();
signals: 
    void accepted();
};

#endif // EFFECTPREFERENCES_H
