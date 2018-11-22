#include <QDebug>
#include <QSettings>
#include "disabledPreferences.h"

/**
 * |-- Disabled ----------------------|
 * | [ ] Enabled                   |
 * |                               |
 * | Will be enabled on restart    |
 * |                               |
 * | [ Add ] [ Remove]             |
 * |-------------------------------|
 */
DisabledPreferences::DisabledPreferences( QWidget * parent) : QWidget(parent)
{
    setupUi(this);
}


void DisabledPreferences::accept()
{
    bool enabled = enabledCheckBox->isChecked();
    emit accepted(enabled);
}
