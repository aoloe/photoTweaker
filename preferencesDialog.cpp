#include "preferencesDialog.h"
#include "effect/scalePreferences.h"

PreferencesDialog::PreferencesDialog( QWidget * parent) : QDialog(parent)
{
    setupUi(this);

    scrollArea->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout();

    ScalePreferences* scalePreferences = new ScalePreferences();
    scalePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(scalePreferences);
    ScalePreferences* scolePreferences = new ScalePreferences();
    scolePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(scolePreferences);
    ScalePreferences* sculePreferences = new ScalePreferences();
    sculePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(sculePreferences);
    scrollAreaWidgetContents->setLayout(layout);
}
