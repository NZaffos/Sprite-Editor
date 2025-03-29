#include "colorpalette.h"

ColorPalette::ColorPalette(Ui::MainWindow* ui, QColor& userColor)
    : ui(ui), userColor(userColor) {
}

void ColorPalette::setColorPalette() {
    paletteScrollArea = ui->colorPalette; // The QScrollArea
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

void ColorPalette::addColorToPalette() {
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

// void ColorPalette::removeColorFromPalette(unsigned int index) {
// }

// void ColorPalette::setColor() {

// }

void ColorPalette::setSliders(){
    QString sliderStyle = getSliderStyleSheet();
    ui->redSlider->setStyleSheet(sliderStyle);
    ui->greenSlider->setStyleSheet(sliderStyle);
    ui->blueSlider->setStyleSheet(sliderStyle);
    ui->alphaSlider->setStyleSheet(sliderStyle);
}

void ColorPalette::updateSlider(int value) {
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

void ColorPalette::updateSliderStyle(QSlider *slider, int value, const QString &colorComponent) {
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

QString ColorPalette::getSliderStyleSheet(QString color) {
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

void ColorPalette::setSLiderTextEdits() {
    updateTextEditStyle(ui->redSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->greenSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->blueSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->alphaSliderIO, "rgb(0, 0, 0)");
}

void ColorPalette::updateTextEditStyle(QLineEdit *textEdit, const QString &Color) {
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

void ColorPalette::sliderIOValue() {
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

ColorPalette::~ColorPalette() {
    delete paletteScrollArea;
    delete paletteContainer;
    delete paletteLayout;
    qDeleteAll(colorButtons); // Deletes all elements inside the QVector
    colorButtons.clear();
}
