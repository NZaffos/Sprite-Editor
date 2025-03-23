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
