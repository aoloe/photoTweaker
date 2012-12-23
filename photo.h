#ifndef PHOTO_H
#define PHOTO_H
class Photo : public QWidget
{
    Q_OBJECT
public:
    void open(const QString &filepath);
    void save();
    void saveAs();

    inline QString getFilename() { return filepath.split('/').last(); }
    inline QImage* getImage() { return image; }
    inline void setImage(const QImage &image) { *this->image = image; }


    inline void setEdited(bool flag) { isEdited = flag; }
    inline bool getEdited() { return isEdited; }

    void resize();
    void scale();

    void applyEffect(EffectsEnum effect);

    void restoreCursor();

    inline QUndoStack* getUndoStack() { return undoStack; }

    /**
     * @brief Save all image changes to image copy.
     *
     */
    void saveImageChanges();
    /**
     * @brief Removes selection borders from image and clears all selection varaibles to default.
     *
     */
    void clearSelection(); // TODO: move to the selection tool?
    /**
     * @brief Push current image to undo stack.
     *
     */
    void undo(UndoCommand *command);

private:
    QImage *image;
    QString filepath;
    bool isEdited;
    QPixmap *pixmap;
    QCursor *currentCursor;
    qreal zoomFactor;
    QUndoStack *undoStack;
    QVector<AbstractInstrument*> instrumentsHandlers;
    AbstractInstrument *instrumentHandler;
    QVector<AbstractEffect*> effectsHandlers;
    AbstractEffect *effectHandler;

signals:
void sendCursorPosition(const QPoint&);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
}
#endif // PHOTO_H
