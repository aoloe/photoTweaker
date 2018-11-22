#ifndef SELECTION_H
#define SELECTION_H

#include <QtCore/QObject>
#include <QRect>
#include <QPoint>

class Selection : public QObject
{
    Q_OBJECT

public:
    enum Direction
    {
        NONE = 0,
        INNER = 1, // center: it's in the selection area, not the border
        N = 2,
        S = 4,
        E = 8,
        W = 16,
        NE = N | E,
        SE = S | E,
        NW = N | W,
        SW = S | W
    };
    explicit Selection();
    explicit Selection(QRect selection);
    bool isMouseInSelection(QPoint pos);
    void start();
    void stop();
    bool isSelecting();
    Direction getActiveHandle(QPoint pos);
    void detectActiveHandle(QPoint pos);
    void setImageArea(QRect area) {imageArea = area;}
    void setImageAreaView(QRect area) {imageAreaView = area;}
    void calculateAreaView(QPoint position);
    QRect getArea() {return area;}
    QRect getAreaView() {return areaView;}
    void setViewScale(qreal scale) {viewScale = scale;}
    void setOrigin(QPoint origin);
    void setSize(QSize size) {area.setSize(size);}
    void setSize() {setSize(QSize(0, 0));}
    void resize(qreal ratio);
    bool isEmpty() {return areaView.isEmpty();}
    void setMousePosition(QPoint position) {mousePosition = position;}

protected:
    QRect area; // selection in image coordinates
    QRect areaView; // selection in view coordinates
    QRect imageArea; // size of the whole image
    QRect imageAreaView; // size of the whole image view (scaled)
    void resizeArea();
    qreal viewScale;
    QPoint mousePosition;
    enum Direction activeHandle;
    bool creatingArea; // set to true when creating a new area: allows negative rectangles
private:
    Direction getCardinalDirection(QRect area, QPoint point);
};

#endif // SELECTION_H

