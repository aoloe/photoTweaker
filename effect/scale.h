#ifndef EFFECTSCALE_H
#define EFFECTSCALE_H

#include <QSignalMapper>
#include "abstracteffect.h"

class QToolBar;
class QButtonGroup;
class QAbstractButton;

class EffectScale : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectScale(QObject *parent = 0);
    void writeSettings();
    void readSettings();
    void setSizeList(QList<int> list) {size = list;}

    void addToToolBar(QToolBar &toolBar);
    // QSignalMapper *signalMapper;
    QWidget* getPreferencesWidget();
    void apply(QImage& image, int size);
public slots:
    // void apply(const QString &value);
    void acceptPreferencesWidget(bool enabled, QList<int> size);
    void buttonClicked(QAbstractButton* button);
    void buttonPressed(QAbstractButton* button);
    void onSave(QImage& image);
private:
    QList<int> size;
    QButtonGroup* buttonGroup;
    bool buttonUnchecking;
};

#endif // EFFECTSCALE_H
