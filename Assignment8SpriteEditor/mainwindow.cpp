#include "mainwindow.h"
#include "ui_mainwindow.h"

// This is the view class IMPORTANT:
// Delete this comment before submission!!!
MainWindow::MainWindow(Model* model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(model)
{
    ui->setupUi(this);

    // New Displays
    displays = new Displays(model);

    setSliders();
    setSLiderTextEdits();
    setColorPalette();

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
            this,
            &MainWindow::updateSlider);
    connect(ui->redSliderIO,
            &QLineEdit::textChanged,
            this,
            &MainWindow::sliderIOValue);

    connect(ui->greenSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateSlider);
    connect(ui->greenSliderIO,
            &QLineEdit::textChanged,
            this,
            &MainWindow::sliderIOValue);

    connect(ui->blueSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateSlider);
    connect(ui->blueSliderIO,
            &QLineEdit::textChanged,
            this,
            &MainWindow::sliderIOValue);

    connect(ui->alphaSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateSlider);
    connect(ui->alphaSliderIO,
            &QLineEdit::textChanged,
            this,
            &MainWindow::sliderIOValue);

    // Color Palette
    connect(ui->addToPaletteButton,
            &QToolButton::clicked,
            this,
            [this]() {addColorToPalette();});
} // End of constructor

void MainWindow::setColorPalette() {
    paletteScrollArea = ui->colorPalette;  // The QScrollArea
    paletteContainer = ui->colorPaletteScrollContents; // The inner QWidget

    paletteLayout = new QGridLayout();
    paletteLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    paletteLayout->setHorizontalSpacing(2);  // Reduce space between columns
    paletteLayout->setVerticalSpacing(2);    // Reduce space between rows
    paletteLayout->setContentsMargins(6, 10, 0, 0);  // Remove extra margins

    paletteContainer->setLayout(paletteLayout);

    paletteScrollArea->setWidget(paletteContainer);
    paletteScrollArea->setWidgetResizable(true);

}

void MainWindow::addColorToPalette() {
    QPushButton *colorButton = new QPushButton();
    colorButton->setFixedSize(25, 25);
    colorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
        .arg(userColor.red())
        .arg(userColor.green())
        .arg(userColor.blue())
        .arg(userColor.alpha()));

    int row = colorButtons.size() / paletteCols;
    int col = colorButtons.size() % paletteCols;
    paletteLayout->addWidget(colorButton, row, col);

    colorButtons.append(colorButton);
}

void MainWindow::removeColorFromPalette(unsigned int index) {
}

void MainWindow::setColor() {

}

void MainWindow::setSliders(){
    QString sliderStyle = getSliderStyleSheet();
    ui->redSlider->setStyleSheet(sliderStyle);
    ui->greenSlider->setStyleSheet(sliderStyle);
    ui->blueSlider->setStyleSheet(sliderStyle);
    ui->alphaSlider->setStyleSheet(sliderStyle);
}

void MainWindow::updateSlider(int value) {
    QSlider *slider = qobject_cast<QSlider *>(sender());  // Get the slider that sent the signal

    QString colorComponent;
    if (slider == ui->redSlider) {
        colorComponent = "red";
    } else if (slider == ui->greenSlider) {
        colorComponent = "green";
    } else if (slider == ui->blueSlider) {
        colorComponent = "blue";
    } else if (slider == ui->alphaSlider) {
        colorComponent = "alpha";
    }

    updateSliderStyle(slider, value, colorComponent);  // Update the style for the corresponding slider
}

void MainWindow::updateSliderStyle(QSlider *slider, int value, const QString &colorComponent) {
    QString color;
    if (colorComponent == "red") {
        color = QString("rgb(%1, 0, 0)").arg(value);
        userColor.setRed(value);
        ui->redSliderIO->setText(QString::number(value));
    } else if (colorComponent == "green") {
        color = QString("rgb(0, %1, 0)").arg(value);
        userColor.setGreen(value);
        ui->greenSliderIO->setText(QString::number(value));
    } else if (colorComponent == "blue") {
        color = QString("rgb(0, 0, %1)").arg(value);
        userColor.setBlue(value);
        ui->blueSliderIO->setText(QString::number(value));
    } else if (colorComponent == "alpha") {
        color = QString("rgb(%1, %1, %1)").arg(value);
        userColor.setAlpha(value);
        qDebug() << "alpha color is: " << userColor.alpha();
        ui->alphaSliderIO->setText(QString::number(value));
    }

    QString colorStyle = QString("background-color: rgba(%1, %2, %3, %4);")
        .arg(userColor.red())
        .arg(userColor.green())
        .arg(userColor.blue())
        .arg(userColor.alpha());

    ui->currentColor->setStyleSheet(colorStyle);

    QString style = getSliderStyleSheet(color);
    slider->setStyleSheet(style);

}

QString MainWindow::getSliderStyleSheet(QString color) {
    return QString(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #999;"
        "    height: 8px;"
        "    background: #333;"
        "    margin: 2px 0;"
        "    border-radius: 4px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: %1;"
        "    border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: white;"
        "    border: 1px solid black;"
        "    width: 16px;"
        "    margin: -6px 0;"
        "    border-radius: 8px;"
        "}")
        .arg(color);
}

void MainWindow::setSLiderTextEdits() {
    updateTextEditStyle(ui->redSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->greenSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->blueSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->alphaSliderIO, "rgb(0, 0, 0)");
}

void MainWindow::updateTextEditStyle(QLineEdit *textEdit, const QString &Color) {
    QString style = QString(
        "QLineEdit {"
        "    background-color: %1;"
        "    color: white;"  // Brighten text
        "    border: 1px solid #555;"
        "    border-radius: 4px;"
        "    padding: 2px 4px;"
        "}"
        ).arg(Color);

    textEdit->setStyleSheet(style);
}

void MainWindow::sliderIOValue() {
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());
    bool ok;
    int value = lineEdit->text().toInt(&ok);

    if (!ok || value < 0) {
        value = 0;
    } else if (value > 255) {
        value = 255;
    }

    lineEdit->setText(QString::number(value));

    // Update the corresponding slider
    if (lineEdit == ui->redSliderIO) {
        ui->redSlider->setValue(value);
    } else if (lineEdit == ui->greenSliderIO) {
        ui->greenSlider->setValue(value);
    } else if (lineEdit == ui->blueSliderIO) {
        ui->blueSlider->setValue(value);
    } else if (lineEdit == ui->alphaSliderIO) {
        ui->alphaSlider->setValue(value);
    }
}

MainWindow::~MainWindow()
{
    // UI and other classes
    delete ui;
    delete displays;
    delete model;

    // Palette objects
    delete paletteScrollArea;
    delete paletteContainer;
    delete paletteLayout;
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

             //ui->coordinate->setText(QString("(x: %1, y: %2)").arg(x).arg(y));

             //qDebug() << "select pixel at scene Position: " << scenePos.x() << ", " << scenePos.y();

             // Check if in image bounds
             if (x >= 0 && x < model->getImage()->width() &&
                 y >= 0 && y < model->getImage()->height()){

                 drawing = true;
                 currPixel = scenePos;

                 // Update pixel
                 if (event->button() == Qt::LeftButton){
                    //qDebug() << "alpha color is: " << userColor.alpha();
                    model->setPixel(x, y, userColor);

                 } else if (event->button() == Qt::RightButton){
                     // if right mouse button clicked - erease
                     model->setPixel(x, y, QColor("rgba(0,0,0,0)"));
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
