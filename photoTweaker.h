#ifndef PHOTOTWEAKER_H
#define PHOTOTWEAKER_H

#include "ui_photoTweaker.h"
#include "photo.h"

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

#endif // PHOTOTWEAKER_H
