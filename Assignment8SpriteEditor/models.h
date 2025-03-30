#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QImage>
#include <vector>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class Model : public QObject
{
    Q_OBJECT

public:
    Model(QObject *parent = nullptr);
    ~Model();

    QImage *getImage();

    /**
     * Sets the color at a specific pixel coordinate in the image to the selected color
     * @brief setPixel
     * @param x coordinate of pixel
     * @param y coordinate of pixel
     * @param userColor - new rgb value of the pixel
     */
    void setPixel(int x, int y, QColor userColor);
    void setPixelTracker(int x, int y, QColor userColor);
    void erasePixel(int x, int y);
    void getPixel(int x, int y);
    void setSelectColor(QColor color);

    int getCanvasSize();

    /**
     * Adds a new frame to the animation
     */
    void addFrame();

    /**
     * Duplicates a frame to add a new frame to the animation
     */
    void duplicateFrame();

    /**
     * Removes a frame from the animation
     * @param index the frame to remove
     */
    void removeFrame(unsigned int index);

    /**
     * Selects the specified frame and updates the main canvas
     * @param index the frame to select
     */
    void selectFrame(unsigned int index);

    /**
     * Returns the current frame index
     * @return the current frame index
     */
    unsigned int getCurrentFrameIndex() const;

    /**
     * Helper to return a thumbnail for a given frame
     * @param index the frame to generate the thumbnail for
     * @param width width of the thumbnail
     * @param height height of the thumbnail
     * @return A QPixmap for use as a thumbnail for a frame
     */
    QPixmap getFrameThumbnail(int index, int width, int height) const;

    /**
     * Returns a reference to the list of all frames in the animation
     * @return a list containing all of the frames
     */
    const std::vector<QImage> &getFrames() const;

    void shiftFrameUp();
    void shiftFrameDown();
    void swapFrame(bool swapUp);

    // Palette

    /**
     * @brief addToPalette
     */
    void addToPalette(QColor);

    /**
     * @brief removeFromPalette
     * @param index
     */
    void removeFromPalette(unsigned int index);

    /**
     * @brief getColorFromPalette
     * @param index
     * @return
     */
    QColor getColorFromPalette(unsigned int index);

signals:
    void canvasUpdated();
    void frameModified(unsigned int index);
    int updateAnimationIcon(int index);
    int updateFpsSliderIO(int value);
    bool togglePlayPauseButtonIcon(bool enabled);

public slots:
    void sliderValueChanged(int value);
    void clearCanvas();
    void clearTracker();
    void toggleAnimation();
    void saveProject();
    void loadProject();
    void mirrorFrame();
    void rotateFrame();

private:
    /**
     * Determines the dimensions of our square canvas
     * @brief size
     */
    int size = 32;

    /**
     * The main canvas displayed on the users screen
     * The size is determined by the user input and is then scaled by 10x factor
     * @brief image
     */
    QImage *image;

    std::vector<QImage> frames;
    unsigned int currentFrameIndex = 0;
    int animationFps = 1;
    int animationIndex = 0;
    bool animationPlaying = true;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleasedEvent(QMouseEvent *event);

    QColor selectColor;

    QColor blendAdditive(QColor dest, QColor src);

    QImage *tracker;

    /**
     * @brief palette
     */
    QVector<QColor> palette;

private slots:
    void updateAnimationFrame();
};

#endif // MODELS_H

// #include <QImage>
// #include <QColor>

// class Tools {
// public:
//     static void paintBucket(QImage& image, int x, int y, QColor newColor);
//     static void drawRectangle(QImage& image, QRect rect, QColor color);
//     static void rotateImage(QImage& image, int angle);
// };
