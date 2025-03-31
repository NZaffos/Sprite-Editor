#include "palette.h"

Palette::Palette(Ui::MainWindow *ui, Model *model, QColor &userColor, QObject *parent)
    : QObject(parent), ui(ui), userColor(userColor), model(model)
{
    setColorPalette();
    setSliderTextEdits();
    setSliders();
}

void Palette::setColorPalette()
{
    paletteScrollArea = ui->colorPalette;
    paletteContainer = ui->colorPaletteScrollContents;

    paletteLayout = new QGridLayout();
    paletteLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    paletteLayout->setHorizontalSpacing(2);
    paletteLayout->setVerticalSpacing(2);
    paletteLayout->setContentsMargins(6, 10, 0, 0);

    paletteContainer->setLayout(paletteLayout);

    paletteScrollArea->setWidget(paletteContainer);
    paletteScrollArea->setWidgetResizable(true);
}

void Palette::addColorToPalette()
{
    QPushButton *colorButton = new QPushButton();
    colorButton->setFixedSize(25, 25);

    colorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
                                   .arg(userColor.red())
                                   .arg(userColor.green())
                                   .arg(userColor.blue())
                                   .arg(userColor.alpha()));

    unsigned int index = colorButtons.size();

    int row = index / paletteCols;
    int col = index % paletteCols;

    paletteLayout->addWidget(colorButton, row, col);

    colorButtons.append(colorButton);

    model->addToPalette(userColor);

    connect(colorButton,
            &QPushButton::pressed,
            this,
            [=]()
            {
                colorButtonPress(index);
            });
}

void Palette::removeColorFromPalette()
{
    if (!deleteButtonActive)
    {
        return;
    }
    model->removeFromPalette(currentColorButtonIndex);

    paletteLayout->removeWidget(colorButtons.at(currentColorButtonIndex));

    deleteButtonActive = false;
}

void Palette::colorButtonPress(int index)
{
    deleteButtonActive = true;
    currentColorButtonIndex = index;

    QColor color = model->getColorFromPalette(index);
    updateSlidersToColor(color);

    userColor = color;
}

void Palette::setSliders()
{
    QString sliderStyle = getSliderStyleSheet();
    ui->redSlider->setStyleSheet(sliderStyle);
    ui->greenSlider->setStyleSheet(sliderStyle);
    ui->blueSlider->setStyleSheet(sliderStyle);
    ui->alphaSlider->setStyleSheet(sliderStyle);
}

void Palette::updateSlider(int value)
{
    QSlider *slider = qobject_cast<QSlider *>(sender()); // Get the slider that sent the signal

    QString colorComponent;
    if (slider == ui->redSlider)
    {
        colorComponent = "red";
    }
    else if (slider == ui->greenSlider)
    {
        colorComponent = "green";
    }
    else if (slider == ui->blueSlider)
    {
        colorComponent = "blue";
    }
    else if (slider == ui->alphaSlider)
    {
        colorComponent = "alpha";
    }

    updateSliderStyle(slider, value, colorComponent); // Update the style for the corresponding slider
}

void Palette::updateSlidersToColor(QColor color){
    ui->redSlider->setValue(color.red());
    ui->greenSlider->setValue(color.green());
    ui->blueSlider->setValue(color.blue());
    ui->alphaSlider->setValue(color.alpha());

    updateSliderStyle(ui->redSlider, color.red(), "red");
    updateSliderStyle(ui->greenSlider, color.green(), "green");
    updateSliderStyle(ui->blueSlider, color.blue(), "blue");
    updateSliderStyle(ui->alphaSlider, color.alpha(), "alpha");
}

void Palette::updateSliderStyle(QSlider *slider, int value, const QString &colorComponent)
{
    QString color;
    if (colorComponent == "red")
    {
        color = QString("rgb(%1, 0, 0)").arg(value);
        userColor.setRed(value);
        ui->redSliderIO->setText(QString::number(value));
    }
    else if (colorComponent == "green")
    {
        color = QString("rgb(0, %1, 0)").arg(value);
        userColor.setGreen(value);
        ui->greenSliderIO->setText(QString::number(value));
    }
    else if (colorComponent == "blue")
    {
        color = QString("rgb(0, 0, %1)").arg(value);
        userColor.setBlue(value);
        ui->blueSliderIO->setText(QString::number(value));
    }
    else if (colorComponent == "alpha")
    {
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

void Palette::setSliderTextEdits()
{
    updateTextEditStyle(ui->redSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->greenSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->blueSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->alphaSliderIO, "rgb(0, 0, 0)");
}

void Palette::updateTextEditStyle(QLineEdit *textEdit, const QString &Color)
{
    QString style = QString(
                        "QLineEdit {"
                        "    background-color: %1;"
                        "    color: white;" // Brighten text
                        "    border: 1px solid #555;"
                        "    border-radius: 4px;"
                        "    padding: 2px 4px;"
                        "}")
                        .arg(Color);

    textEdit->setStyleSheet(style);
}

void Palette::sliderIOValue()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());
    bool ok;
    int value = lineEdit->text().toInt(&ok);

    if (!ok || value < 0)
    {
        value = 0;
    }
    else if (value > 255)
    {
        value = 255;
    }

    lineEdit->setText(QString::number(value));

    // Update the corresponding slider
    if (lineEdit == ui->redSliderIO)
    {
        ui->redSlider->setValue(value);
    }
    else if (lineEdit == ui->greenSliderIO)
    {
        ui->greenSlider->setValue(value);
    }
    else if (lineEdit == ui->blueSliderIO)
    {
        ui->blueSlider->setValue(value);
    }
    else if (lineEdit == ui->alphaSliderIO)
    {
        ui->alphaSlider->setValue(value);
    }
}

QString Palette::getSliderStyleSheet(QString color)
{
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

Palette::~Palette()
{
    delete paletteScrollArea;
    delete paletteContainer;
    delete paletteLayout;
    qDeleteAll(colorButtons); // Deletes all elements inside the QVector
    colorButtons.clear();
}
