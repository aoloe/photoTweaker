#ifndef PHOTO_H
#define PHOTO_H

#include <QWidget>

class QUndoStack;
class QUndoGroup;
class UndoCommand;

class AbstractInstrument;
class AbstractEffect;


class Photo : public QWidget
{
    Q_OBJECT
public:
    // explicit ImageArea(const bool &isOpen = false, const QString &filePath = "", QWidget *parent = 0);
    Photo();
    ~Photo();

    bool open();
    bool open(const QString filePath);
    void save();

    // void update(); // needed for compatibility with EasyPaint's ImageArea
    void updateImageView();
    void clear();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void setFilePath(QString filePath) {this->filePath = filePath;}
    inline QString getFileName() { return filePath.split('/').last(); }
    inline QImage getImage() { return image; }
    inline QImage getImageView() { return imageView; }
    inline QImage* getImagePointer() { return &image; }
    inline void setImage(const QImage image) { this->image = image; }

    const uchar* getData();


    inline void setEdited(bool flag) { isEdited = flag; }
    inline void setEdited() { isEdited = true; }
    inline bool getEdited() { return isEdited; }

    void resize();
    void scale();

    // void applyEffect(EffectsEnum effect);

    void restoreCursor();

    inline QUndoStack* getUndoStack() { return undoStack; }

    /**
     * @brief Save all image changes to image copy.
     * needed for compatibility with EasyPaint's ImageArea in undo code
     *
     */
    void saveImageChanges();

    /**
     * @brief Push current image to undo stack.
     *
     */
    void addUndoInformation();

public:
    static const int INSTRUMENTS_COUNT;
    static const int NONE_INSTRUMENT;
    static const int CURSOR;

public slots:
    /**
     * @brief Removes selection borders from image and clears all selection varaibles to default.
     */
    void clearSelection();

private:
    QImage image; // the original image, as loaded and as it will be saved
    QImage imageView; // the image resized for viewing
    qreal viewScale;
    QString filePath;
    bool isEdited;
    QPixmap pixmap;
    QCursor currentCursor;
    qreal zoomFactor;
    QUndoStack *undoStack;
    QVector<AbstractInstrument*> instrumentsHandlers;
    AbstractInstrument *instrumentHandler;
    QVector<AbstractEffect*> effectsHandlers;
    AbstractEffect *effectHandler;

    inline void emitShow() { emit show(); }

signals:
    void show();
    void sendCursorPosition(const QPoint&);

    void setStatusSize(int width, int height);
    void setStatusMouse(int x, int y);
    void setStatusMouse();
    void setStatusMessage(QString message);
    void setWindowTitle(QString message);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};
#endif // PHOTO_H
