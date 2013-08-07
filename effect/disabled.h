#ifndef EFFECTDISABLED_H
#define EFFECTDISABLED_H 

#include "abstracteffect.h"

class EffectPreferences;

class EffectDisabled : public AbstractEffect
{
    Q_OBJECT
public:
    EffectDisabled(QObject *parent = 0);
    QWidget* getPreferencesWidget();
public slots:
    void acceptPreferencesWidget(bool enabled);
};

#endif // EFFECTDISABLED_H
