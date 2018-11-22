#include <QDebug>
#include "effectPreferences.h"

/**
 * This is the basic widget for effects that do not have preferences. It only offers the option
 * to disable the effets.
 *
 * |-- Scale ----------------------|
 * | [ ] Enabled                   |
 * |-------------------------------|
 * 
 */
EffectPreferences::EffectPreferences( QWidget * parent) : QWidget(parent)
{
    setupUi(this);
}

void EffectPreferences::accept()
{
    emit accepted();
}
