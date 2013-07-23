#include <QDebug>
#include "selection.h"

Selection::Selection()
{
    area = QRect();
    selectionCreating = false;
    mouseLastPosition = QPoint(0,0);
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

void Selection::setSelection(QRect area)
{
    this->area = area;
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

void Selection::detectActiveHandle(QPoint pos)
{
    activeHandle = NONE;
    if (!area.isEmpty())
    {
        QRect selectionInner = area.adjusted(3, 3, -3, -3);
        activeHandle = getCardinalDirection(pos, selectionInner);
    }
}

/**
 * retuns the relative position of qpoint towards the area as a cardinal point
 * (N, S, W, E, NE, SE, NW, SW)
 */
Selection::Direction Selection::getCardinalDirection(QPoint point, QRect area)
{
    enum Direction result = NONE;

    if (area.contains(point))
    {
        result = C;
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
