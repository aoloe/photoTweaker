#ifndef PHOTOTWEAK_H
#define PHOTOTWEAK_H

#include "ui_photoTweak.h"
#include "photo.h"

class Photo;
// #include <Magick++.h>

// using namespace Magick;

class PhotoTweak : public QMainWindow, public Ui::PhotoTweak
{
	Q_OBJECT
public:
	PhotoTweak();
    void setFilePath(QString filePath) {this->filePath = filePath;}
	void run();
	
public slots:
	void open();
	void save();
	void show();

private:
	QString filePath;
    Photo *photo;

	// Magick++ Objects
	QByteArray 	imgData;
	QPixmap	pixmap;
	// Image 		magickImage;
	// Blob 			blob;
};

#endif // PHOTOTWEAK_H
