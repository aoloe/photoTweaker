#include <QDebug>
#include "preferencesDialog.h"
#include "effect/scalePreferences.h"

PreferencesDialog::PreferencesDialog( QWidget * parent) : QDialog(parent)
{
    setupUi(this);

    scrollArea->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout();

    // TODO: get the preferences from each effects
    scalePreferences = new ScalePreferences();
    scalePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(scalePreferences);



    // only as sample beceause we want some items in the list dialog
    ScalePreferences* scolePreferences = new ScalePreferences();
    scolePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(scolePreferences);
    ScalePreferences* sculePreferences = new ScalePreferences();
    sculePreferences->setMinimumSize(scalePreferences->size());
    layout->addWidget(sculePreferences);
    scrollAreaWidgetContents->setLayout(layout);
}

void PreferencesDialog::writeSettings()
{
    qDebug() << "preferencesDialog writeSettings";
    scalePreferences->writeSettings();
}

void PreferencesDialog::readSettings()
{
    qDebug() << "preferencesDialog readSettings";
    scalePreferences->readSettings();
}
