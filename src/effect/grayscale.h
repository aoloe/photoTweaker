#ifndef EFFECTGRAYSCALE_H
#define EFFECTGRAYSCALE_H

#include "abstracteffect.h"

class Photo;
class QToolBar;

class EffectGrayscale : public AbstractEffect
{
    Q_OBJECT
public:
    explicit EffectGrayscale(QObject *parent = 0);
    void addToToolBar(QToolBar &toolbar);
public slots:
    void apply();
};

#endif // EFFECTRAYSCALE_H
