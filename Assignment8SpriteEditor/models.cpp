#include "models.h"
#include "qpixmap.h"
#include "QTimer"

Model::Model(QObject *parent) : QObject(parent){
    image = new QImage(sizeX, sizeY, QImage::Format_ARGB32);
    image->fill(QColor(0, 0, 0, 50));

    frames.push_back(*image);
    updateAnimationFrame(0);
}

Model::~Model(){
    delete image;
}

QImage* Model::getImage(){
    return image;
}

void Model::clearCanvas() {
    image->fill(Qt::white);  // Clear canvas to white
    emit canvasUpdated();
}

void Model::addFrame() {
    QImage newFrame(sizeX, sizeY, QImage::Format_ARGB32);
    newFrame.fill(QColor(0, 0, 0, 50)); // Adjust to create new frame as default
    frames.push_back(newFrame);
    selectFrame(frames.size() - 1);
}

void Model::duplicateFrame() {
    QImage newFrame = frames[currentFrameIndex].copy();
    frames.push_back(newFrame);
    selectFrame(frames.size() - 1);
}

void Model::removeFrame(unsigned int index) {
    if (frames.size() <= 1 || index >= frames.size())
        return;

    frames.erase(frames.begin() + index);

    if(index > 0)
        currentFrameIndex = index--;
    else
        currentFrameIndex = 0;

    *image = frames[currentFrameIndex];

    emit frameSelected(currentFrameIndex);
    emit canvasUpdated();
}

void Model::selectFrame(unsigned int index) {
    if (index < frames.size()) {
        *image = frames[index];
        currentFrameIndex = index;
        emit frameSelected(index);
        emit canvasUpdated();
    }
}

unsigned int Model::getCurrentFrameIndex() const {
    return currentFrameIndex;
}

QPixmap Model::getFrameThumbnail(int index, int width, int height) const {
    if (index < static_cast<int>(frames.size()) && index >= 0) {
        return QPixmap::fromImage(frames[index]).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return QPixmap();
}

const std::vector<QImage>& Model::getFrames() const {
    return frames;
}

void Model::shiftFrameUp() {
    swapFrame(true);
}

void Model::shiftFrameDown() {
    swapFrame(false);
}

void Model::swapFrame(bool swapUp) {
    int offset;
    if (swapUp)
        offset = -1;
    else
        offset = 1;

    QImage& selectedImage = frames[currentFrameIndex];
    QImage& swappingImage = frames[currentFrameIndex + offset];

    QImage tempImage = selectedImage;
    selectedImage = swappingImage.copy();
    swappingImage = tempImage.copy();

    currentFrameIndex += offset;

    *image = frames[currentFrameIndex].copy();
    emit frameSelected(currentFrameIndex);
    emit canvasUpdated();
}

void Model::sliderValueChanged(int value) {
    animationFps = value;
}

void Model::updateAnimationFrame(int index) {
    emit updateAnimationIcon(index);

    if(index >= static_cast<int>(frames.size()) - 1)
        index = 0;
    else
        index++;
    QTimer::singleShot(1000/animationFps, this, [this, index](){updateAnimationFrame(index);});
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
  
    if(currentFrameIndex < frames.size()){
        frames[currentFrameIndex] = *image;
    }
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
