#include "ui_preferencesDialog.h"

class PreferencesDialog : public QDialog, public Ui::PreferencesDialog
{
        Q_OBJECT

        public:
            PreferencesDialog( QWidget * parent = 0);
};
