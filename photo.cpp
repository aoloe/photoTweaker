#include "photo.h"
#include "undocommand.h

#include "instrument/abstract.h"
#include "instrument/selection.h"

#include "effect/gray.h"

Photo::Photo()
{
    setMouseTracking(true);

    filepath.clear();

    mZoomFactor = 1;

    undoStack = new QUndoStack(this);
    // undoStack->setUndoLimit(DataSingleton::Instance()->getHistoryDepth());




    initializeImage();
    if(isOpen && filepath.isEmpty())
    {
        open();
    }
    else if(isOpen && !filepath.isEmpty())
    {
        open(filepath);
    }

    // connect(mAdditionalTools, SIGNAL(sendNewImageSize(QSize)), this, SIGNAL(sendNewImageSize(QSize)));


    InstrumentSelection *instrumentSelection = new InstrumentSelection(this);
    connect(instrumentSelection, SIGNAL(sendEnableCropAction(bool)), this, SIGNAL(sendEnableCropAction(bool)));
    connect(selectionInstrument, SIGNAL(sendEnableSelectionInstrument(bool)), this, SIGNAL(sendEnableSelectionInstrument(bool)));

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

Photo::i~Photo()
{

