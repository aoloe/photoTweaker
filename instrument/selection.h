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

#ifndef SELECTIONINSTRUMENT_H
#define SELECTIONINSTRUMENT_H

#include "abstractinstrument.h"

QT_BEGIN_NAMESPACE
class QUndoStack;
class Photo;
QT_END_NAMESPACE

class SelectionInstrument : public AbstractInstrument
{
    Q_OBJECT

public:
    explicit SelectionInstrument(QObject *parent = 0);

private:
    void mousePressEvent(QMouseEvent *event, Photo &photo);
    void mouseMoveEvent(QMouseEvent *event, Photo &photo);
    void mouseReleaseEvent(QMouseEvent *event, Photo &photo);

    /**
     * @brief Removes selection borders from image and clears all selection varaibles to default.
     *
     * @param photo Photo for applying changes.
     */
    void clearSelection(Photo &photo);
    /**
     * @brief Save all image changes to image copy.
     *
     */
    void saveImageChanges(Photo &);

    /*
    virtual void startSelection(Photo &photo) = 0;
    virtual void startResizing(Photo &photo) = 0;
    virtual void startMoving(Photo &photo) = 0;
    virtual void select(Photo &photo) = 0;
    virtual void resize(Photo &photo) = 0;
    virtual void move(Photo &photo) = 0;
    virtual void completeSelection(Photo &photo) = 0;
    virtual void completeResizing(Photo &photo) = 0;
    virtual void completeMoving(Photo &photo) = 0;
    virtual void clear() = 0;
    */

protected:
    void drawBorder(Photo &photo);
    void updateCursor(QMouseEvent *event, Photo &photo);

    QPoint bottomRightPoint, topLeftPoint, moveDiffPoint;
    bool isPaint, isSelectionExists, isSelectionMoving, isSelectionResizing, isImageSelected;
    int height, width;
    void startSelection(Photo &);
    void startResizing(Photo &photo);
    void startMoving(Photo &photo);
    void select(Photo &);
    void resize(Photo &);
    void move(Photo &);
    void completeSelection(Photo &photo);
    void completeResizing(Photo &photo);
    void completeMoving(Photo &);
    void clear();
    void paint(Photo &photo, bool = false, bool = false);
    
    /*
    virtual void startSelection(Photo &photo) = 0;
    virtual void startResizing(Photo &photo) = 0;
    virtual void startMoving(Photo &photo) = 0;
    virtual void select(Photo &photo) = 0;
    virtual void resize(Photo &photo) = 0;
    virtual void move(Photo &photo) = 0;
    virtual void completeSelection(Photo &photo) = 0;
    virtual void completeResizing(Photo &photo) = 0;
    virtual void completeMoving(Photo &photo) = 0;
    virtual void clear() = 0;
    */

    QImage selectedImage, /**< Copy of selected image. */
           pasteImage; /**< Image to paste */
};

#endif // SELECTIONINSTRUMENT_H
