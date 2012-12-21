// --- VIEWER IMPL ---

#include "photoTweak.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

// --- VIEWER ---
PhotoTweak::PhotoTweak()
{
    setupUi(this);

    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openMedia()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void PhotoTweak::show()
{
    QMainWindow::show();
    if (!filename.isEmpty())
        showMedia();
}

void PhotoTweak::openMedia()
{
    filename = QFileDialog::getOpenFileName(this, tr("Select Media"), QDir::homePath());

    if (filename.isEmpty())
        return;
    qDebug() << "filename:" << filename;

    showMedia();
}

void PhotoTweak::showMedia()
{
    try 
    {
        magickImage.read( qPrintable(filename) );
    }
    catch(Exception &error_)
    {
        QMessageBox::warning(this, "Error!", QString("Unable to process file %1").arg(filename) );
        return;
    }
        
    // STEP 2 - Simple Processing with Magick
    magickImage.scale(Geometry(800, 600));
    magickImage.type( GrayscaleType );
    magickImage.magick("XPM");
    magickImage.write(&blob);
    imgData = ((char*)(blob.data()));
    
    // STEP 3 - Display
    pixmap.loadFromData(imgData, "XPM");
    label->setPixmap( pixmap );
}
