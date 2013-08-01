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
#include "selectionInstrument.h"
#include "selection.h"
#include "../photo.h"
#include "../undocommand.h"
#include "math.h"

#include <QDebug>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QRubberBand>
#include <QtGui/QWidget> // for painting the semi-transparent background
#include <QtGui/QPainterPath> // for painting the semi-transparent background
// #include <QtGui/QBrush> // for painting the semi-transparent background

SelectionInstrument::SelectionInstrument(QObject *parent) :
    AbstractInstrument(parent)
{
    rubberBand = 0;
    selection_o = 0;
}

// --> entity-control-boundary pattern

void SelectionInstrument::createSelection(QPoint origin, Photo &photo)
{
    // TODO: rename selection_o to selection
    selection_o = new Selection();
    selection_o->setImageArea(photo.getImageView().rect());
    selection_o->setMousePosition(origin); // necessary to calculate C movements
    selection_o->setOrigin(origin);
    selection_o->setActiveHandleCreation();
    rubberBand = new QRubberBand(QRubberBand::Rectangle, &photo);
}

void SelectionInstrument::destructSelection()
{
    if (selection_o != NULL)
    {
        selection_o = NULL; // TODO: check if we should free the object's memory (ale/20130726)
        rubberBand->hide(); // TODO: check if necessary
        rubberBand = NULL;
    }
}


void SelectionInstrument::mousePressEvent(QMouseEvent *event, Photo &photo)
{
    if (selection_o == NULL)
    {
        createSelection(event->pos(), photo);
    }
    else
    {
        selection_o->setMousePosition(event->pos()); // necessary to calculate C movements
        if (selection_o->isMouseInSelection(event->pos()))
        {
            selection_o->detectActiveHandle(event->pos());
        } 
        else
        {
            selection_o->setOrigin(event->pos());
            selection_o->setSize();
            selection_o->setActiveHandleCreation();
            rubberBand->setGeometry(QRect());
        }
    }
}

void SelectionInstrument::mouseMoveEvent(QMouseEvent *event, Photo &photo)
{

    if (selection_o != NULL)
    {
        if (selection_o->hasActiveHandle())
        {
            selection_o->calculateArea(event->pos());
            selection_o->setMousePosition(event->pos()); // necessary to calculate C movements
            qDebug() << "getArea" << selection_o->getArea();
            rubberBand->setGeometry(selection_o->getArea()); // TODO: or selection_o->draw()?
            rubberBand->show();
        }
        updateCursor(event, photo);
    }
}

void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
{
    // this->selection = QRect(selection.topLeft() / viewScale, selection.size() / viewScale);

    if (selection_o->isEmpty())
    {
        destructSelection();
    }
    else
    {
        selection_o->releaseActiveHandle();
    }
    updateCursor(event, photo);
}

void SelectionInstrument::resizeEvent(QResizeEvent *event, Photo &photo)
{
    if (selection_o != NULL)
    {
        selection_o->setImageArea(photo.getImageView().rect());
        selection_o->resize(viewScale);
        rubberBand->setGeometry(selection_o->getArea()); // TODO: or selection_o->draw()?
    }
}

/**
 * called by Photo when opening a new file
 */
void SelectionInstrument::clearSelection()
{
    destructSelection();
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

/**
 * TODO: probably, if other instruments are created (or some effects want to set a cursor) this method
 * will overwrite the value of the cursor: find a better way to do it!
 */
void SelectionInstrument::updateCursor(QMouseEvent *event, Photo &photo)
{
    Selection::Direction handle = Selection::NONE;
    if (selection_o != NULL)
    {
        if (selection_o->isMouseInSelection(event->pos()))
        {
            handle = selection_o->getActiveHandle(event->pos());
        }
    }
    switch (handle) {
        case Selection::N:
        case Selection::S:
            photo.setCursor(Qt::SizeVerCursor);
        break;
        case Selection::W:
        case Selection::E:
            photo.setCursor(Qt::SizeHorCursor);
        break;
        case Selection::SW:
        case Selection::NE:
            photo.setCursor(Qt::SizeBDiagCursor);
        break;
        case Selection::SE:
        case Selection::NW:
            photo.setCursor(Qt::SizeFDiagCursor);
        break;
        case Selection::INNER:
            photo.setCursor(Qt::SizeAllCursor);
        break;
        default:
            photo.restoreCursor();
    }
}
