#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QImage>
#include <vector>

class Model : public QObject {
    Q_OBJECT
public:
    QImage canvas;
    std::vector<QImage> frames;

    void clearCanvas();
    void addFrame();
    void removeFrame(unsigned int index);

signals:
    void canvasUpdated();
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
