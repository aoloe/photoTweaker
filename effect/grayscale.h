#ifndef EFFECTGRAYSCALE_H
#define EFFECTGRAYSCALE_H

#include "abstracteffect.h"

class Photo;

class EffectGrayscale : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectGrayscale(QObject *parent = 0);
    void apply(Photo &photo);
};

#endif // EFFECTRAYSCALE_H
