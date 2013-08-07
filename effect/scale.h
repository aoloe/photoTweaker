#ifndef EFFECTSCALE_H
#define EFFECTSCALE_H

#include <QSignalMapper>
#include "abstracteffect.h"

class QToolBar;

class Photo;
class ScalePreferences;

class EffectScale : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectScale(QObject *parent = 0);
    void writeSettings();
    void readSettings();
    void setSizeList(QList<int> list) {size = list;}

    void addToToolBar(QToolBar &toolBar);
    QSignalMapper *signalMapper;
    QWidget* getPreferencesWidget();
    void apply(Photo &photo);
public slots:
    void doEffect(const QString &value);
    void acceptPreferencesWidget(bool enabled, QList<int> size);
private:
    QList<int> size;
};

#endif // EFFECTSCALE_H
