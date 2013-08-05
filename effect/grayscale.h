#ifndef EFFECTGRAYSCALE_H
#define EFFECTGRAYSCALE_H

#include "abstracteffect.h"

class Photo;
class QToolBaQToolBarr;

class EffectGrayscale : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectGrayscale(QObject *parent = 0);
    void addToToolBar(QToolBar &toolbar);
    void apply(Photo &photo);
};

#endif // EFFECTRAYSCALE_H
