#include "models.h"

Model::Model(QObject *parent) : QObject(parent){
    image = new QImage(sizeX, sizeY, QImage::Format_ARGB32);
    image -> fill(Qt::white);

    image->setPixel(10, 10, qRgb(255, 0, 0));   // Red
    image->setPixel(15, 15, qRgb(0, 255, 0));   // Green
    image->setPixel(20, 20, qRgb(0, 0, 255));   // Blue
}

Model::~Model(){
    delete image;
}

QImage* Model::getImage(){
    return image;
}

void Model::clearCanvas() {
    image -> fill(Qt::white);  // Clear canvas to white
}

void Model::addFrame() {
    frames.push_back(*image);  // Add current canvas as a new frame
}

void Model::removeFrame(unsigned int index) {
    if (index >= 0 && index < frames.size()) {
        frames.erase(frames.begin() + index);
    }
}

// #include "tools.h"

// void Tools::paintBucket(QImage& image, int x, int y, QColor newColor) {
//     // Implement paint bucket logic
// }

// void Tools::drawRectangle(QImage& image, QRect rect, QColor color) {
//     // Implement drawing rectangle logic
// }

// void Tools::rotateImage(QImage& image, int angle) {
//     // Implement image rotation logic
// }

void Model::setPixel(int x, int y, QColor userColor){
    qDebug() << "select pixel at: " << x << ", " << y;
    image->setPixel(x, y, userColor.rgb());
}
