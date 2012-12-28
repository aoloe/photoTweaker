#include "photoTweak.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "photo.h"

PhotoTweak::PhotoTweak()
{
    setupUi(this);

    photo = new Photo();
    setCentralWidget(photo);

    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionNothing, SIGNAL(triggered()), photo, SLOT(clearSelection()));

    connect(photo, SIGNAL(show()), this, SLOT(show())); // XXX: no idea if this is needed...
}

void PhotoTweak::run()
{
    QMainWindow::show();
    qDebug() << "filePath:" << filePath;
    if (!filePath.isEmpty())
    {
        photo->open(filePath);
        photo->update();
    }
}

void PhotoTweak::open()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath());
    qDebug() << "filePath:" << filePath;

    if (filePath.isEmpty())
        return;

    if (!photo->open(filePath))
    {
        // TODO: disable the menus that need an image to be open (save)
    }
    photo->update();

}

void PhotoTweak::save()
{
    if(!filePath.isEmpty())
    {
        photo->save();
    }
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
    

}
