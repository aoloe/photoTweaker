/*
 * This source file is part of photoTweaker.
 *
 * Copyright (c) 2012 Ale Rimoldi <https://github.com/aoloe/photoTweaker>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <QDebug>
#include <QtGui/QApplication>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

#include "photo.h"
#include "undocommand.h"
#include "datasingleton.h"

#include "instrument/abstractinstrument.h"
#include "instrument/selection.h"

// #include "effect/gray.h"

const int Photo::INSTRUMENTS_COUNT = 1;
const int Photo::NONE_INSTRUMENT = -1;
const int Photo::CURSOR = 0;

Photo::Photo()
// ImageArea::ImageArea(const bool &isOpen, const QString &filePath, QWidget *parent) :
//       QWidget(parent), isEdited(false), isPaint(false), isResize(false)

{
    setMouseTracking(true);

    filePath.clear();
    image = QImage();

    zoomFactor = 1; // XXX: could be useful for calculating the relationship between screen and image coordinates

    isEdited = false;

    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(DataSingleton::Instance()->getHistoryDepth());



    SelectionInstrument *selectionInstrument = new SelectionInstrument(this);
    // connect(instrumentSelection, SIGNAL(sendEnableCropAction(bool)), this, SIGNAL(sendEnableCropAction(bool)));
    // connect(selectionInstrument, SIGNAL(sendEnableSelectionInstrument(bool)), this, SIGNAL(sendEnableSelectionInstrument(bool)));

    instrumentsHandlers.fill(0, (int)INSTRUMENTS_COUNT);
    // qDebug() << "CURSOR" << CURSOR;
    // qDebug() << "selectionInstrument" << selectionInstrument;
    instrumentsHandlers[CURSOR] = selectionInstrument;
    // qDebug() << "instrumentsHandlers" << instrumentsHandlers;
    DataSingleton::Instance()->setInstrument(NONE_INSTRUMENT);


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
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        DataSingleton::Instance()->setInstrument(CURSOR);
        qDebug() << "opened file" << filePath;
    }
    else
    {
        image = QImage();
        DataSingleton::Instance()->setInstrument(NONE_INSTRUMENT);
        qDebug() << "failed to open file" << filePath;
    }
    // qDebug() << "image size" << image.byteCount();
    updateImageView();
    isEdited = false;
    return loaded;
}

void Photo::save()
{
    if(!filePath.isEmpty())
    {
        SelectionInstrument *instrument = static_cast <SelectionInstrument*> (instrumentsHandlers.at(CURSOR));
        if (isEdited || instrument->isSelection())
        {
            qDebug() << "saving selection:" << instrument->getSelection();
            qDebug() << "filePath:" << filePath;
            image.copy(instrument->getSelection()).save(filePath);
        }
    }

    isEdited = false;
}

void Photo::updateImageView()
{
    // qDebug() << "image size" << image.size();
    // qDebug() << "window size" << size();
    // qDebug() << "image size bounded" << image.size().boundedTo(size());

    if (size() == image.size().boundedTo(size()))
    {
        // TODO: scale to a bigger area than the current window size, in order to be able to show the scaling steps without rescaling each time the image
        QSize newSize = image.size().boundedTo(size());
        imageView = image.scaled(newSize, Qt::KeepAspectRatio);
        // qDebug() << "imageView size:" << imageView.size();
    }
    else
    {
        imageView = image.copy();
    }
    viewScale = (float) imageView.width() / (float) image.width();
    update();
}

void Photo::paintEvent(QPaintEvent *event)
{
     QPainter painter(this);
     QRect dirtyRect = event->rect();
     painter.drawImage(dirtyRect, imageView, dirtyRect);
}

void Photo::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Photo::resize()";
    updateImageView();
    QWidget::resizeEvent(event);
    if (DataSingleton::Instance()->getInstrument() != NONE_INSTRUMENT)
    {
        instrumentHandler = instrumentsHandlers.at(DataSingleton::Instance()->getInstrument());
        instrumentHandler->setViewScale(viewScale);
        instrumentHandler->resizeEvent(event, *this);
    }
}

 void Photo::clear()
 {
     image.fill(qRgb(255, 255, 255));
     // modified = true;
     update();
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
    SelectionInstrument *instrument = static_cast <SelectionInstrument*> (instrumentsHandlers.at(CURSOR));
    instrument->clearSelection();
}

void Photo::mousePressEvent(QMouseEvent *event)
{
    int instrument = DataSingleton::Instance()->getInstrument();
    if (instrument >= 0)
    {
        instrumentHandler = instrumentsHandlers.at(instrument);
        instrumentHandler->mousePressEvent(event, *this);
    }
}

void Photo::mouseMoveEvent(QMouseEvent *event)
{
    int instrument = DataSingleton::Instance()->getInstrument();
    if (instrument >= 0)
    {
        instrumentHandler = instrumentsHandlers.at(instrument);
        instrumentHandler->mouseMoveEvent(event, *this);
    }
}

void Photo::mouseReleaseEvent(QMouseEvent *event)
{
    int instrument = DataSingleton::Instance()->getInstrument();
    if (instrument >= 0)
    {
        instrumentHandler = instrumentsHandlers.at(instrument);
        instrumentHandler->mouseReleaseEvent(event, *this);
    }
}

void Photo::undo(UndoCommand *command)
{
}

void Photo::restoreCursor()
{
}
