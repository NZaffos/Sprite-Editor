// Reviewed by Jas Sandhu - u1430476

#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QImage>
#include <vector>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

/**
 * University of Utah – CS 3505
 * @authors Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen
 * @date 03/25/2025
 * @brief This class is responsible for handling the backend logic of the sprite editor such as painting, erasing, creating new canvas sizes and more
 */
class Model : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Model - base constructor for the Model class
     * @param  parent class that creates an instance of the model
     */
    Model(QObject *parent = nullptr);

    /**
     * Deconstructor of the model class
     */
    ~Model();

    /**
     * @brief getImage - returns a reference to the QImage
     * @return QImage object
     */
    QImage *getImage();

    /**
     * @brief createImage - creates a new QImage object with the parameter size
     * @param size - the dimensions of the created QImage
     */
    void createImage(int size);

    /**
     * Sets the color at a specific pixel coordinate in the image to the selected color
     * @brief setPixel
     * @param x coordinate of pixel
     * @param y coordinate of pixel
     * @param userColor - new rgb value of the pixel
     */
    void setPixel(int x, int y, QColor userColor);

    /**
     * @brief setPixelTracker - identifies if the current pixel is colored with the users current color and either ignores or colors the pixel
     * @param x - x cooridnate of the pixel
     * @param y - y cooridnate of the pixel
     * @param userColor - current color selected by user
     */
    void setPixelTracker(int x, int y, QColor userColor);

    /**
     * @brief erasePixel - erases the pixel at the coordinates in the image to be empty
     * @param x - x cooridnate of the pixel
     * @param y - y cooridnate of the pixel
     */
    void erasePixel(int x, int y);

    /**
     * @brief getPixel - Returns the color/RGBA value at the selected pixel coordinate
     * @param x - x cooridnate of the pixel
     * @param y - y cooridnate of the pixel
     */
    void getPixel(int x, int y);

    /**
     * @brief setSelectColor - sets the selectcolor variable to the color of the selected pixel
     * @param color - the color at the selected pixel
     */
    void setSelectColor(QColor color);

    /**
     * @brief getCanvasSize - returns the size/dimensions of the current canvas
     * @return int canvasSize
     */
    int getCanvasSize();

    /**
     * @brief Adds a new frame to the animation
     */
    void addFrame();

    /**
     * @brief Duplicates a frame to add a new frame to the animation
     */
    void duplicateFrame();

    /**
     * @brief Removes a frame from the animation
     * @param index the frame to remove
     */
    void removeFrame(unsigned int index);

    /**
     * @brief Selects the specified frame and updates the main canvas
     * @param index the frame to select
     */
    void selectFrame(unsigned int index);

    /**
     * @brief Returns the current frame index
     * @return the current frame index
     */
    unsigned int getCurrentFrameIndex() const;

    /**
     * @brief Helper to return a thumbnail for a given frame
     * @param index the frame to generate the thumbnail for
     * @param width width of the thumbnail
     * @param height height of the thumbnail
     * @return A QPixmap for use as a thumbnail for a frame
     */
    QPixmap getFrameThumbnail(int index, int width, int height) const;

    /**
     * @brief Returns a reference to the list of all frames in the animation
     * @return a list containing all of the frames
     */
    const std::vector<QImage> &getFrames() const;

    /**
     * @brief shiftFrameUp - shifts the current frame above
     */
    void shiftFrameUp();

    /**
     * @brief shiftFrameDown - shifts the current frame down
     */
    void shiftFrameDown();

    /**
     * @brief swapFrame - swap the position of two frames
     * @param swapUp
     */
    void swapFrame(bool swapUp);

    /**
     * @brief addToPalette - adds the QColor to the palette vector
     * @param QColor - current color by user to be added to palette
     */
    void addToPalette(QColor);

    /**
     * @brief removeFromPalette - removes the selected index from the palette vector
     * @param index - index of selected color in palette
     */
    void removeFromPalette(unsigned int index);

    /**
     * @brief getColorFromPalette - sets the userColor to the color from the selected index
     * @param index - selected index on the palette
     * @return QColor value stored at the index
     */
    QColor getColorFromPalette(unsigned int index);

    /**
     * @brief shapeStart - the starting point of the shape tool
     * @param x - x cooridnate of the pixel
     * @param y - y cooridnate of the pixel
     */
    void shapeStart(int x, int y);

    /**
     * @brief rectangleShape - creates a rectangle in the canvas
     * @param x - x cooridnate of the pixel
     * @param y - y cooridnate of the pixel
     * @param usercolor - users current brush color
     */
    void rectangleShape(int x, int y, QColor usercolor);

    /**
     * @brief ellipseShape - creates the eclipse shape
     * @param x - x cooridnate of the pixel
     * @param y - y cooridnate of the pixel
     * @param usercolor - users current brush color
     */
    void ellipseShape(int x, int y, QColor usercolor);

    /**
     * @brief getShapePreview - returns the QImage that contains a preview of the shape
     * @return QImage object
     */
    QImage *getShapePreview();

    /**
     * @brief mergeShapePreview - draws the shape preview onto the base canvas
     */
    void mergeShapePreview();

    /**
     * @brief paintBucket The paint bucket tool
     * @param x X cordinate
     * @param y Y cordiante
     * @param userColor The users color
     */
    void paintBucket(int x, int y, QColor userColor);

signals:
    /**
     * @brief canvasUpdated - signal sent to the View to update the canvas preview
     */
    void canvasUpdated();

    /**
     * Emitted when a frame's content is modified.
     * @param index The index of the modified frame.
     */
    void frameModified(unsigned int index);

    /**
     * Emitted to update animation preview with current frame.
     * @param index The frame index to display.
     */
    void updateAnimationIcon(int index);

    /**
     * Emitted when FPS slider value changes.
     * @param value The new FPS value.
     */
    void updateFpsSliderIO(int value);

    /**
     * Emitted to toggle play/pause button icon.
     * @param enabled True for play state, false for pause.
     */
    void togglePlayPauseButtonIcon(bool enabled);

    /**
     *  Emitted when a new frame needs to be created.
     */
    void requestNewFrame();

    /**
     * Emitted when a frame needs deletion.
     * @param index The frame index to delete.
     */
    void requestDeleteFrame(unsigned int index);

    /**
     * Emitted to update selected frame in UI.
     * @param index The new selected frame index.
     */
    void requestNewSelectedFrameIndex(unsigned int index);

    /// Emitted after all frames are reloaded from file.
    void framesReloaded();

    /**
     * Emitted when canvas size changes.
     * @param size The new canvas dimension.
     */
    void requestWindowResize(unsigned int size);

public slots:
    /**
     * @brief sliderValueChanged - updates the frames poer second of the animation player
     * @param value - new fps value for animation
     */
    void sliderValueChanged(int value);

    /**
     * @brief clearCanvas - clears the current canvas
     */
    void clearCanvas();

    /**
     * @brief clearNonCanvas - clears all other non canvas objects such as the tracker or shape preview
     */
    void clearNonCanvas();

    /**
     * @brief toggleAnimation - toggles animation on or off
     */
    void toggleAnimation();

    /**
     * @brief saveProject - saves the project to an spp file to a user-chosen destination
     */
    void saveProject();

    /**
     * @brief loadProject - loads an spp file into the project
     */
    void loadProject();

    /**
     * @brief mirrorFrame - mirrors the canvas vertically
     */
    void mirrorFrame();

    /**
     * @brief rotateFrame - rotates the frame by 90 degrees
     */
    void rotateFrame();

private:
    /**
     * Determines the dimensions of our square canvas
     * @brief size
     */
    double size;

    /**
     * The main canvas displayed on the users screen
     * The size is determined by the user input and is then scaled by 10x factor
     * @brief image
     */
    QImage *image;

    /**
     * Stores all animation frames as QImage objects.
     * Each frame represents one image in the animation sequence.
     */
    std::vector<QImage> frames;

    /**
     * The index of the currently selected frame being edited.
     * Ranges from 0 to frames.size()-1.
     */
    unsigned int currentFrameIndex = 0;

    /**
     * Animation playback speed in frames per second.
     * Valid range: 1-60 FPS.
     */
    int animationFps = 1;

    /**
     * Current frame index being displayed during animation preview.
     * Automatically wraps around when reaching the end of frames.
     */
    int animationIndex = 0;

    /**
     * Animation playback state flag.
     * @value true Animation is currently playing
     * @value false Animation is paused
     */
    bool animationPlaying = true;

    /**
     * Timer object that controls animation playback timing.
     * Fires at intervals based on animationFps to advance frames.
     */
    QTimer *animationTimer = nullptr;

    /**
     * @brief mousePressEvent - handles when the mouse buttons have been pressed by user
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseMoveEvent - handles when user moves the mouse while button is clicked
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief mouseReleasedEvent - handles when the user releases the mouse
     * @param event
     */
    void mouseReleasedEvent(QMouseEvent *event);

    /**
     * @brief selectColor - QColor at the selected pixel
     */
    QColor selectColor;

    /**
     * @brief blendAdditive - blends two colors at the same pixel if one color has an alpha value < 225
     * @param dest - color at the selected pixel
     * @param src - current user brush color
     * @return new QColor value that is a blend of both dest and src values
     */
    QColor blendAdditive(QColor dest, QColor src);

    /**
     * @brief tracker - tracks the users current changes to pixel coordinates
     */
    QImage *tracker;

    /**
     * @brief shapePreview - temporray preview of the shape object overlayed above the cuurent canvas image while the user is still creating the shape
     */
    QImage *shapePreview;

    /**
     * @brief shapeStartX - starting x-coordinate of the shape
     */
    int shapeStartX;

    /**
     * @brief shapeStartY - starting y-coordinate of the shape
     */
    int shapeStartY;

    /**
     * @brief palette - the color palette that can be edited by user to store custom colors
     */
    QVector<QColor> palette;

    /**
     * @brief paintBucketRecursive Recursive method for paint bucket
     * @param x X cordinate
     * @param y Y cordinate
     * @param userColor The users color
     * @param colorToReplace The color to replace
     */
    void paintBucketRecursive(int x, int y, QColor userColor, QColor colorToReplace);

private slots:
    void updateAnimationFrame();
};

#endif // MODELS_H
