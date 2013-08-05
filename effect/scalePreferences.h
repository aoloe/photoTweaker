#include "ui_scalePreferences.h"

class ScalePreferences : public QWidget, public Ui::ScalePreferences
{
        Q_OBJECT

public:
    ScalePreferences( QWidget * parent = 0);

    void writeSettings();
    void readSettings();

public slots:
    void addItem();
    void removeItem();
    void activateItem(QListWidgetItem * itemClicked, QListWidgetItem *itemPrevious);
};

