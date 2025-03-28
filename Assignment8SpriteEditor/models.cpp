#include "models.h"

Model::Model(QObject *parent) : QObject(parent){
    image = new QImage(sizeX, sizeY, QImage::Format_ARGB32);
    image->fill(QColor(0, 0, 0, 50));
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
    getPixel(x, y);

    //image->setPixelColor(x, y, userColor);

    // if the pixel selected is empty
    if (selectColor.red()   == 0
     && selectColor.green() == 0
     && selectColor.blue()  == 0
     && selectColor.alpha() == 255
    ){
        //qDebug() << "Pixel is empty";
        image->setPixelColor(x, y, userColor);
    } else {
        QColor blendedColor = blendAdditive(userColor, selectColor);
        image->setPixelColor(x, y, blendedColor); // Apply blended color
    }

    //qDebug() << (selectColor == QColor(0, 0, 0, 255));

    // qDebug() << "color is: " << "red: " << userColor.red() << "green: " << userColor.green() << "blue: " << userColor.blue() << "alpha: " << userColor.alpha();
}

void Model::erasePixel(int x, int y){
    image->setPixelColor(x, y, QColor(0, 0, 0, 50));
}

void Model::getPixel(int x, int y){
    selectColor = image->pixelColor(x, y);
    //qDebug() << "Color at coords: " << selectColor.red() << ", " << selectColor.blue() << ", " << selectColor.green() << ", " << selectColor.alpha();
}

int Model::getCanvasX(){
    return sizeX;
}

int Model::getCanvasY(){
    return sizeY;
}

void Model::setSelectColor(QColor color){
    selectColor = color;
}

QColor Model::blendAdditive(QColor src, QColor dest) {
    int redDest = dest.red(), greenDest = dest.green(), blueDest = dest.blue(), alphaDest = dest.alpha();
    int redSrc = src.red(), greenSrc = src.green(), blueSrc = src.blue(), alphaSrc = src.alpha();

    // Compute new alpha
    int alphaOverride = alphaSrc + alphaDest * (255 - alphaSrc) / 255;
    if (alphaOverride == 0) return QColor(0, 0, 0, 0); // Fully transparent

    // Compute new RGB values
    int redOverride = (redSrc * alphaSrc + redDest * alphaDest * (255 - alphaSrc) / 255) / alphaOverride;
    int greenOverride = (greenSrc * alphaSrc + greenDest * alphaDest * (255 - alphaSrc) / 255) / alphaOverride;
    int blueOverride = (blueSrc * alphaSrc + blueDest * alphaDest * (255 - alphaSrc) / 255) / alphaOverride;

    //qDebug() << "Blended color is: " << "red: " << redOverride << "green: " << greenOverride << "blue: " << blueOverride << "alpha: " << alphaOverride;

    return QColor(redOverride, greenOverride, blueOverride, alphaOverride);
}
