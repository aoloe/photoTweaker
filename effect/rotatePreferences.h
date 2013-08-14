#ifndef ROTATEPREFERENCES_H
#define ROTATEPREFERENCES_H

#include <QDebug>

#include "ui_rotatePreferences.h"

class EffectRotate;

class RotatePreferences : public QWidget, public Ui::RotatePreferences
{
        Q_OBJECT

public:
    RotatePreferences( QWidget * parent = 0);
public:
    // for some reason, clockwise can't directly be used as boolean (ale/20130807)
    void setEnabled(bool enabled) {if (enabled) enabled = true; enabledCheckBox->setChecked(enabled);}
    void setClockwise(bool clockwise) {if (clockwise) clockwise = true; clockwiseCheckBox->setChecked(clockwise);}

public slots:
    void accept();
signals: 
    void accepted(bool enabled, bool clockwise);
private:
    EffectRotate* effect;
};

#endif // ROTATEPREFERENCES_H
