#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QImage>
#include <vector>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class Model : public QObject {
    Q_OBJECT

public:
    Model(QObject *parent = nullptr);
    ~Model();

    std::vector<QImage> frames;
    void clearCanvas();
    void addFrame();
    void removeFrame(unsigned int index);
    QImage* getImage();

    /**
     * Sets the color at a specific pixel coordinate in the image to the selected color
     * @brief setPixel
     * @param x coordinate of pixel
     * @param y coordinate of pixel
     * @param userColor - new rgb value of the pixel
     */
    void setPixel(int x, int y, QColor userColor);
    void erasePixel(int x, int y);
    void getPixel(int x, int y);
    void setSelectColor(QColor color);

    int getCanvasX();
    int getCanvasY();

signals:
    void canvasUpdated();

private:
    /**
     * Determines the width of our canvas
     * @brief sizeX
     */
    int sizeX = 32;

    /**
     * Determines the height of the canvas
     * @brief sizeY
     */
    int sizeY = 32;

    /**
     * The main canvas displayed on the users screen
     * The size is determined by the user input and is then scaled by 10x factor
     * @brief image
     */
    QImage *image;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleasedEvent(QMouseEvent *event);

    QColor selectColor;

    QColor blendAdditive(QColor dest, QColor src);

private slots:

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
