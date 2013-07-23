#ifndef SELECTION_H
#define SELECTION_H

#include <QtCore/QObject>
#include <QRect>
#include <QPoint>

class Selection : public QObject
{
    Q_OBJECT

public:
    explicit Selection();
    explicit Selection(QRect selection);
    void setSelection(QRect selection);
    bool isMouseInSelection(QPoint pos);
    void detectActiveHandle(QPoint pos);
    /*
    void clearSelection();
    QRect getSelection() {return selection;} // return the selection in image coordinates
    bool isSelection() {return !selection.isEmpty();}
    */

protected:
    QRect area; // selection in image coordinates
    bool selectionCreating;
    enum Direction
    {
        NONE = 0,
        C = 1, // center: it's in the selection area, not the border
        N = 2,
        S = 4,
        E = 8,
        W = 16,
        NE = N | E,
        SE = S | E,
        NW = N | W,
        SW = S | W
    };
    enum Direction activeHandle;
    QPoint mouseLastPosition;
private:
    Direction abcd(QRect selection, QPoint pos);
    Direction getCardinalDirection(QPoint point, QRect area);
};

#endif // SELECTION_H

