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
#include <QtGui/QWidget> // for painting the emi transparent background
#include <QtGui/QPainterPath> // for painting the emi transparent background
// #include <QtGui/QBrush> // for painting the emi transparent background

SelectionInstrument::SelectionInstrument(QObject *parent) :
    AbstractInstrument(parent)
{
    rubberBand = 0;
    selection = QRect();
    viewScale = 0.0;
    isSelecting = false;
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
    updateCursor(event, photo);
}

void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse released";
    isSelecting = false;
    QRect selection = rubberBand->geometry();
    
    this->selection = QRect(selection.topLeft() / viewScale, selection.size() / viewScale);

    updateCursor(event, photo);
}

void SelectionInstrument::resizeEvent(QResizeEvent *event, Photo &photo)
{
    if (!selection.isEmpty())
    {
        QRect selection = QRect(this->selection.topLeft() * viewScale, this->selection.size() * viewScale);
        // qDebug() << "new selection:" << selection;
        rubberBand->setGeometry(selection);
    }
}

void SelectionInstrument::clearSelection()
{
    qDebug() << "clear selection";
    isSelecting = false;
    selection = QRect();
    if (rubberBand)
        rubberBand->hide();
}

void SelectionInstrument::paint(Photo &photo, bool, bool)
{
}

void SelectionInstrument::paintEvent(QPaintEvent* event, Photo &photo)
{
    // this should put a shade on the non selected area but does not really work..
    // should be improved, before it can be activated...
    /*
    if (rubberBand && isSelection())
    {
        qDebug() << "paintEvent";
        qDebug() << "parent" << parent();
        QWidget* parentWidget = static_cast <QWidget*> (parent());
        qDebug() << "geometry" << parentWidget->geometry();
        qDebug() << "parent geometry" << parentWidget->geometry();
        qDebug() << "photo geometry" << photo.geometry();
        QRect surface = (static_cast <QWidget*> (parent()))->geometry();
        qDebug() << "surface" << surface;
        QPainterPath path = QPainterPath();
        path.setFillRule(Qt::OddEvenFill);
        path.addRect(static_cast <QRectF>(surface));
        // path.addRect(static_cast <QRectF>(rubberBand->geometry()));
        QPainter painter(parentWidget);
        painter.setBrush(QBrush(QColor(100, 100, 100, 70)));
        painter.setPen(QPen(QColor(0, 0, 0, 80)));
        painter.drawPath(path);
    }
    */
}


void SelectionInstrument::updateCursor(QMouseEvent *event, Photo &photo)
{
    qDebug() << "update cursor";
    if (!selection.isEmpty())
    {
        const QRect selection = this->rubberBand->geometry();
        if (selection.adjusted(3, 3, -3, -3).contains(event->pos(), true))
        {
            photo.setCursor(Qt::SizeAllCursor);
        }
        else if (selection.adjusted(-6, -6, 6, 6).contains(event->pos(), true))
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
