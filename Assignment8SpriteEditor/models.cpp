#include "models.h"

void Model::clearCanvas() {
    canvas.fill(Qt::white);  // Clear canvas to white
}

void Model::addFrame() {
    frames.push_back(canvas);  // Add current canvas as a new frame
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
