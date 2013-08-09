#include <QDebug>
#include "selection.h"

Selection::Selection()
{
    area = QRect();
    activeHandle = NONE;
    mousePosition = QPoint(0,0);
    /*
    mouseOnSelection = NONE;
    clickOnSelection = NONE;
    */
}

Selection::Selection(QRect area)
{
    Selection();
    this->area = area;
}

void Selection::setOrigin(QPoint origin)
{ 
    // qDebug() << "area" << area;
    // qDebug() << "origin" << origin;
    area.setX(origin.x());
    area.setY(origin.y());
    // qDebug() << "area" << area;
}

void Selection::resize(qreal ratio)
{
    area.setX((int) area.x() * ratio);
    area.setY((int) area.y() * ratio);
    area.setWidth((int) area.width() * ratio);
    area.setHeight((int) area.height() * ratio);
}

bool Selection::isMouseInSelection(QPoint pos)
{
    bool result = false;
    if (!area.isEmpty())
    {
        QRect selectionOuter = area.adjusted(-6, -6, 6, 6);
        result = selectionOuter.contains(pos);
    }
    return result;
}

Selection::Direction Selection::getActiveHandle(QPoint pos)
{
    Direction result = NONE;
    if (!area.isEmpty())
    {
        QRect selectionInner = area.adjusted(3, 3, -3, -3);
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
Selection::Direction Selection::getCardinalDirection(QRect area, QPoint point)
{
    enum Direction result = NONE;

    if (area.contains(point))
    {
        result = INNER;
    }
    else
    {
        if (point.y() < area.top())
        {
            result = static_cast<Direction>(result | N);
        }
        else if (point.y() > area.bottom())
        {
            result = static_cast<Direction>(result | S);
        }

        if (point.x() < area.left())
        {
            result = static_cast<Direction>(result | W);
        }
        else if (point.x() > area.right())
        {
            result = static_cast<Direction>(result | E);
        }

    }
    return result;
}

void Selection::calculateArea(QPoint position)
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
            if (area.left() + dx < 0)
            {
                dx1 = -area.left();
            }
            else if (area.right() + dx > imageArea.width() - 1)
            {
                dx1 = imageArea.width() - area.right();
            }
            else
            {
                dx1 = dx;
            }
            if (area.top() + dy < 0)
            {
                dy1 = -area.top();
            }
            else if (area.bottom() + dy > imageArea.height() - 1)
            {
                dy1 = imageArea.height() - area.bottom();
            }
            else
            {
                dy1 = dy;
            }
        }
        else
        {
            // ... and touches the border when the mouse goes out of the image area.
            Direction direction = getCardinalDirection(imageArea, position);
            if ((direction & N) &&  (area.top() > imageArea.top()))
            {
                dy1 = imageArea.top() - area.top(); 
            }
            else if ((direction & S) && (area.bottom() < imageArea.bottom()))
            {
                dy1 = imageArea.bottom() - area.bottom(); 
            }
            if ((direction & E) && (area.right() < imageArea.right()))
            {
                dx1 = imageArea.right() - area.right(); 
            }
            else if ((direction & W) && (area.left() > imageArea.left()))
            {
                dx1 =  imageArea.left() - area.left(); 
            }
        }
        dx2 = dx1;
        dy2 = dy1;
        area.adjust(dx1, dy1, dx2, dy2);
    }
    else if (creatingArea)
    {
        int dx = position.x() - area.x();
        int dy = position.y() - area.y();
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
            area.setWidth(qAbs(dx));
            area.setHeight(qAbs(dy));
            creatingArea = false;
        }
    }
    else
    {
        // resize the selection (up to the image area)
        switch (activeHandle) {
            case N:
                dy1 = position.y() - area.top();
            break;
            case S:
                dy2 =  position.y() - area.bottom();
            break;
            case W:
                dx1 =  position.x() - area.left();
            break;
            case E:
                dx2 =  position.x() - area.right();
            break;
            case SE:
                dx2 = position.x() - area.bottomRight().x();
                dy2 = position.y() - area.bottomRight().y();
            break;
            case NW:
                dx1 =  position.x() - area.topLeft().x();
                dy1 =  position.y() - area.topLeft().y();
            break;
            case SW:
                dx1 = position.x() - area.bottomLeft().x();
                dy2 = position.y() - area.bottomLeft().y();
            break;
            case NE:
                dx2 =  position.x() - area.topRight().x();
                dy1 =  position.y() - area.topRight().y();
            break;
        }
        // ensure that the area is at least 1x1 and don't allow negative areas
        if (position.x() <= area.left())
        {
            dx2 = 0;
        }
        if (position.y() <= area.top())
        {
            dy2 = 0;
        }
        area.adjust(dx1, dy1, dx2, dy2);
        area.intersect(imageArea);
    }
}
