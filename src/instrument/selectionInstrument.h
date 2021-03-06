#ifndef SELECTIONINSTRUMENT_H
#define SELECTIONINSTRUMENT_H

#include "abstractinstrument.h"
#include "selection.h"

QT_BEGIN_NAMESPACE
class QUndoStack;
class Photo;
QT_END_NAMESPACE

class QRubberBand;

class SelectionInstrument : public AbstractInstrument
{
    Q_OBJECT

public:
    explicit SelectionInstrument(QObject *parent = 0);
    void createSelection(QPoint origin, Photo &photo);
    void destructSelection();
    void clearSelection();
    QRect getSelection() {return selection->getArea();} // return the selection in image coordinates
    bool isSelection() {return (selection != NULL) && (!selection->isEmpty());}

protected:
    void paint(Photo &photo, bool = false, bool = false);
    QPoint origin;
    QRubberBand* rubberBand;
    Selection* selection; // the selection entity
    bool selectionCreating;
    /*
    // TODO: find how avoid the static cast when doing the | in getCardinalDirection()
    inline SelectionDirection operator|(SelectionDirection a, SelectionDirection b)
        {return static_cast<SelectionDirection>(static_cast<int>(a) | static_cast<int>(b));}
    */
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
    enum Direction mouseOnSelection; // TODO: rename it!
    enum Direction clickOnSelection;
    QPoint mouseLastPosition;
private:
    void mousePressEvent(QMouseEvent *event, Photo &photo);
    void mouseMoveEvent(QMouseEvent *event, Photo &photo);
    void mouseReleaseEvent(QMouseEvent *event, Photo &photo);
    void resizeEvent(QResizeEvent *event, Photo &photo);
    void paintEvent(QPaintEvent *event, Photo &photo);
    Direction getCardinalDirection(QPoint point, QRect area);
    void updateCursor(QMouseEvent *event, Photo &photo);

};

#endif // SELECTIONINSTRUMENT_H
