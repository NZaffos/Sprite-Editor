#include "models.h"

Model::Model(QObject *parent) : QObject(parent){
    image = new QImage(sizeX, sizeY, QImage::Format_ARGB32);
    image->fill("rgba(0, 0, 0, 0");
}

Model::~Model(){
    delete image;
}

QImage* Model::getImage(){
    return image;
}

void Model::clearCanvas() {
    image->fill(Qt::white);  // Clear canvas to white
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
    image->setPixel(x, y, userColor.rgba());
    qDebug() << "color is: " << "red: " << userColor.red() << "green: " << userColor.green() << "blue: " << userColor.blue() << "alpha: " << userColor.alpha();
}

int Model::getCanvasX(){
    return sizeX;
}

int Model::getCanvasY(){
    return sizeY;
}

// USE FOR ADDIVTIVE COLOR

// void Model::setPixel(int x, int y, QColor newColor) {
//     QColor existingColor = image->pixelColor(x, y);  // Get the current pixel color
//     QColor blendedColor = blendAdditive(existingColor, newColor);
//     image->setPixelColor(x, y, blendedColor); // Apply blended color
// }

// QColor blendAdditive(QColor dest, QColor src) {
//     int r_d = dest.red(), g_d = dest.green(), b_d = dest.blue(), a_d = dest.alpha();
//     int r_s = src.red(), g_s = src.green(), b_s = src.blue(), a_s = src.alpha();

//     // Compute new alpha
//     int a_o = a_s + a_d * (255 - a_s) / 255;
//     if (a_o == 0) return QColor(0, 0, 0, 0); // Fully transparent

//     // Compute new RGB values
//     int r_o = (r_s * a_s + r_d * a_d * (255 - a_s) / 255) / a_o;
//     int g_o = (g_s * a_s + g_d * a_d * (255 - a_s) / 255) / a_o;
//     int b_o = (b_s * a_s + b_d * a_d * (255 - a_s) / 255) / a_o;

//     return QColor(r_o, g_o, b_o, a_o);
// }
