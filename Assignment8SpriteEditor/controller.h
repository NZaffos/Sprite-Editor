#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "models.h"

class Controller {
public:
    Controller(Model* model);

    void drawPixel(int x, int y, QColor color);
    void erasePixel(int x, int y);

private:
    Model* model;
};

#endif // CONTROLLERS_H
