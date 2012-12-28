/*
 * This source file is part of EasyPaint.
 *
 * Copyright (c) 2012 EasyPaint <https://github.com/Gr1N/EasyPaint>
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

#include <QtGui/QPainter>
#include <QtGui/QApplication>

SelectionInstrument::SelectionInstrument(QObject *parent) :
    AbstractInstrument(parent)
{
    isSelectionExists = isSelectionMoving = isSelectionResizing
            = isPaint = isImageSelected = false;
}

void SelectionInstrument::mousePressEvent(QMouseEvent *event, Photo &photo)
{
    if (isSelectionExists)
    {
        photo.setImage(imageCopy);
        paint(photo);
        if (event->pos().x() > topLeftPoint.x() &&
                event->pos().x() < bottomRightPoint.x() &&
                event->pos().y() > topLeftPoint.y() &&
                event->pos().y() < bottomRightPoint.y())
        {
            makeUndoCommand(photo);
            if (!isImageSelected)
            {
                startMoving(photo);
                isImageSelected = true;
            } 
            else
            {
                drawBorder(photo);
            }
            isSelectionMoving = true;
            moveDiffPoint = bottomRightPoint - event->pos();
            return;
        }
        else if (event->pos().x() >= bottomRightPoint.x() &&
                 event->pos().x() <= bottomRightPoint.x() + 6 &&
                 event->pos().y() >= bottomRightPoint.y() &&
                 event->pos().y() <= bottomRightPoint.y() + 6)
        {
            makeUndoCommand(photo);
            startResizing(photo);
            isSelectionResizing = true;
            return;
        }
        else
        {
            clearSelection(photo);
        }
    }
    if(event->button() == Qt::LeftButton)
    {
        bottomRightPoint = topLeftPoint = event->pos();
        height =  width = 0;
        imageCopy = photo.getImage();
        startSelection(photo);
        isPaint = true;
    }
}

void SelectionInstrument::mouseMoveEvent(QMouseEvent *event, Photo &photo)
{
    if (isSelectionExists)
    {
        if (isSelectionMoving)
        {
            bottomRightPoint = event->pos() +
                                           moveDiffPoint;
            topLeftPoint = event->pos() + moveDiffPoint -
                                  QPoint(width - 1, height - 1);
            photo.setImage(imageCopy);
            move(photo);
            drawBorder(photo);
            isPaint = false;
        }
        else if (isSelectionResizing)
        {
            bottomRightPoint = event->pos();
            height = fabs(topLeftPoint.y() - bottomRightPoint.y()) + 1;
            width = fabs(topLeftPoint.x() - bottomRightPoint.x()) + 1;
            photo.setImage(imageCopy);
            resize(photo);
            drawBorder(photo);
            isPaint = false;
        }
    }
    if (isPaint)
    {
        bottomRightPoint = event->pos();
        height = fabs(topLeftPoint.y() - bottomRightPoint.y()) + 1;
        width = fabs(topLeftPoint.x() - bottomRightPoint.x()) + 1;
        photo.setImage(imageCopy);
        drawBorder(photo);
        select(photo);
    }
    updateCursor(event, photo);
}

void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
{
    int right = topLeftPoint.x() > bottomRightPoint.x() ? topLeftPoint.x() : bottomRightPoint.x();
    int bottom = topLeftPoint.y() > bottomRightPoint.y() ? topLeftPoint.y() : bottomRightPoint.y();
    int left = topLeftPoint.x() < bottomRightPoint.x() ? topLeftPoint.x() : bottomRightPoint.x();
    int top = topLeftPoint.y() < bottomRightPoint.y() ? topLeftPoint.y() : bottomRightPoint.y();
    bottomRightPoint = QPoint(right, bottom);
    topLeftPoint = QPoint(left, top);

    if (isSelectionExists)
    {
        updateCursor(event, photo);
        if(isSelectionMoving)
        {
            photo.setImage(imageCopy);
            completeMoving(photo);
            paint(photo);
            drawBorder(photo);
            isPaint = false;
            isSelectionMoving = false;
        }
        else if (isSelectionResizing)
        {
            photo.setImage(imageCopy);
            paint(photo);
            completeResizing(photo);
            paint(photo);
            drawBorder(photo);
            isPaint = false;
            isSelectionResizing = false;
        }
    }
    if (isPaint)
    {
        if (event->button() == Qt::LeftButton)
        {
            photo.setImage(imageCopy);
            if (topLeftPoint != bottomRightPoint)
            {
                photo.setImage(imageCopy);
                paint(photo);
                completeSelection(photo);
                paint(photo);
                isSelectionExists = true;
            }
            drawBorder(photo);
            isPaint = false;
        }
    }
}

void SelectionInstrument::drawBorder(Photo &photo)
{
    if (width > 1 && height > 1)
    {
        QPainter painter(photo.getImagePointer());
        painter.setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        painter.setBackgroundMode(Qt::TransparentMode);
        if(topLeftPoint != bottomRightPoint)
        {
            painter.drawRect(QRect(topLeftPoint, bottomRightPoint - QPoint(1, 1)));
        }
        photo.setEdited(true);
        painter.end();
        photo.update();
    }
}

void SelectionInstrument::clearSelection(Photo &photo)
{
    if (isSelectionExists)
    {
        photo.setImage(imageCopy);
        paint(photo);
        imageCopy = photo.getImage();
        isSelectionExists = isSelectionMoving = isSelectionResizing
                = isPaint = isImageSelected = false;
        photo.update(); 
        photo.restoreCursor();
        clear();
    }
}

void SelectionInstrument::saveImageChanges(Photo &)
{
}

void SelectionInstrument::updateCursor(QMouseEvent *event, Photo &photo)
{
    if (isSelectionExists)
    {
        if (event->pos().x() > topLeftPoint.x() &&
            event->pos().x() < bottomRightPoint.x() &&
            event->pos().y() > topLeftPoint.y() &&
            event->pos().y() < bottomRightPoint.y())
        { 
            photo.setCursor(Qt::SizeAllCursor);
        }
        else if (event->pos().x() >= bottomRightPoint.x() &&
                 event->pos().x() <= bottomRightPoint.x() + 6 &&
                 event->pos().y() >= bottomRightPoint.y() &&
                 event->pos().y() <= bottomRightPoint.y() + 6)
        {
            photo.setCursor(Qt::SizeFDiagCursor);
        }
        else
        {
            photo.restoreCursor();
        }
    }
    else
    {
        photo.restoreCursor();
    }
}


void SelectionInstrument::startSelection(Photo &)
{
}

void SelectionInstrument::startResizing(Photo &photo)
{
}

void SelectionInstrument::startMoving(Photo &photo)
{
}

void SelectionInstrument::select(Photo &)
{
}

void SelectionInstrument::resize(Photo &)
{
}

void SelectionInstrument::move(Photo &)
{
}

void SelectionInstrument::completeSelection(Photo &photo)
{
    selectedImage = photo.getImage().copy(
        topLeftPoint.x(),
        topLeftPoint.y(),
        width, height
    );
}

void SelectionInstrument::completeResizing(Photo &photo)
{
    selectedImage = photo.getImage().copy(
        topLeftPoint.x(),
        topLeftPoint.y(),
        width, height
    );
}

void SelectionInstrument::completeMoving(Photo &)
{
}

void SelectionInstrument::clear()
{
    selectedImage = QImage();
}

void SelectionInstrument::paint(Photo &photo, bool, bool)
{
    if (isSelectionExists)
    {
        if(topLeftPoint != bottomRightPoint)
        {
            QPainter painter(photo.getImagePointer());
            QRect source(0, 0, selectedImage.width(), selectedImage.height());
            QRect target(topLeftPoint, bottomRightPoint);
            painter.drawImage(target, selectedImage, source);
            painter.end();
        }
        photo.setEdited(true);
        photo.update();
    }
}
