#include "photoTweaker.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QUndoGroup>
#include <QLabel>
#include <QDebug>
#include <QAction> //insert by Katrin
#include <QToolButton>

#include "photo.h"
#include "effect/grayscale.h"
#include "effect/rotation.h"
#include "effect/scale.h"
#include "preferencesDialog.h"


const int PhotoTweaker::EFFECT_COUNT = 3;
const int PhotoTweaker::EFFECT_NONE = -1; // TODO: is it needed? (ale/20130724)
const int PhotoTweaker::EFFECT_ROTATION = 0;
const int PhotoTweaker::EFFECT_GRAYSCALE = 1;
const int PhotoTweaker::EFFECT_SCALE = 2;

/**
 * @brief PhotoTweaker::PhotoTweaker manages the application's main window
 */

PhotoTweaker::PhotoTweaker()
{
    setupUi(this);

    undoGroup = new QUndoGroup(this);


    initializeStatusBar();
    initializeToolBar();

    photo = new Photo();
    setCentralWidget(photo);



    initializeMenu();

    connect(photo, SIGNAL(show()), this, SLOT(show())); // XXX: no idea if this is needed...

    connect(photo, SIGNAL(setStatusSize(int, int)), this, SLOT(setStatusSize(int, int)));
    connect(photo, SIGNAL(setStatusMouse(int, int)), this, SLOT(setStatusMouse(int, int)));
    connect(photo, SIGNAL(setStatusMouse()), this, SLOT(setStatusMouse()));
    connect(photo, SIGNAL(setStatusMessage(QString)), this, SLOT(setStatusMessage(QString)));
    connect(photo, SIGNAL(setWindowTitle(QString)), this, SLOT(setTitle(QString)));
}

void PhotoTweaker::initializeMenu()
{
    /*
     * file   edit          select         effects
     *  open   undo          none
     *  --     redo          --
     *  save   --            v move    m
     *  --     preferences     grow    >
     *  quit                   shrink  <
     *                       --
     *                       ... left  h
     *                       ... up    j
     *                       ... down  k
     *                       ... right l
     * 
     */

    QMenu *menuFile = menuBar()->addMenu(tr("&File"));

    actionFileOpen = new QAction(tr("&Open"), this);
    connect(actionFileOpen, SIGNAL(triggered()), this, SLOT(open()));
    actionFileOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addAction(actionFileOpen);

    menuFile->addSeparator();

    actionFileSave = new QAction(tr("&Save"), this);
    connect(actionFileSave, SIGNAL(triggered()), this, SLOT(save()));
    actionFileSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    menuFile->addAction(actionFileSave);

    menuFile->addSeparator();

    actionFileQuit = new QAction(tr("&Quit"), this);
    connect(actionFileQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    // actionFileQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    // actionFileQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    actionFileQuit->setShortcuts(
        QList<QKeySequence>()
            << Qt::CTRL + Qt::Key_Q
            << Qt::CTRL + Qt::Key_W
    );
    menuFile->addAction(actionFileQuit);

    QMenu *menuEdit = menuBar()->addMenu(tr("&Edit"));

    actionEditUndo = undoGroup->createUndoAction(this, tr("&Undo"));
    actionEditUndo->setEnabled(false);
    actionEditUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    menuEdit->addAction(actionEditUndo);

    actionEditRedo = undoGroup->createRedoAction(this, tr("&Redo"));
    actionEditRedo->setEnabled(false);
    actionEditRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    menuEdit->addAction(actionEditRedo);

    menuEdit->addSeparator();

    actionEditPreferences = new QAction(tr("&Preferences"), this);
    connect(actionEditPreferences, SIGNAL(triggered()), this, SLOT(preferences()));
    menuEdit->addAction(actionEditPreferences);
    // actionPreferences->setMenuRole(QAction::PreferencesRole);

    // connect(actionNothing, SIGNAL(triggered()), photo, SLOT(clearSelection()));




}

/**
 * @brief PhotoTweaker::initializeToolBar adds the effect buttons to the toolbar
 * and creates a list of effet actions.
 */
void PhotoTweaker::initializeToolBar()
{
    // TODO: make it a setting where the toolbar is set (default left?)
    QToolBar* toolBar = new QToolBar();
    addToolBar(Qt::TopToolBarArea, toolBar );

    // toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    effectActions.fill(0, (int)EFFECT_COUNT);

    // TODO: let the effects add themselves to the toolbar

    QAction *actionRotate = new QAction(tr("Rotate"), this);
    actionRotate->setIcon(QIcon(":/media/icons/rotate.png"));
    connect(actionRotate, SIGNAL(triggered(bool)), this, SLOT(doEffect(bool)));
    toolBar->addAction(actionRotate);
    effectActions[EFFECT_ROTATION] = actionRotate;

    QAction *actionGrayscale= new QAction(tr("Gray"), this);
    actionGrayscale->setIcon(QIcon(":/media/icons/grayscale.png"));
    connect(actionGrayscale, SIGNAL(triggered(bool)), this, SLOT(doEffect(bool)));
    toolBar->addAction(actionGrayscale);
    effectActions[EFFECT_GRAYSCALE] = actionGrayscale;

    // TODO: find a way to cleanly add a button with a label
    // - the tooltip should be different than the text shown ("Scale 900" vs. "900")
    // - it would be nice to have only one action with different parameters for each target size
    QAction *actionScale= new QAction(tr("900"), this);
    actionScale->setIcon(QIcon(":/media/icons/scale.png"));
    connect(actionScale, SIGNAL(triggered(bool)), this, SLOT(doEffect(bool)));
    effectActions[EFFECT_SCALE] = actionScale;

    // QPushButton *scaleButton = new QPushButton();
    QToolButton *scaleButton = new QToolButton();
    scaleButton->setIcon(QIcon(":/media/icons/scale.png"));
    scaleButton->setText("900");
    scaleButton->setFocusPolicy(Qt::NoFocus);
    scaleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    scaleButton->setDefaultAction(actionScale);
    toolBar->addWidget(scaleButton);

    /*
    // failed try to use a widget in order to put a label right of the button
    // QPushButton *scaleButton = new QPushButton();
    QHBoxLayout *layout = new QHBoxLayout();

    QToolButton *sculeButton = new QToolButton();
    sculeButton->setIcon(QIcon(":/media/icons/scale.png"));
    // sculeButton->setText("600");
    scaleButton->setAutoRaise(true);
    sculeButton->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(sculeButton);

    QWidget *scoleWidget = new QWidget();
    // scoleButton->setIcon(QIcon(":/media/icons/scale.png"));
    scoleWidget->setLayout(layout);
    // scoleButton->setText("600");
    // scoleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    scoleWidget->setDefaultAction(actionScale);
    toolBar->addWidget(scoleWidget);
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
        open();
    }
}

/**
 * @brief PhotoTweaker::effectsAct
 * @param bool state wether the button is down or up
 */
void PhotoTweaker::doEffect(bool state)
{
     QAction *currentAction = static_cast<QAction*>(sender());
     if (currentAction == effectActions[EFFECT_ROTATION])
     {
         EffectRotation* effect = new EffectRotation(this);
         effect->apply(*photo);
     }
     else if (currentAction == effectActions[EFFECT_GRAYSCALE])
     {
         EffectGrayscale* effect = new EffectGrayscale(this);
         effect->apply(*photo);
     }
     else if (currentAction == effectActions[EFFECT_SCALE])
     {
         EffectScale* effect = new EffectScale(this);
         effect->apply(*photo);
     }

/*
        TODO: is this an "abstract" way to call the effects?
            if(currentAction == mInstrumentsActMap[COLORPICKER] && !mPrevInstrumentSetted)
            {
                DataSingleton::Instance()->setPreviousInstrument(DataSingleton::Instance()->getInstrument());
                mPrevInstrumentSetted = true;
            }
            setAllInstrumentsUnchecked(currentAction);
            currentAction->setChecked(true);
            DataSingleton::Instance()->setInstrument(mInstrumentsActMap.key(currentAction));
            emit sendInstrumentChecked(mInstrumentsActMap.key(currentAction));
        }
        else
        {
            setAllInstrumentsUnchecked(NULL);
            DataSingleton::Instance()->setInstrument(NONE_INSTRUMENT);
            emit sendInstrumentChecked(NONE_INSTRUMENT);
            if(currentAction == mInstrumentsActMap[CURSOR])
                DataSingleton::Instance()->setPreviousInstrument(mInstrumentsActMap.key(currentAction));
*/
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
    QAction *currentAction = static_cast<QAction*>(sender());
    if (currentAction == actionFileOpen)
    {
        filePath = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath());
    }
    qDebug() << "filePath:" << filePath;

    if (filePath.isEmpty())
        return;

    if (!photo->open(filePath))
    {
        // TODO: disable the menus that need an image to be open (save)
    }
    photo->update();
    undoGroup->addStack(photo->getUndoStack());
    undoGroup->setActiveStack(photo->getUndoStack());
}

void PhotoTweaker::save()
{
    qDebug() << "trying to save";
    if(!filePath.isEmpty())
    {
        qDebug() << "save " << filePath;
        photo->save();
    }
}

void PhotoTweaker::preferences()
{
        PreferencesDialog* dialog = new PreferencesDialog(this);
        dialog->show();
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
