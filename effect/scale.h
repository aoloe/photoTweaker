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
    QSignalMapper *signalMapper;
    explicit EffectScale(QObject *parent = 0);
    void writeSettings();
    void readSettings();
    void setSizeList(QList<int> list) {size = list;}

    void addToToolBar(QToolBar &toolBar);
    QWidget* getPreferencesWidget();
    void apply(Photo &photo);
public slots:
    void doEffect(const QString &value);
    void acceptPreferencesWidget();
signals:
    void clicked(int value);
private:
    QList<int> size;
    ScalePreferences* scalePreferences;
};

#endif // EFFECTSCALE_H
