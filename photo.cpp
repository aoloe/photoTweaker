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
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QFileInfo>

#include <QUndoStack>

#include "photo.h"
#include "undocommand.h"

#include "instrument/abstractinstrument.h"
#include "instrument/selectionInstrument.h"

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
    undoStack->setUndoLimit(1); // by design, we only support one undo step

    SelectionInstrument *selectionInstrument = new SelectionInstrument(this);
    // connect(instrumentSelection, SIGNAL(sendEnableCropAction(bool)), this, SIGNAL(sendEnableCropAction(bool)));
    // connect(selectionInstrument, SIGNAL(sendEnableSelectionInstrument(bool)), this, SIGNAL(sendEnableSelectionInstrument(bool)));

    instrumentsHandlers.fill(0, (int)INSTRUMENTS_COUNT);
    // qDebug() << "CURSOR" << CURSOR;
    // qDebug() << "selectionInstrument" << selectionInstrument;
    instrumentsHandlers[CURSOR] = selectionInstrument;
    // qDebug() << "instrumentsHandlers" << instrumentsHandlers;
    currentInstrument = NONE_INSTRUMENT;

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
    // connect(this, SIGNAL(update()), photoTweaker, SLOT(show()));
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
    emit setWindowTitle(QFileInfo(filePath).fileName());
    loaded = image.load(filePath);
    // qDebug() << "loaded:" << loaded;
    if (loaded)
    {
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        currentInstrument = CURSOR;
        // qDebug() << "opened file" << filePath;
        emit setStatusSize(image.width(), image.height());
    }
    else
    {
        image = QImage();
        currentInstrument = NONE_INSTRUMENT;
        qDebug() << "failed to open file" << filePath;
    }
    // qDebug() << "image size" << image.byteCount();
    clearSelection();
    updateImageView();
    isEdited = false;
    return loaded;
}

void Photo::save()
{
    if(!filePath.isEmpty())
    {
        SelectionInstrument *instrument = static_cast <SelectionInstrument*> (instrumentsHandlers.at(CURSOR));
        // qDebug() << "isSelection" << instrument->isSelection();
        if (isEdited)
        {
            // qDebug() << "filePath:" << filePath;
            if (instrument->isSelection())
            {
                // qDebug() << "saving selection:" << instrument->getSelection();
                QImage image = this->image.copy(instrument->getSelection());
                emit onSave(image);
                image.save(filePath);
            }
            else
            {
                image.save(filePath);
            }

            /*
            // TODO: implement the following abstraction
            // TODO: how to define an order of execution (crop before size!)
            foreach instrument in instrumentsHandlers:
                instrument->applyOnSave(image)
            foreach effect in effectsHandlers:
                effect->applyOnSave(image)
            image.save(filePath)
            */
        }
    }
    /**
     * TODO: save (actionInstruments and) actionEvent
     */
    /* SCALE:
                mPImageArea->setImage(mPImageArea->getImage()->scaled(resizeDialog.getNewSize()));
                mPImageArea->resize(mPImageArea->getImage()->rect().right() + 6,
                                    mPImageArea->getImage()->rect().bottom() + 6);
                mPImageArea->setEdited(true);
                mPImageArea->clearSelection();

    */
    isEdited = false;
}

void Photo::updateImageView()
{
    // qDebug() << "image size" << image.size();
    // qDebug() << "window size" << size();
    // qDebug() << "image size bounded" << image.size().boundedTo(size());

    if (width() > image.width())
    {
        if (height() > image.height())
        {
            QSize newSize = image.size();
            imageView = image.scaled(newSize, Qt::KeepAspectRatio);
        }
        else
        {
            imageView = image.scaledToHeight(height());
        }
    }
    else if (height() > image.height())
    {
        imageView = image.scaledToWidth(width());
    }
    else
    {
        QSize newSize = image.size().boundedTo(size());
        imageView = image.scaled(newSize, Qt::KeepAspectRatio);
        // qDebug() << "imageView size:" << imageView.size();
    }
    viewScale = (float) imageView.width() / (float) image.width();
    update();
    emit setStatusSize(image.width(), image.height());
}

void Photo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, imageView, dirtyRect);

    int instrument = currentInstrument;
    if (instrument >= 0)
    {
        instrumentHandler = instrumentsHandlers.at(instrument);
        instrumentHandler->paintEvent(event, *this);
    }
}

void Photo::resizeEvent(QResizeEvent *event)
{
    updateImageView();
    QWidget::resizeEvent(event);
    if (currentInstrument != NONE_INSTRUMENT)
    {
        instrumentHandler = instrumentsHandlers.at(currentInstrument);
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
    int instrument = currentInstrument;
    if (instrument >= 0)
    {
        instrumentHandler = instrumentsHandlers.at(instrument);
        instrumentHandler->mousePressEvent(event, *this);
    }
}

void Photo::mouseMoveEvent(QMouseEvent *event)
{
    int instrument = currentInstrument;
    if (instrument >= 0)
    {
        instrumentHandler = instrumentsHandlers.at(instrument);
        instrumentHandler->mouseMoveEvent(event, *this);
    }
    if (image.rect().contains(event->pos()))
    {
        emit setStatusMouse(event->pos().x(), event->pos().y());
    }
    else
    {
        emit setStatusMouse();
    }
}

void Photo::mouseReleaseEvent(QMouseEvent *event)
{
    int instrument = currentInstrument;
    if (instrument >= 0)
    {
        instrumentHandler = instrumentsHandlers.at(instrument);
        instrumentHandler->mouseReleaseEvent(event, *this);
    }
}

void Photo::addUndoInformation()
{
    UndoCommand *command = new UndoCommand(getImage(), *this);
    undoStack->push(command);

}

void Photo::restoreCursor()
{
    setCursor(Qt::CrossCursor);

}
