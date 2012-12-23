#include <QDebug>
#include "photo.h"
#include "undocommand.h"

#include "instrument/abstractinstrument.h"
#include "instrument/selection.h"

// #include "effect/gray.h"

Photo::Photo()
// ImageArea::ImageArea(const bool &isOpen, const QString &filePath, QWidget *parent) :
//       QWidget(parent), mIsEdited(false), mIsPaint(false), mIsResize(false)

{
    // setMouseTracking(true);

    filePath.clear();
    image = new QImage();


    // mZoomFactor = 1;

    // undoStack = new QUndoStack(this);
    // undoStack->setUndoLimit(DataSingleton::Instance()->getHistoryDepth());




    // initializeImage();
    /*
    if(isOpen && filePath.isEmpty())
    {
        open();
    }
    else if(isOpen && !filePath.isEmpty())
    {
        open(filePath);
    }
    */

    // connect(mAdditionalTools, SIGNAL(sendNewImageSize(QSize)), this, SIGNAL(sendNewImageSize(QSize)));


    /*
    InstrumentSelection *instrumentSelection = new InstrumentSelection(this);
    connect(instrumentSelection, SIGNAL(sendEnableCropAction(bool)), this, SIGNAL(sendEnableCropAction(bool)));
    connect(selectionInstrument, SIGNAL(sendEnableSelectionInstrument(bool)), this, SIGNAL(sendEnableSelectionInstrument(bool)));
    */

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

void Photo::open()
{
    if (!filePath.isEmpty())
        open(filePath);
}

void Photo::open(const QString filePath)
{
    this->filePath = filePath;
    image->load(filePath);
    qDebug() << "opened file" << filePath;
    *image = image->convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

void Photo::update()
{
    emitShow();
}

/*
void Photo::update()
{

}
*/

const uchar* Photo::getData()
{
    return image->constBits();
}

void Photo::saveImageChanges()
{
}

void Photo::clearSelection()
{
}

void Photo::mousePressEvent(QMouseEvent *event)
{
}

void Photo::mouseMoveEvent(QMouseEvent *event)
{
}

void Photo::mouseReleaseEvent(QMouseEvent *event)
{
}
