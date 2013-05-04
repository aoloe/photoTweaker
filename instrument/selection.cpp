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
    selectionCreating = false;
    selectionResizing = NONE;
    selectionMoving = false;
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
    selectionCreating = true;
}

void SelectionInstrument::mouseMoveEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse moved";
    if (selectionCreating && rubberBand)
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    else if (selectionResizing != NONE)
    {
        switch (selectionResizing) {
            case N:
            break;
            case NE:
            break;
            case E:
            break;
            case SE:
            break;
            case S:
            break;
            case SW:
            break;
            case W:
            break;
            case NW:
            break;
            default:
                photo.restoreCursor();
        }
    }
    updateCursor(event, photo);
}

void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse released";
    selectionCreating = false;
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
    selectionCreating = false;
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
    // qDebug() << "update cursor";
    if (!selection.isEmpty())
    {
        const QRect selection = this->rubberBand->geometry();
        QRect selectionOuter = selection.adjusted(-6, -6, 6, 6);
        QRect selectionInner = selection.adjusted(3, 3, -3, -3);
        selectionResizing = NONE;
        selectionMoving = false;
        if (selectionOuter.contains(event->pos()))
        {
            if (selectionInner.contains(event->pos()))
            {
                photo.setCursor(Qt::SizeAllCursor);
                selectionMoving = true;
            }
            else
            {
                if (event->pos().y() < selectionInner.top())
                {
                    selectionResizing = static_cast<SelectionDirection>(selectionResizing | N);
                }
                else if (event->pos().y() > selectionInner.bottom())
                {
                    selectionResizing = static_cast<SelectionDirection>(selectionResizing | S);
                }

                if (event->pos().x() < selectionInner.left())
                {
                    selectionResizing = static_cast<SelectionDirection>(selectionResizing | E);
                }
                else if (event->pos().x() > selectionInner.right())
                {
                    selectionResizing = static_cast<SelectionDirection>(selectionResizing | W);
                }
                // qDebug() << "selectionResizing" << selectionResizing;

                switch (selectionResizing) {
                    case N:
                    case S:
                        photo.setCursor(Qt::SizeVerCursor);
                    break;
                    case W:
                    case E:
                        photo.setCursor(Qt::SizeHorCursor);
                    break;
                    case SE:
                    case NW:
                        photo.setCursor(Qt::SizeBDiagCursor);
                    break;
                    case SW:
                    case NE:
                        photo.setCursor(Qt::SizeFDiagCursor);
                    break;
                    default:
                        photo.restoreCursor();
                }
            }
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
