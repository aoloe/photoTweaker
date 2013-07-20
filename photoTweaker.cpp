#include "photoTweaker.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QDebug>

#include "photo.h"

PhotoTweaker::PhotoTweaker()
{
    setupUi(this);

    initializeStatusBar();
    initializeToolBar();

    photo = new Photo();
    setCentralWidget(photo);

    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionNothing, SIGNAL(triggered()), photo, SLOT(clearSelection()));

    connect(photo, SIGNAL(show()), this, SLOT(show())); // XXX: no idea if this is needed...

    connect(photo, SIGNAL(setStatusSize(int, int)), this, SLOT(setStatusSize(int, int)));
    connect(photo, SIGNAL(setStatusMouse(int, int)), this, SLOT(setStatusMouse(int, int)));
    connect(photo, SIGNAL(setStatusMouse()), this, SLOT(setStatusMouse()));
    connect(photo, SIGNAL(setStatusMessage(QString)), this, SLOT(setStatusMessage(QString)));
    connect(photo, SIGNAL(setWindowTitle(QString)), this, SLOT(setTitle(QString)));
}
void PhotoTweaker::initializeToolBar()
{
    // TODO: make it a setting where the toolbar is set (default left?)
    QToolBar* toolBar = new QToolBar();
    addToolBar(Qt::TopToolBarArea, toolBar );

    QAction *actionRotate = new QAction(tr("Rotate"), this);
    // actionRotate->setIcon(QIcon(":/media/icons/rotate.png"));
    actionRotate->setIcon(QIcon(":/media/icons/curve.png"));
    connect(actionRotate, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    toolBar->addAction(actionRotate);
/*

    QAction *mCursorAction = new QAction(tr("Selection"), this);
    mCursorAction->setCheckable(true);
    mCursorAction->setIcon(QIcon(":/media/instruments-icons/cursor.png"));
    connect(mCursorAction, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsMenu->addAction(mCursorAction);
    mInstrumentsActMap.insert(CURSOR, mCursorAction);
*/


}
void PhotoTweaker::initializeStatusBar()
{
    statusBar = new QStatusBar();
    setStatusBar(statusBar);

    statusBarSize = new QLabel();
    statusBarMouse = new QLabel();
    statusBarMessage = new QLabel();

    statusBar->addPermanentWidget(statusBarSize);
    statusBar->addPermanentWidget(statusBarMouse);
    statusBar->addPermanentWidget(statusBarMessage, 100);

    // sizeLabel->setText(QString("%1 x %2").arg(size.width()).arg(size.height()));
    statusBarMouse->setText(QString("%1,%2").arg(10).arg(0));

}


void PhotoTweaker::run()
{
    QMainWindow::show();
    qDebug() << "filePath:" << filePath;
    if (!filePath.isEmpty())
    {
        photo->open(filePath);
        photo->update();
    }
}

void PhotoTweaker::setStatusSize(int width, int height)
{
    statusBarSize->setText(QString("%1 x %2").arg(width).arg(height));
}

void PhotoTweaker::setStatusMouse(int x, int y)
{
    statusBarMouse->setText(QString("%1,%2").arg(x).arg(y));
}

void PhotoTweaker::setStatusMouse()
{
    statusBarMouse->setText("");
}

void PhotoTweaker::setStatusMessage(QString message)
{
    statusBarMessage->setText(message);
}

void PhotoTweaker::setTitle(QString title)
{
   setWindowTitle(QString("%1 - %2").arg(title).arg(QFileInfo( QCoreApplication::applicationFilePath() ).fileName()));
}

void PhotoTweaker::open()
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

void PhotoTweaker::save()
{
    if(!filePath.isEmpty())
    {
        photo->save();
    }
}

void PhotoTweaker::show()
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
