#include "controller.h"

Controller::Controller(Model* model) : model(model) {}

void Controller::drawPixel(int x, int y, QColor color) {
    model->canvas.setPixelColor(x, y, color);
    emit model->canvasUpdated();  // Notify View to update
}

void Controller::erasePixel(int x, int y) {
    model->canvas.setPixelColor(x, y, Qt::white);  // Erase pixel
    emit model->canvasUpdated();
}
