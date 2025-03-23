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

    setSliders();

    setSLiderTextEdits();


    userColor = QColor(0, 0, 0, 255);

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

} // End of constructor

void MainWindow::setColorPalette() {

}

void MainWindow::addColorToPalette(QColor color) {

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
        colorComponent = "all";
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
    } else if (colorComponent == "all") {
        color = QString("rgb(%1, %1, %1)").arg(value);
        userColor.setAlpha(value);
        ui->alphaSliderIO->setText(QString::number(value));
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
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());  // Identify the sender

    bool ok;
    int value = lineEdit->text().toInt(&ok);

    if (!ok) {
        value = 0;  // Set to 0 if conversion fails
        lineEdit->setText(QString::number(value));
    }
    if (!ok || value < 0) {
        value = 0;
    }
    if (value > 255) {
        value = 255;
    }

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
    delete ui;
    delete displays;
    delete controller;
}
