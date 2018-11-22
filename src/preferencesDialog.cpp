#include <QDebug>
#include "preferencesDialog.h"
#include "effect/scalePreferences.h"

PreferencesDialog::PreferencesDialog( QWidget * parent) : QDialog(parent)
{
    setupUi(this);

    scrollArea->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout();
    scrollAreaWidgetContents->setLayout(layout);
}

void PreferencesDialog::addEffect(AbstractEffect *effect)
{
    QWidget* effectWidget = effect->getPreferencesWidget();
    if (effectWidget)
    {
        scrollAreaWidgetContents->layout()->addWidget(effectWidget);
        connect(this, SIGNAL(accepted()), effectWidget, SLOT(accept()));
    }
}

void PreferencesDialog::setListAlignTop()
{
    qobject_cast<QVBoxLayout *>(scrollAreaWidgetContents->layout())->addStretch(0);
}
