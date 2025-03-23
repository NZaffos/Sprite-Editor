#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QImage>
#include <vector>

class Model : public QObject {
    Q_OBJECT
public:
    QImage canvas;
    std::vector<QImage> frames;

    void clearCanvas();
    void addFrame();
    void removeFrame(unsigned int index);

signals:
    void canvasUpdated();
};

#endif // MODELS_H
