#ifndef EFFECTROTATE_H
#define EFFECTROTATE_H

#include "abstracteffect.h"

class QToolBar;

class EffectRotate : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectRotate(QObject *parent = 0);
    void writeSettings();
    void readSettings();
    void setClockwise(bool clockwise) {this->clockwise = clockwise;}
    void addToToolBar(QToolBar &toolbar);
    QWidget* getPreferencesWidget();
public slots:
    void apply();
    void acceptPreferencesWidget(bool enabled, bool clockwise);
private:
    bool clockwise;
};

#endif // EFFECTROTATE_H
