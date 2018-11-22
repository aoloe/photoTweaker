#ifndef ABSTRACTEFFECT_H
#define ABSTRACTEFFECT_H

#include <QObject>
#include "photoTweaker.h"

QT_BEGIN_NAMESPACE
class QToolBar;
class Photo;
QT_END_NAMESPACE

/**
 * @brief Abstract class for implementing effects.
 *
 */
class AbstractEffect : public QObject
{
    Q_OBJECT

public:
    explicit AbstractEffect(QObject *parent = 0);
    virtual ~AbstractEffect(){}

    void setMainApp(PhotoTweaker* app) {mainApp = app;}
    virtual void addToToolBar(QToolBar &toolbar);
    virtual void writeSettings();
    virtual void readSettings();
    void setEnabled(bool enabled) {this->enabled = enabled;}
    bool getEnabled() {return enabled;}
    void setEffectName(QString name) {effectName = name;}
    virtual QWidget* getPreferencesWidget();
    virtual void apply(Photo &photo);
    
public:
    bool enabled; // TODO: why does it have to be public? (ale/20130807)
public slots:
    void onSave(QImage& image);
protected:
    QString effectName;
    PhotoTweaker* mainApp;
};

#endif // ABSTRACTEFFECT_H
