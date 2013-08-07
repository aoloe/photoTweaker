#include "ui_preferencesDialog.h"
#include "effect/abstracteffect.h"
class ScalePreferences;

class PreferencesDialog : public QDialog, public Ui::PreferencesDialog
{
    Q_OBJECT
public:
    PreferencesDialog( QWidget * parent = 0);
    void addEffect(AbstractEffect *effect);
private:
    ScalePreferences* scalePreferences;
};
