#ifndef SELECTIONINSTRUMENT_H
#define SELECTIONINSTRUMENT_H

#include "abstractinstrument.h"

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
    void clearSelection();
    QRect getSelection() {return selection;} // return the selection in image coordinates

protected:
    void paint(Photo &photo, bool = false, bool = false);
    QPoint origin;
    QRubberBand* rubberBand;
    QRect selection; // selection in image coordinates
    bool isSelecting;
    bool isSelection;
private:
    void mousePressEvent(QMouseEvent *event, Photo &photo);
    void mouseMoveEvent(QMouseEvent *event, Photo &photo);
    void mouseReleaseEvent(QMouseEvent *event, Photo &photo);
    void resizeEvent(QResizeEvent *event, Photo &photo);

};

#endif // SELECTIONINSTRUMENT_H
