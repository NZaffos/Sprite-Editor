#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model* model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);

    // New Displays
    displays = new Displays(model);

    // New Controller
    controller = new Controller(model);
    QString sliderStyle = getSliderStyleSheet();
    ui->redSlider->setStyleSheet(sliderStyle);
    ui->greenSlider->setStyleSheet(sliderStyle);
    ui->blueSlider->setStyleSheet(sliderStyle);
    ui->alphaSlider->setStyleSheet(sliderStyle);

    userColor = QColor(0, 0, 0, 255);

    // Connects
    connect(ui->redSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateSlider);
    connect(ui->greenSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateSlider);
    connect(ui->blueSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateSlider);
    connect(ui->alphaSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateSlider);

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
        colorComponent = "all";
    }

    updateSliderStyle(slider, value, colorComponent);  // Update the style for the corresponding slider
}

void MainWindow::updateSliderStyle(QSlider *slider, int value, const QString &colorComponent) {
    QString color;
    if (colorComponent == "red") {
        color = QString("rgb(%1, 0, 0)").arg(value);
        userColor.setRed(value);
    } else if (colorComponent == "green") {
        color = QString("rgb(0, %1, 0)").arg(value);
        userColor.setGreen(value);
    } else if (colorComponent == "blue") {
        color = QString("rgb(0, 0, %1)").arg(value);
        userColor.setBlue(value);
    } else if (colorComponent == "all") {
        color = QString("rgb(%1, %1, %1)").arg(value);
        userColor.setAlpha(value);
    }

    QString colorStyle = QString("background-color: rgb(%1, %2, %3);")
        .arg(userColor.red())
        .arg(userColor.green())
        .arg(userColor.blue());
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

MainWindow::~MainWindow()
{
    delete ui;
    delete displays;
    delete controller;
}
