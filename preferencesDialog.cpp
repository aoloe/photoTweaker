#include <QDebug>
#include "preferencesDialog.h"
#include "effect/scalePreferences.h"

PreferencesDialog::PreferencesDialog( QWidget * parent) : QDialog(parent)
{
    setupUi(this);

    scrollArea->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout();

    // TODO: get the preferences from each effects
    qDebug() << "effects.count" << effects.count();

    // only as sample beceause we want some items in the list dialog
    /*
    ScalePreferences* scolePreferences = new ScalePreferences();
    scolePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(scolePreferences);
    ScalePreferences* sculePreferences = new ScalePreferences();
    sculePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(sculePreferences);
    */
    scrollAreaWidgetContents->setLayout(layout);
}

void PreferencesDialog::addEffect(AbstractEffect *effect)
{
    effects << effect; // TODO: check if it is really needed to store the effect list (ale/20130807)

    QWidget* effectWidget = effect->getPreferencesWidget();
    if (effectWidget)
    {
        scrollAreaWidgetContents->layout()->addWidget(effectWidget);
        connect(this, SIGNAL(accepted()), effectWidget, SLOT(accept()));
    }
}
