#ifndef TOOLS_H
#define TOOLS_H

#include <QImage>
#include <QColor>

class Tools {
public:
    static void paintBucket(QImage& image, int x, int y, QColor newColor);
    static void drawRectangle(QImage& image, QRect rect, QColor color);
    static void rotateImage(QImage& image, int angle);
};

#endif // TOOLS_H
