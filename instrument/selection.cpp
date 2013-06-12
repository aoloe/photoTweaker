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
    mouseLastPosition = QPoint(0,0);
    mouseOnSelection = NONE;
    clickOnSelection = NONE;
}

void SelectionInstrument::mousePressEvent(QMouseEvent *event, Photo &photo)
{
    // qDebug() << "mouse pressed";
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
    // qDebug() << "mouse moved";
    if (selectionCreating && rubberBand)
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    else if (clickOnSelection != NONE)
    {
        QRect selection = rubberBand->geometry();
        int dx1 = 0;
        int dx2 = 0;
        int dy1 = 0;
        int dy2 = 0;
        // TODO: catch the cases where the selections becomes "negative" and change the origin accordingly (even if it's an uninteresting corner case...)
        // TODO: check if it's possible to keep the mouse at the same place in the selection when the border is reached
        //
        // if (event->pos().y() 
        switch (clickOnSelection) {
            case N:
                dy1 = event->pos().y() - selection.top();
            break;
            case S:
                qDebug() << "move S";
                dy2 =  event->pos().y() - selection.bottom();
                qDebug() << "dy2" << dy2;
            break;
            case W:
                qDebug() << "move W";
                dx1 =  event->pos().x() - selection.left();
            break;
            case E:
                qDebug() << "move E";
                dx2 =  event->pos().x() - selection.right();
            break;
            case SE:
                qDebug() << "move SE";
                dx2 = event->pos().x() - selection.bottomRight().x();
                dy2 = event->pos().y() - selection.bottomRight().y();
            break;
            case NW:
                qDebug() << "move NW";
                dx1 =  event->pos().x() - selection.topLeft().x();
                dy1 =  event->pos().y() - selection.topLeft().y();
                // rubberBand->setGeometry(QRect(selection.bottomRight(), QSize(width, height)));
                // rubberBand->setGeometry(QRect(origin, QSize())
            break;
            case SW:
                qDebug() << "move SW";
                dx1 = event->pos().x() - selection.bottomLeft().x();
                dy2 = event->pos().y() - selection.bottomLeft().y();
            break;
            case NE:
                qDebug() << "move NE";
                dx2 =  event->pos().x() - selection.topRight().x();
                dy1 =  event->pos().y() - selection.topRight().y();
            break;
            case C:
                QPoint position = event->pos();
                int dx = position.x() - mouseLastPosition.x();
                int dy = position.y() - mouseLastPosition.y();
                if ((selection.left() + dx >= 0) && (selection.right() + dx <= photo.getImageView().width()))
                {
                    dx1 += dx;
                    dx2 += dx;
                }
                if ((selection.top() + dy >= 0) && (selection.bottom() + dy <= photo.getImageView().height()))
                {
                    dy1 += dy;
                    dy2 += dy;
                }
                mouseLastPosition = position;
            break;
        }
        rubberBand->setGeometry(selection.adjusted(dx1, dy1, dx2, dy2));
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

    // qDebug() << "x" << event->pos().x();
    qDebug() << "selection left" << selection.left();
    qDebug() << "selection width" << selection.width();
    qDebug() << "image width" << photo.getImageView().width();
    
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
        mouseOnSelection = NONE;
        if (selectionOuter.contains(event->pos()))
        {
            if (selectionInner.contains(event->pos()))
            {
                mouseOnSelection = C;
            }
            else
            {
                if (event->pos().y() < selectionInner.top())
                {
                    mouseOnSelection = static_cast<Direction>(mouseOnSelection | N);
                }
                else if (event->pos().y() > selectionInner.bottom())
                {
                    mouseOnSelection = static_cast<Direction>(mouseOnSelection | S);
                }

                if (event->pos().x() < selectionInner.left())
                {
                    mouseOnSelection = static_cast<Direction>(mouseOnSelection | W);
                }
                else if (event->pos().x() > selectionInner.right())
                {
                    mouseOnSelection = static_cast<Direction>(mouseOnSelection | E);
                }

            }
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
