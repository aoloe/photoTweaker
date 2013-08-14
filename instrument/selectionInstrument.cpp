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
    selection = 0;
}

// --> entity-control-boundary pattern

void SelectionInstrument::createSelection(QPoint origin, Photo &photo)
{
    if (!selection)
    {
        selection = new Selection();
        selection->setImageArea(photo.getImage().rect());
        selection->setImageAreaView(photo.getImageView().rect());
        selection->setViewScale(viewScale);
    }
    selection->setMousePosition(origin); // necessary to calculate C movements
    selection->setOrigin(origin);
    selection->setSize();
    selection->start();
    if (!rubberBand)
    {
        rubberBand = new QRubberBand(QRubberBand::Rectangle, &photo);
    }
    else
    {
        rubberBand->setGeometry(QRect(0, 0, 0, 0));
    }
}

void SelectionInstrument::destructSelection()
{
    if (selection != NULL)
    {
        selection = NULL; // TODO: check if we should free the object's memory (ale/20130726)
        rubberBand->hide(); // TODO: check if necessary
        rubberBand = NULL;
    }
}


void SelectionInstrument::mousePressEvent(QMouseEvent *event, Photo &photo)
{
    if (selection == NULL)
    {
        createSelection(event->pos(), photo);
    }
    else
    {
        selection->setMousePosition(event->pos()); // necessary to calculate C movements
        if (selection->isMouseInSelection(event->pos()))
        {
            selection->detectActiveHandle(event->pos());
        } 
        else
        {
            createSelection(event->pos(), photo);
            // selection->setOrigin(event->pos());
            // selection->setSize();
            // selection->setActiveHandleCreation();
            // rubberBand->setGeometry(QRect());
        }
    }
}

void SelectionInstrument::mouseMoveEvent(QMouseEvent *event, Photo &photo)
{

    if (selection != NULL)
    {
        if (selection->isSelecting())
        {
            selection->calculateAreaView(event->pos());
            selection->setMousePosition(event->pos()); // necessary to calculate C movements
            // qDebug() << "getArea" << selection->getArea();
            rubberBand->setGeometry(selection->getAreaView()); // TODO: or selection->draw()?
            rubberBand->show();
        }
        updateCursor(event, photo);
    }
}

void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
{
    // this->selection = QRect(selection.topLeft() / viewScale, selection.size() / viewScale);

    if (selection->isEmpty())
    {
        destructSelection();
    }
    else
    {
        selection->stop();
    }
    updateCursor(event, photo);
    photo.setEdited(true);
}

void SelectionInstrument::resizeEvent(QResizeEvent *event, Photo &photo)
{
    if (selection != NULL)
    {
        selection->setImageAreaView(photo.getImageView().rect());
        selection->setImageArea(photo.getImage().rect());
        selection->resize(viewScale);
        rubberBand->setGeometry(selection->getAreaView()); // TODO: or selection->draw()?
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
        // qDebug() << "paintEvent";
        // qDebug() << "parent" << parent();
        QWidget* parentWidget = static_cast <QWidget*> (parent());
        // qDebug() << "geometry" << parentWidget->geometry();
        // qDebug() << "parent geometry" << parentWidget->geometry();
        // qDebug() << "photo geometry" << photo.geometry();
        QRect surface = (static_cast <QWidget*> (parent()))->geometry();
        // qDebug() << "surface" << surface;
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
    if (selection != NULL)
    {
        if (selection->isMouseInSelection(event->pos()))
        {
            handle = selection->getActiveHandle(event->pos());
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
