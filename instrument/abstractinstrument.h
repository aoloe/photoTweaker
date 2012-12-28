/*
 * This source file is part of EasyPaint.
 *
 * Copyright (c) 2012 EasyPaint <https://github.com/Gr1N/EasyPaint>
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

#ifndef ABSTRACTINSTRUMENT_H
#define ABSTRACTINSTRUMENT_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>

class Photo;

/**
 * @brief Abstract instrument class.
 *
 */
class AbstractInstrument : public QObject
{
    Q_OBJECT

public:
    explicit AbstractInstrument(QObject *parent = 0);
    virtual ~AbstractInstrument(){}

    virtual void mousePressEvent(QMouseEvent *event, Photo &photo) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event, Photo &photo) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *event, Photo &photo) = 0;
    
signals:
    
protected:
    QPoint startPoint, endPoint; /**< Point for events. */
    QImage imageCopy; /**< Image for storing copy of current image on photo, needed for some instruments. */

    virtual void paint(Photo &photo, bool isSecondaryColor = false, bool additionalFlag = false) = 0;

    /**
     * @brief Creates UndoCommand & pushes it to UndoStack.
     *
     * Base realisation simply save all image to UndoStack
     * @param photo corresponse to image, which is edited
     */
    virtual void makeUndoCommand(Photo &photo);
    
};

#endif // ABSTRACTINSTRUMENT_H
