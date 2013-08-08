#ifndef PHOTOTWEAKER_H
#define PHOTOTWEAKER_H

#include <QDebug>
#include "ui_photoTweaker.h"
#include "photo.h"

class QLabel;
class QStatusBar;
class QUndoGroup;
class QCloseEvent;

class Photo;
// #include <Magick++.h>

// using namespace Magick;

class PhotoTweaker : public QMainWindow, public Ui::PhotoTweaker
{
	Q_OBJECT
public:
	PhotoTweaker();
    void setFilePath(QString filePath) {this->filePath = filePath;}
	void run();

    Photo* getPhoto() {return this->photo;}

    struct effectStruct {
        int id;
        QString name;
        bool enabled;
        AbstractEffect* effect;
    };

    static const int EFFECT_COUNT;
    static const int EFFECT_ROTATION;
    static const int EFFECT_GRAYSCALE;
    static const int EFFECT_SCALE;

public slots:
	void open();
	void save();
	void preferences();
	void show();
	void setStatusSize(int width, int height);
	void setStatusMouse(int x, int y);
	void setStatusMouse();
	void setStatusMessage(QString message);
	void setTitle(QString title);

protected:
    void closeEvent(QCloseEvent *event);

private:

    void initializeEffects();
    QList<effectStruct> effects; // temporary list of effects (enabled/disabled) until we have real plugins
    void initializeStatusBar();
    QStatusBar* statusBar;
    QLabel* statusBarSize;
    QLabel* statusBarMouse;
    QLabel* statusBarMessage;
    
    void writeSettings();
    void readSettings();
    void initializeToolBar();
    QToolBar* toolBar;
    void initializeMenu();

    QAction *actionFileOpen, *actionFileSave, *actionFileQuit, 
    *actionEditUndo, *actionEditRedo, *actionEditPreferences;

    QUndoGroup *undoGroup;

	QString filePath;
    Photo* photo;

	// Magick++ Objects
	QByteArray 	imgData;
	QPixmap	pixmap;
	// Image 		magickImage;
	// Blob 			blob;
};

#endif // PHOTOTWEAKER_H
