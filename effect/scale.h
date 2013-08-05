#ifndef EFFECTSCALE_H
#define EFFECTSCALE_H

#include <QSignalMapper>
#include "abstracteffect.h"

class Photo;
class QToolBar;

class EffectScale : public AbstractEffect
{
    Q_OBJECT
public:
    QSignalMapper *signalMapper;
    explicit EffectScale(QObject *parent = 0);
    void addToToolBar(QToolBar &toolBar);
    void apply(Photo &photo);
public slots:
    void doEffect(const QString &value);
    void doEffect(bool status);
signals:
    void clicked(int value);



};

#endif // EFFECTSCALE_H
