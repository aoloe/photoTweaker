#include "ui_preferencesDialog.h"
#include "effect/abstracteffect.h"
class ScalePreferences;

class PreferencesDialog : public QDialog, public Ui::PreferencesDialog
{
        Q_OBJECT

        public:
            PreferencesDialog( QWidget * parent = 0);
            void addEffect(AbstractEffect *effect) {this->effects << effect;}
            void writeSettings();
            void readSettings();
        private:
        ScalePreferences* scalePreferences;
        QList<AbstractEffect*> effects;
};
