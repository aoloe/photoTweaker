#ifndef EFFECTSCALE_H
#define EFFECTSCALE_H

#include "abstracteffect.h"

class Photo;

class EffectScale : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectScale(QObject *parent = 0);
    void apply(Photo &photo);
};

#endif // EFFECTSCALE_H
