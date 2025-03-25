#include "displays.h"
#include <QPainter>

// Display tells view what to display
// Displays all the frames on the right side for the user to select through
// Displays the Animation on the top right corner
Displays::Displays(Model* model, QWidget* parent) : QWidget(parent), model(model){}
