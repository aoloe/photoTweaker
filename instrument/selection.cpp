#include <QDebug>
#include "selection.h"

Selection::Selection()
{
    area = QRect();
    areaView = QRect();
    activeHandle = NONE;
    mousePosition = QPoint(0,0);
    /*
    mouseOnSelection = NONE;
    clickOnSelection = NONE;
    */
}

/**
 * TODO: is it used? (ale/20130809)
 */
Selection::Selection(QRect area)
{
    Selection();
    this->area = area;
}

bool Selection::isMouseInSelection(QPoint pos)
{
    bool result = false;
    if (!areaView.isEmpty())
    {
        QRect selectionOuter = areaView.adjusted(-6, -6, 6, 6);
        result = selectionOuter.contains(pos);
    }
    return result;
}

void Selection::start()
{
    creatingArea = true;
    activeHandle = NONE;
}
void Selection::stop()
{
    creatingArea = false;
    activeHandle = NONE;
    resizeArea();
}

bool Selection::isSelecting() {
    return (activeHandle != NONE || creatingArea);
}

void Selection::setOrigin(QPoint origin)
{ 
    // qDebug() << "areaView" << areaView;
    // qDebug() << "origin" << origin;
    areaView.setX(origin.x());
    areaView.setY(origin.y());
    // qDebug() << "areaView" << areaView;
}

void Selection::resize(qreal ratio)
{
    viewScale = ratio;
    areaView.setX((int) area.x() * ratio);
    areaView.setY((int) area.y() * ratio);
    areaView.setWidth((int) area.width() * ratio);
    areaView.setHeight((int) area.height() * ratio);
}

void Selection::resizeArea()
{
    area.setX((int) areaView.x() / viewScale);
    area.setY((int) areaView.y() / viewScale);
    area.setWidth((int) areaView.width() / viewScale);
    area.setHeight((int) areaView.height() / viewScale);
}


Selection::Direction Selection::getActiveHandle(QPoint pos)
{
    Direction result = NONE;
    if (!areaView.isEmpty())
    {
        QRect selectionInner = areaView.adjusted(3, 3, -3, -3);
        result = getCardinalDirection(selectionInner, pos);
    }
    return result;
}

void Selection::detectActiveHandle(QPoint pos)
{
    activeHandle = getActiveHandle(pos);
}

/**
 * retuns the relative position of qpoint towards the area as a cardinal point
 * (N, S, W, E, NE, SE, NW, SW)
 */
Selection::Direction Selection::getCardinalDirection(QRect rect, QPoint point)
{
    enum Direction result = NONE;

    if (rect.contains(point))
    {
        result = INNER;
    }
    else
    {
        if (point.y() < rect.top())
        {
            result = static_cast<Direction>(result | N);
        }
        else if (point.y() > rect.bottom())
        {
            result = static_cast<Direction>(result | S);
        }

        if (point.x() < rect.left())
        {
            result = static_cast<Direction>(result | W);
        }
        else if (point.x() > rect.right())
        {
            result = static_cast<Direction>(result | E);
        }

    }
    return result;
}

void Selection::calculateAreaView(QPoint position)
{
    int dx1 = 0;
    int dx2 = 0;
    int dy1 = 0;
    int dy2 = 0;

    if (activeHandle == INNER)
    {
         // when moving the selection...
        if (imageArea.contains(position))
        {
            // ... that the selection does not move out of the image area...
            int dx = position.x() - mousePosition.x();
            int dy = position.y() - mousePosition.y();
            if (areaView.left() + dx < 0)
            {
                dx1 = -areaView.left();
            }
            else if (areaView.right() + dx > imageAreaView.width() - 1)
            {
                dx1 = imageAreaView.width() - areaView.right();
            }
            else
            {
                dx1 = dx;
            }
            if (areaView.top() + dy < 0)
            {
                dy1 = -areaView.top();
            }
            else if (areaView.bottom() + dy > imageAreaView.height() - 1)
            {
                dy1 = imageAreaView.height() - areaView.bottom();
            }
            else
            {
                dy1 = dy;
            }
        }
        else
        {
            // ... and touches the border when the mouse goes out of the image area.
            Direction direction = getCardinalDirection(imageAreaView, position);
            if ((direction & N) &&  (areaView.top() > imageAreaView.top()))
            {
                dy1 = imageAreaView.top() - areaView.top(); 
            }
            else if ((direction & S) && (areaView.bottom() < imageAreaView.bottom()))
            {
                dy1 = imageAreaView.bottom() - areaView.bottom(); 
            }
            if ((direction & E) && (areaView.right() < imageAreaView.right()))
            {
                dx1 = imageAreaView.right() - areaView.right(); 
            }
            else if ((direction & W) && (areaView.left() > imageAreaView.left()))
            {
                dx1 =  imageAreaView.left() - areaView.left(); 
            }
        }
        dx2 = dx1;
        dy2 = dy1;
        areaView.adjust(dx1, dy1, dx2, dy2);
    }
    else if (creatingArea)
    {
        int dx = position.x() - areaView.x();
        int dy = position.y() - areaView.y();
        if ((dx != 0) && (dy != 0))
        {
            activeHandle = W;
            if (dx > 0)
            {
                activeHandle = E;
            }
            if (dy > 0)
            {
                activeHandle = static_cast<Direction>(activeHandle | S);
            }
            else
            {
                activeHandle = static_cast<Direction>(activeHandle | N);
            }
            // qDebug() << "activeHandle" << activeHandle;
            areaView.setWidth(qAbs(dx));
            areaView.setHeight(qAbs(dy));
            creatingArea = false;
        }
    }
    else
    {
        // resize the selection (up to the image area)
        switch (activeHandle) {
            case N:
                dy1 = position.y() - areaView.top();
            break;
            case S:
                dy2 =  position.y() - areaView.bottom();
            break;
            case W:
                dx1 =  position.x() - areaView.left();
            break;
            case E:
                dx2 =  position.x() - areaView.right();
            break;
            case SE:
                dx2 = position.x() - areaView.bottomRight().x();
                dy2 = position.y() - areaView.bottomRight().y();
            break;
            case NW:
                dx1 =  position.x() - areaView.topLeft().x();
                dy1 =  position.y() - areaView.topLeft().y();
            break;
            case SW:
                dx1 = position.x() - areaView.bottomLeft().x();
                dy2 = position.y() - areaView.bottomLeft().y();
            break;
            case NE:
                dx2 =  position.x() - areaView.topRight().x();
                dy1 =  position.y() - areaView.topRight().y();
            break;
        }
        // ensure that the area is at least 1x1 and don't allow negative areas
        if (position.x() <= areaView.left())
        {
            dx2 = 0;
        }
        if (position.y() <= areaView.top())
        {
            dy2 = 0;
        }
        areaView.adjust(dx1, dy1, dx2, dy2);
        areaView = areaView.intersected(imageAreaView);
    }
}
