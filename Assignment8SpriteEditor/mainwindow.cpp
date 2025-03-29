#include "mainwindow.h"
#include "ui_mainwindow.h"

// This is the view class IMPORTANT:
// Delete this comment before submission!!!
MainWindow::MainWindow(Model* model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(model)
{
    ui->setupUi(this);

    displays = new Displays(model);
    palette = new ColorPalette(ui, userColor);


    userColor = QColor(0, 0, 0, 255);
    scene = new QGraphicsScene(this);
    ui -> graphicsView -> setScene(scene);

    // Add the pixmap to the scene
    scene -> addPixmap(QPixmap::fromImage(*model -> getImage()));

    // Ensure the scenes area matches the pixmap
    scene -> setSceneRect(scene -> itemsBoundingRect());

    // Get user canvas size
    int canvasX = 725/model -> getCanvasX();
    int canvasY = 725/model -> getCanvasY();

    // Configure the view
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, false);
    ui->graphicsView->scale(canvasX, canvasY);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Enable mouse tracking
    ui -> graphicsView -> setMouseTracking(true);
    ui -> graphicsView -> viewport() -> setMouseTracking(true);

    // Connects
    connect(ui->redSlider,
            &QSlider::valueChanged,
            palette,
            &ColorPalette::updateSlider);
    connect(ui->redSliderIO,
            &QLineEdit::textChanged,
            palette,
            &ColorPalette::sliderIOValue);

    connect(ui->greenSlider,
            &QSlider::valueChanged,
            palette,
            &ColorPalette::updateSlider);
    connect(ui->greenSliderIO,
            &QLineEdit::textChanged,
            palette,
            &ColorPalette::sliderIOValue);

    connect(ui->blueSlider,
            &QSlider::valueChanged,
            palette,
            &ColorPalette::updateSlider);
    connect(ui->blueSliderIO,
            &QLineEdit::textChanged,
            palette,
            &ColorPalette::sliderIOValue);

    connect(ui->alphaSlider,
            &QSlider::valueChanged,
            palette,
            &ColorPalette::updateSlider);
    connect(ui->alphaSliderIO,
            &QLineEdit::textChanged,
            palette,
            &ColorPalette::sliderIOValue);

    // Color Palette
    connect(ui->addToPaletteButton,
            &QToolButton::clicked,
            palette,
            [this]() {palette->addColorToPalette();
            });
} // End of constructor


MainWindow::~MainWindow()
{
    // UI and other classes
    delete ui;
    delete displays;
    delete model;

    // Palette objects
    delete palette;
}

// void drawLineTo(const QPoint &endPoint){
//     QPainter painter(&image);
//     painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//     pointer.drawLine(lastPoint, endPoint);
//     modified = true;
//     int rad = (myPenWidth / 2) + 2;

//     update(QRect(lastPoint, endPoint).normal)
// }

void MainWindow::mousePressEvent(QMouseEvent *event){
     if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton){
         // convert mouse click to global coordinates in mainwindow
         QPoint globalPos = event->globalPosition().toPoint();

         // map global coordinates to the graphicView's local coordinates
         QPoint viewPos = ui->graphicsView->mapFromGlobal(globalPos);

         qDebug() << "select pixel at scene Position: " << viewPos.x()/10 << ", " << viewPos.y()/10;

         if (ui->graphicsView->rect().contains(viewPos)){
             // Map to scene coordinates
             QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

             // Get pixel position
             int x = static_cast<int>(scenePos.x());
             int y = static_cast<int>(scenePos.y());

             //qDebug() << "select pixel at scene Position: " << scenePos.x() << ", " << scenePos.y();

             // Check if in image bounds
             if (x >= 0 && x < model->getImage()->width() &&
                 y >= 0 && y < model->getImage()->height()){

                 drawing = true;
                 currPixel = scenePos;

                 // Update pixel
                 if (event->button() == Qt::LeftButton){
                    qDebug() << "alpha color is: " << userColor.alpha();
                    model->setPixel(x, y, userColor);
                 } else if (event->button() == Qt::RightButton){
                     // if right mouse button clicked - erease
                     model->setPixel(x, y, Qt::white);
                 }
                 updateView();
             }
         }
     }
 }

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    qDebug() << "select pixel at scene Position";
    if (drawing && (event->button() == Qt::LeftButton)){
        // map global coordinates to the graphicView's local coordinates
        QPoint viewPos = ui->graphicsView->mapFromGlobal(event->globalPosition().toPoint());

        qDebug() << "select pixel at scene Position: " << viewPos.x()/10 << ", " << viewPos.y()/10;

        if (ui->graphicsView->rect().contains(viewPos)) {
            // Map to scene coordinates
            QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

            // Get pixel position
            int x = static_cast<int>(scenePos.x());
            int y = static_cast<int>(scenePos.y());

            // Check if moving from current pixel position
            if(x != currPixel.x() || y != currPixel.y()){
                // Check if in image bounds
                if (x >= 0 && x < model->getImage()->width() &&
                    y >= 0 && y < model->getImage()->height()) {

                    // Update pixel
                    qDebug() << "alpha color is: " << userColor.alpha();
                    model->setPixel(x, y, userColor.rgba());

                    // update current pixel
                    currPixel = scenePos;
                    updateView();
                }
            }
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
         drawing = false;
    }
}

void MainWindow::updateView(){
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(*model->getImage()));
}
