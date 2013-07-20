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
#include <QtGui/QWidget> // for painting the emi transparent background
#include <QtGui/QPainterPath> // for painting the emi transparent background
// #include <QtGui/QBrush> // for painting the emi transparent background

/*
void SelectionInstrument::mousePressEvent(QMouseEvent *event, Photo &photo)
void SelectionInstrument::mouseMoveEvent(QMouseEvent *event, Photo &photo)
void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
void SelectionInstrument::resizeEvent(QResizeEvent *event, Photo &photo)
void SelectionInstrument::clearSelection()
void SelectionInstrument::paint(Photo &photo, bool, bool)
void SelectionInstrument::paintEvent(QPaintEvent* event, Photo &photo)
void SelectionInstrument::updateCursor(QMouseEvent *event, Photo &photo)




SelectionInstrument::Direction SelectionInstrument::getCardinalDirection(QPoint point, QRect area)
*/

SelectionInstrument::SelectionInstrument(QObject *parent) :
    AbstractInstrument(parent)
{
    rubberBand = 0;
    selection = QRect();
    viewScale = 0.0;
    selectionCreating = false;
    mouseLastPosition = QPoint(0,0);
    mouseOnSelection = NONE;
    clickOnSelection = NONE;
}

// --> entity-control-boundary pattern

SelectionInstrument::Direction SelectionInstrument::abcd(QRect selection, QPoint pos)
{
    enum Direction mouseOnSelection = NONE;
    if (!selection.isEmpty())
    {
        QRect selectionOuter = selection.adjusted(-6, -6, 6, 6);
        QRect selectionInner = selection.adjusted(3, 3, -3, -3);
        if (selectionOuter.contains(pos))
        {
            mouseOnSelection = getCardinalDirection(pos, selectionInner);
            // qDebug() << "mouseOnSelection" << mouseOnSelection;
        }
    }
    return mouseOnSelection;
}

void SelectionInstrument::mousePressEvent(QMouseEvent *event, Photo &photo)
{
    // TODO: rename selection_o to selection and only define it there is actually a selection
    selection_o = new Selection();
    if (this->rubberBand != NULL)
    {
        selection_o->setSelection(this->rubberBand->geometry());
    }
    selection_o->detectActiveHandle(event->pos())

    enum Direction mouseOnSelection = NONE;
    mouseOnSelection = abcd(this->rubberBand == NULL ? QRect() : this->rubberBand->geometry(), event->pos());


    if (mouseOnSelection == NONE)
    {
        origin = event->pos();
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, &photo);
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
        selectionCreating = true;
    }
    else
    {
        clickOnSelection = mouseOnSelection;
        if (clickOnSelection == C)
        {
            mouseLastPosition = event->pos();
        }
    }
}

void SelectionInstrument::mouseMoveEvent(QMouseEvent *event, Photo &photo)
{
    if (selectionCreating && rubberBand)
        rubberBand->setGeometry(photo.getImageView().rect().intersected(QRect(origin, event->pos())));
    else if (clickOnSelection != NONE)
    {
        // TODO: according to http://harmattan-dev.nokia.com/docs/platform-api-reference/xml/daily-docs/libqt4/qrect.html#coordinates we should use x() + width() and y() + height(), and avoid right() and bottom().
        QRect selection = rubberBand->geometry();
        int dx1 = 0;
        int dx2 = 0;
        int dy1 = 0;
        int dy2 = 0;
        if (clickOnSelection == C) {
            // moving around the selection
            QPoint position = event->pos();
            QImage view = photo.getImageView();
            if (view.rect().contains(position)) {
                int dx = position.x() - mouseLastPosition.x();
                int dy = position.y() - mouseLastPosition.y();
                if ((selection.left() + dx >= 0) && (selection.right() + dx <= photo.getImageView().width() - 1))
                {
                    dx1 += dx;
                    dx2 += dx;
                }
                if ((selection.top() + dy >= 0) && (selection.bottom() + dy <= photo.getImageView().height() - 1))
                {
                    dy1 += dy;
                    dy2 += dy;
                }
            } else {
                Direction direction = getCardinalDirection(position, view.rect());
                if ((direction & N) &&  (selection.top() > view.rect().top()))
                {
                    dy1 = view.rect().top() - selection.top(); 
                }
                else if ((direction & S) && (selection.bottom() < view.rect().bottom()))
                {
                    dy1 = view.rect().bottom() - selection.bottom(); 
                }
                if ((direction & E) && (selection.right() < view.rect().right()))
                {
                    dx1 = view.rect().right() - selection.right(); 
                }
                else if ((direction & W) && (selection.left() > view.rect().left()))
                {
                    dx1 =  view.rect().left() - selection.left(); 
                }
                dy2 = dy1;
                dx2 = dx1;
            }
            mouseLastPosition = position;
            rubberBand->setGeometry(selection.adjusted(dx1, dy1, dx2, dy2));
        } else {
            // extending / shrinking the selection
            switch (clickOnSelection) {
                case N:
                    dy1 = event->pos().y() - selection.top();
                break;
                case S:
                    dy2 =  event->pos().y() - selection.bottom();
                break;
                case W:
                    dx1 =  event->pos().x() - selection.left();
                break;
                case E:
                    dx2 =  event->pos().x() - selection.right();
                break;
                case SE:
                    dx2 = event->pos().x() - selection.bottomRight().x();
                    dy2 = event->pos().y() - selection.bottomRight().y();
                break;
                case NW:
                    dx1 =  event->pos().x() - selection.topLeft().x();
                    dy1 =  event->pos().y() - selection.topLeft().y();
                break;
                case SW:
                    dx1 = event->pos().x() - selection.bottomLeft().x();
                    dy2 = event->pos().y() - selection.bottomLeft().y();
                break;
                case NE:
                    dx2 =  event->pos().x() - selection.topRight().x();
                    dy1 =  event->pos().y() - selection.topRight().y();
                break;
            }
            // ensure that the selection is at least 1x1 and don't allow negative selections
            if (event->pos().x() <= selection.left())
            {
                dx2 = 0;
            }
            if (event->pos().y() <= selection.top())
            {
                dy2 = 0;
            }
            // update the selection with the adjusted selection, but do not go outside of the image boundaries
            rubberBand->setGeometry(photo.getImageView().rect().intersected(selection.adjusted(dx1, dy1, dx2, dy2)));
        }
        // qDebug() << "updating the selection";
    }
    else
    {
    // qDebug() << "updating the cursor";
    updateCursor(event, photo);
    }
}

void SelectionInstrument::mouseReleaseEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse released";
    selectionCreating = false;
    QRect selection = rubberBand->geometry();
    
    this->selection = QRect(selection.topLeft() / viewScale, selection.size() / viewScale);

    mouseOnSelection = NONE;
    clickOnSelection = NONE;

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

/**
 * get the relative position of a point inside or around a rectangular area
 */
SelectionInstrument::Direction SelectionInstrument::getCardinalDirection(QPoint point, QRect area)
{
    enum SelectionInstrument::Direction result = SelectionInstrument::NONE;

    if (area.contains(point))
    {
        result = SelectionInstrument::C;
    }
    else
    {
        if (point.y() < area.top())
        {
            result = static_cast<Direction>(result | SelectionInstrument::N);
        }
        else if (point.y() > area.bottom())
        {
            result = static_cast<Direction>(result | SelectionInstrument::S);
        }

        if (point.x() < area.left())
        {
            result = static_cast<Direction>(result | SelectionInstrument::W);
        }
        else if (point.x() > area.right())
        {
            result = static_cast<Direction>(result | SelectionInstrument::E);
        }

    }
    return result;
}

void SelectionInstrument::updateCursor(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "update cursor";
    if (!selection.isEmpty())
    {
        const QRect selection = this->rubberBand->geometry();
        QRect selectionOuter = selection.adjusted(-6, -6, 6, 6);
        QRect selectionInner = selection.adjusted(3, 3, -3, -3);
        mouseOnSelection = NONE;
        if (selectionOuter.contains(event->pos()))
        {
            mouseOnSelection = getCardinalDirection(event->pos(), selectionInner);
            // qDebug() << "mouseOnSelection" << mouseOnSelection;
        }
    }
    switch (mouseOnSelection) {
        case N:
        case S:
            photo.setCursor(Qt::SizeVerCursor);
        break;
        case W:
        case E:
            photo.setCursor(Qt::SizeHorCursor);
        break;
        case SW:
        case NE:
            photo.setCursor(Qt::SizeBDiagCursor);
        break;
        case SE:
        case NW:
            photo.setCursor(Qt::SizeFDiagCursor);
        break;
        case C:
            photo.setCursor(Qt::SizeAllCursor);
        break;
        default:
            photo.restoreCursor();
    }
}
