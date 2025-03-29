#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <QWidget>
#include <QLabel>
#include "models.h"

class Displays : public QWidget
{
    Q_OBJECT
public:
    explicit Displays(Model *model, QWidget *parent = nullptr);

private:
    Model *model;
};

#endif // DISPLAYS_H
