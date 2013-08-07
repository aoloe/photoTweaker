#include "photoTweaker.h"

#include <QSettings>

#include <QCloseEvent>

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
#include "effect/disabled.h"
#include "effect/grayscale.h"
#include "effect/rotation.h"
#include "effect/scale.h"
#include "preferencesDialog.h"

QDebug operator<< (QDebug d, const PhotoTweaker::effectStruct &model);


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

    readSettings();

    undoGroup = new QUndoGroup(this);

    initializeEffects();

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

void PhotoTweaker::writeSettings()
{
// TODO: check, store and define in the window if it's floating: WM_WINDOW_TYPE property set to WINDOW_TYPE_NORMAL
// https://github.com/LaurentGomila/SFML/issues/368#issuecomment-15143196
// http://qt-project.org/doc/qt-4.8/widgets-windowflags.html or try setWindowFlags
    QSettings settings("graphicslab.org", "photoTweaker");

    settings.setValue("window/size", size());
    settings.setValue("window/pos", pos());
    settings.beginWriteArray("application/effects");
    for (int i = 0; i < effects.count(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("id", effects[i].id);
        settings.setValue("name", effects[i].name);
        settings.setValue("enabled", effects[i].enabled);
    }
    settings.endArray();
}

void PhotoTweaker::readSettings()
{
    QSettings settings("graphicslab.org", "photoTweaker");
    resize(settings.value("window/size", QSize(400, 400)).toSize()); // TODO: set a sane default
    move(settings.value("window/pos", QPoint(200, 200)).toPoint()); // TODO: set a sane default
    effects.clear();
    int n = settings.beginReadArray("application/effects");
    for (int i = 0; i < n; i++)
    {
        settings.setArrayIndex(i);
        effectStruct item;
        item.id = settings.value("id").toInt();
        item.name = settings.value("name").toString();
        item.enabled = settings.value("enabled").toBool();
        effects << item;
    }
    settings.endArray();
    if (effects.count() != EFFECT_COUNT)
    {
        // until we have real plugins: initialise the list of effects
        effects.clear();
        effectStruct item;
        item.id = EFFECT_ROTATION;
        item.name = "Rotation";
        item.enabled = true;
        effects << item;
        item.id = EFFECT_GRAYSCALE;
        item.name = "Grayscale";
        item.enabled = true;
        effects << item;
        item.id = EFFECT_SCALE;
        item.name = "Scale";
        item.enabled = true;
        effects << item;
    }
    qDebug() << "effects" << effects;
}

void PhotoTweaker::initializeEffects()
{
    AbstractEffect* effect;
    for (int i = 0; i < EFFECT_COUNT; i++)
    {
        if (effects[i].enabled)
        {
            if (effects[i].id == EFFECT_ROTATION)
            {
                effect = new EffectRotation();
            }
            else if (effects[i].id == EFFECT_GRAYSCALE)
            {
                effect = new EffectGrayscale();
            }
            else if (effects[i].id == EFFECT_SCALE)
            {
                effect = new EffectScale();
            }
        }
        else
        {
            effect = new EffectDisabled();
        }
        effect->setEffectName(effects[i].name);
        effect->setEnabled(effect[i].enabled);
        effect->readSettings();
        effects[i].effect = effect;
    }
}

/**
 * @brief Create the menu bar.
 *
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
void PhotoTweaker::initializeMenu()
{

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
    // connection to this->close is needed to do some cleanup (like writing the preferences).
    // this is done in closeEvent().
    connect(actionFileQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionFileQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
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

    // TODO: ESC should clear the selection (ale/20130807)
    // connect(actionNothing, SIGNAL(triggered()), photo, SLOT(clearSelection()));

}

/**
 * @brief PhotoTweaker::initializeToolBar adds the effect buttons to the toolbar
 * and creates a list of effet actions.
 */
void PhotoTweaker::initializeToolBar()
{
    // TODO: make it a setting where the toolbar is set (default left?) (ale/20130807)
    QToolBar* toolBar = new QToolBar();
    addToolBar(Qt::TopToolBarArea, toolBar );

    // TODO: add an option to show the label below the button? (ale/20130807)
    // toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    effectActions.fill(0, (int)EFFECT_COUNT);

    // TODO: let the effects add themselves to the toolbar
    for (int i = 0; i < EFFECT_COUNT; i++)
    {
        if (effects[i].id == EFFECT_SCALE)
        {
            effects[i].effect->addToToolBar(*toolBar);
        }
    }

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

    QAction *actionScale= new QAction(tr("900"), this);
    actionScale->setIcon(QIcon(":/media/icons/scale.png"));
    actionScale->setToolTip("Scale 900");
    connect(actionScale, SIGNAL(triggered(bool)), this, SLOT(doEffect(bool)));
    effectActions[EFFECT_SCALE] = actionScale;

    QToolButton *scaleButton = new QToolButton();
    scaleButton->setIcon(QIcon(":/media/icons/scale.png"));
    scaleButton->setText("900");
    scaleButton->setFocusPolicy(Qt::NoFocus);
    scaleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    scaleButton->setDefaultAction(actionScale);
    toolBar->addWidget(scaleButton);
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
        foreach(effectStruct item, effects)
        {
            dialog->addEffect(item.effect); // TODO: pass the full item?
            // TODO: the slot should probably not be named accepted(). (ale/20130807)
            // connect(dialog, SIGNAL(accepted()), item.effect, SLOT(accepted()));
        }
        if(dialog->exec() == QDialog::Accepted){
            // TODO: write the list of enabled settings
            // this->writeEffectSettings()
            foreach(effectStruct item, effects)
            {
                item.effect->writeSettings();
                item.enabled = item.effect->getEnabled();
            }
            writeSettings();
        }
}

void PhotoTweaker::closeEvent(QCloseEvent *event)
{
    qDebug() << "closing";
    // if (userReallyWantsToQuit()) {
    writeSettings();
    event->accept();
    // } else {
    // event->ignore();
    // }
    QMainWindow::closeEvent(event);
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

QDebug operator<< (QDebug d, const PhotoTweaker::effectStruct &model) {
    d << "<id:" << model.id << ", name:" << model.name << ", enabled:" << model.enabled << ">";
    return d;
}
