#ifndef PHOTOTWEAK_H
#define PHOTOTWEAK_H

#include "ui_photoTweak.h"
#include <Magick++.h>

using namespace Magick;

class PhotoTweak : public QMainWindow, public Ui::PhotoTweak
{
	Q_OBJECT
public:
	PhotoTweak();
    void setFilename(QString filename) {this->filename = filename;}
	void show();
	
public slots:
	void openMedia();
	void showMedia();

private:
	QString		filename;

	// Magick++ Objects
	QByteArray 	imgData;
	QPixmap		pixmap;
	Image 		magickImage;
	Blob 			blob;
};

#endif // PHOTOTWEAK_H
