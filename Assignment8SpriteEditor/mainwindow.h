#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Includes for different QT Objects
#include <QMainWindow>

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qslider.h>
#include <QGridLayout>

// For all the canvas here
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QRgb>

// Includes for files
#include "models.h"
#include "displays.h"
#include "palette.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model* model, QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private:
    Ui::MainWindow* ui;

    Model* model;
    Displays* displays;
    Palette* palette;

    QColor const defaultColor = QColor(255, 255, 255, 255); // Opaque white
    QColor userColor; // User chosen color    

    // Canvas
    QImage* qimage;
    QPainter *qpainter;
    QGraphicsScene *scene;

    bool drawing = false;
    QPointF currPixel;

private slots:

    // Canvas
    void updateView();

private slots:


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
#endif // MAINWINDOW_H
