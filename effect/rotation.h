#ifndef EFFECTROTATION_H
#define EFFECTROTATION_H

#include "abstracteffect.h"

class Photo;

class EffectRotation : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectRotation(QObject *parent = 0);
    void apply(Photo &photo);
};

#endif // EFFECTROTATION_H
