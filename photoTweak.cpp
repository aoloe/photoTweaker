#include "photoTweak.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "photo.h"

PhotoTweak::PhotoTweak()
{
    setupUi(this);

    label->setGeometry(this->geometry());

    photo = new Photo();

    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(photo, SIGNAL(show()), this, SLOT(show()));
}

void PhotoTweak::run()
{
    QMainWindow::show();
    qDebug() << "filePath:" << filePath;
    if (!filePath.isEmpty())
    {
        // photo->setFilePath(filePath);
        photo->open(filePath);
        photo->update();
    }
}

void PhotoTweak::open()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath());

    if (filePath.isEmpty())
        return;

    qDebug() << "filePath:" << filePath;

    show();
}

void PhotoTweak::show()
{
    /*
    try 
    {
        magickImage.read( qPrintable(filePath) );
    }
    catch(Exception &error_)
    {
        QMessageBox::warning(this, "Error!", QString("Unable to process file %1").arg(filePath) );
        return;
    }
        
    // STEP 2 - Simple Processing with Magick
    magickImage.scale(Geometry(800, 600));
    magickImage.type( GrayscaleType );
    magickImage.magick("XPM");
    magickImage.write(&blob);
    imgData = ((char*)(blob.data()));
*/
    // QImage mImage
    // mImage->load(filePath);
    // *mImage = mImage->convertToFormat(QImage::Format_ARGB32_Premultiplied);
    // mImage = new QImage(DataSingleton::Instance()->getBaseSize(), QImage::Format_ARGB32_Premultiplied);
    // resize(mImage->rect().right() + 6, mImage->rect().bottom() + 6);

    
    // STEP 3 - Display
    //pixmap.loadFromData(imgData, "XPM");
    // pixmap = new QPixmap(filePath);
    // label->setPixmap( *pixmap );
    /*
    const uchar* imageData = photo->getData();
    pixmap->loadFromData(imageData);
    label->setPixmap(*pixmap);
    */
    /*
    const QImage* image = photo->getImage();
    // TODO: only do it if the image exists! otherwise it will crash...
    if (image != NULL)
    {
        pixmap->fromImage(*image);
        if (pixmap != NULL)
            // label->setPixmap(pixmap->scaled(label->size(), Qt::KeepAspectRatio));
            label->setPixmap(*pixmap);
        else
            qDebug() << "pixmap is null";
        // label->setPixmap(*pixmap);
    }
    */
    /*
    QImage image;
    qDebug() << "file path" << filePath;
    qDebug() << "exists" << QFile::exists(filePath);
    bool result = image.load(filePath);
    qDebug() << "result" << result;
    QPixmap pixmap;
    // pixmap.fromImage(image);
    pixmap = QPixmap::fromImage(image);
    */
    pixmap = QPixmap::fromImage(photo->getImage());
    
    if (!pixmap.isNull())
        label->setPixmap(pixmap);
    else
        qDebug() << "pixmap is not valid";
    label->show();

}
