#include <QDebug>
#include <QtGui/QApplication>

#include "photo.h"
#include "undocommand.h"
#include "datasingleton.h"

#include "instrument/abstractinstrument.h"
#include "instrument/selection.h"

// #include "effect/gray.h"

const int Photo::INSTRUMENTS_COUNT = 1;
const int Photo::NONE_INSTRUMENT = 1;
const int Photo::CURSOR = 2;

Photo::Photo()
// ImageArea::ImageArea(const bool &isOpen, const QString &filePath, QWidget *parent) :
//       QWidget(parent), isEdited(false), isPaint(false), isResize(false)

{
    setMouseTracking(true);

    filePath.clear();
    image = QImage();

    zoomFactor = 1; // XXX: could be useful for calculating the relationship between screen and image coordinates

    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(DataSingleton::Instance()->getHistoryDepth());

    // TODO: connect here the signal for the window resize


    SelectionInstrument *selectionInstrument = new SelectionInstrument(this);
    // connect(instrumentSelection, SIGNAL(sendEnableCropAction(bool)), this, SIGNAL(sendEnableCropAction(bool)));
    // connect(selectionInstrument, SIGNAL(sendEnableSelectionInstrument(bool)), this, SIGNAL(sendEnableSelectionInstrument(bool)));

    instrumentsHandlers.fill(0, (int)INSTRUMENTS_COUNT);
    instrumentsHandlers[CURSOR] = selectionInstrument;

    currentCursor = QCursor(Qt::CrossCursor);
    setCursor(currentCursor);

    /*
    // Effects handlers
    effectsHandlers.fill(0, (int)EFFECTS_COUNT);
    effectsHandlers[NEGATIVE] = new NegativeEffect(this);
    effectsHandlers[GRAY] = new GrayEffect(this);
    effectsHandlers[BINARIZATION] = new BinarizationEffect(this);
    effectsHandlers[GAUSSIANBLUR] = new GaussianBlurEffect(this);
    effectsHandlers[GAMMA] = new GammaEffect(this);
    effectsHandlers[SHARPEN] = new SharpenEffect(this);
    effectsHandlers[CUSTOM] = new CustomEffect(this);
    */
 
}

Photo::~Photo()
{
    // connect(this, SIGNAL(update()), photoTweak, SLOT(show()));
}

bool Photo::open()
{
    bool result = false;
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (!filePath.isEmpty())
    {
        result =  open(filePath);
    }
    else
    {
        image = QImage();
    }

    QApplication::restoreOverrideCursor();
    return result;
}

bool Photo::open(const QString filePath)
{
    bool loaded = true;
    this->filePath = filePath;
    loaded = image.load(filePath);
    qDebug() << "loaded:" << loaded;
    if (loaded)
    {
        qDebug() << "opened file" << filePath;
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
    else
    {
        image = QImage();
        qDebug() << "failed to open file" << filePath;
    }
    return loaded;
}

void Photo::update()
{
    emitShow();
}

const uchar* Photo::getData()
{
    return image.constBits();
}

void Photo::saveImageChanges()
{
}

void Photo::clearSelection()
{
    /*
    foreach (AbstractInstrument* instrument, instrumentsHandlers)
    {
        if (AbstractSelection *selection = qobject_cast<AbstractSelection*>(instrument))
            selection->clearSelection(*this);
    }
    */
}

void Photo::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mouse pressed";
    instrumentHandler = instrumentsHandlers.at(DataSingleton::Instance()->getInstrument());
    instrumentHandler->mousePressEvent(event, *this);
}

void Photo::mouseMoveEvent(QMouseEvent *event)
{
}

void Photo::mouseReleaseEvent(QMouseEvent *event)
{
}

void Photo::undo(UndoCommand *command)
{
}

void Photo::resize()
{
}

void Photo::restoreCursor()
{
}
