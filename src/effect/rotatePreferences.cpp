#include <QDebug>
#include <QSettings>
#include "rotatePreferences.h"

/**
 * |-- Rotate ---------------------|
 * | [ ] Active                    |
 * |                               |
 * | [ ] Rotate clockwise          |
 * |-------------------------------|
 */
RotatePreferences::RotatePreferences( QWidget * parent) : QWidget(parent)
{
    setupUi(this);
}

void RotatePreferences::accept()
{
    bool enabled = enabledCheckBox->isChecked();
    bool clockwise = clockwiseCheckBox->isChecked();

    emit accepted(enabled, clockwise);
}
