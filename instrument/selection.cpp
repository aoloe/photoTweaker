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

#include "instrument/abstractinstrument.h"
#include "selection.h"
#include "../photo.h"
#include "../undocommand.h"
#include "math.h"

#include <QDebug>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QRubberBand>

SelectionInstrument::SelectionInstrument(QObject *parent) :
    AbstractInstrument(parent)
{
    rubberBand = 0;
    selection = QRect();
    viewScale = 0.0;
    isSelecting = false;
    isSelection = false;
}

void SelectionInstrument::mousePressEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse pressed";
    // TODO: catch (and allow) resizing/moving the current selection with the mouse
    // (c.f. how it is done in easypaint)
    origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, &photo);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
    isSelecting = true;
}

void SelectionInstrument::mouseMoveEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse moved";
    if (isSelecting && rubberBand)
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse released";
    isSelecting = false;
    isSelection = true;
    QRect selection = rubberBand->geometry();
    
    this->selection = QRect(selection.topLeft() / viewScale, selection.size() / viewScale);
}

void SelectionInstrument::resizeEvent(QResizeEvent *event, Photo &photo)
{
    if (isSelection)
    {
        QRect selection = QRect(this->selection.topLeft() * viewScale, this->selection.size() * viewScale);
        // qDebug() << "new selection:" << selection;
        rubberBand->setGeometry(selection);
    }
}

void SelectionInstrument::clearSelection()
{
    isSelecting = false;
    isSelection = false;
    rubberBand->hide();
    selection = QRect();
}

void SelectionInstrument::paint(Photo &photo, bool, bool)
{
}
