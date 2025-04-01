// Reviewed by Caleb Standfield
/**
 * University of Utah - CS 3505
 * @authors Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen
 * @date 3/28/2025
 * @brief Implementation of the palette class. Handels displaying the palette in the view and comunicating with the model.
 */
#include "palette.h"

Palette::Palette(Ui::MainWindow *ui, Model *model, QColor &userColor, QObject *parent)
    : QObject(parent), ui(ui), userColor(userColor), model(model)
{
    // Inital function calls to set up the palette
    setColorPalette();
    setSliderTextEdits();
    setSliders();
    setButtons();
}

void Palette::setColorPalette()
{
    // Set all of the instance vars to their correspoinding UI elemetns
    paletteScrollArea = ui->colorPalette;
    paletteContainer = ui->colorPaletteScrollContents;

    // Set the QGridLayout to the proper margins
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

    // Sets this new button to look deative
    setButtonStyleSheetDeactive(colorButton, userColor);

    // Calculating the index to put the botton into
    int index = colorButtons.size();
    int row = index / paletteCols;
    int col = index % paletteCols;

    // Add to the layout and model representations
    paletteLayout->addWidget(colorButton, row, col);
    colorButtons.append(colorButton);
    model->addToPalette(userColor);

    // Connect each button
    connect(colorButton,
            &QPushButton::pressed,
            this,
            [=]()
            {
                colorButtonPress(colorButton);
            });
}

void Palette::removeColorFromPalette()
{
    if (!deleteButtonActive)
    {
        return;
    }

    model->removeFromPalette(currentColorButtonIndex);

    // Remove button from layout and delete it
    QPushButton* buttonToRemove = colorButtons.at(currentColorButtonIndex);
    paletteLayout->removeWidget(buttonToRemove);
    buttonToRemove->deleteLater();

    // Remove from button list
    colorButtons.removeAt(currentColorButtonIndex);

    // Need to clear the layout and rebuild it so the buttons "slide down"
    while (QLayoutItem* item = paletteLayout->takeAt(0)) {
        delete item;
    }

    // Rebuild the layout with updated positions
    for (int i = 0; i < colorButtons.size(); i++) {
        int row = i / paletteCols;
        int col = i % paletteCols;
        paletteLayout->addWidget(colorButtons[i], row, col);
    }

    // Update button connections
    for (int i = 0; i < colorButtons.size(); i++) {
        QPushButton* button = colorButtons[i];
        // We first should make sure the button is disconnected
        disconnect(button,
                   &QPushButton::pressed,
                   nullptr,
                   nullptr);
        // Re Add the connections for each button
        connect(button,
                &QPushButton::pressed,
                this,
                [=]() {colorButtonPress(button);
                });
    }

    deleteButtonActive = false;
}

void Palette::colorButtonPress(QPushButton* button)
{
    deleteButtonActive = true;

    // Ensure that if a button was deleted we dont try to access that old buttons index and go out of bounds
    if (currentColorButtonIndex < colorButtons.size())
    {
        QPushButton *oldActiveButton = colorButtons.at(currentColorButtonIndex);
        QColor oldColor = model->getColorFromPalette(currentColorButtonIndex);
        setButtonStyleSheetDeactive(oldActiveButton, oldColor);
    }

    // Get the index of the pressed button
    int index = colorButtons.indexOf(button);

    currentColorButtonIndex = index;

    // Set this new button to look active
    QColor color = model->getColorFromPalette(index);
    setButtonStyleSheetActive(button, color);
    updateSlidersToColor(color);

    userColor = color;
}

void Palette::setButtonStyleSheetActive(QPushButton *button, QColor originalColor)
{
    // Style sheet for an active button
    button->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);"
                                  "border: 1px solid blue;")
                              .arg(originalColor.red())
                              .arg(originalColor.green())
                              .arg(originalColor.blue())
                              .arg(originalColor.alpha()));
}

void Palette::setButtonStyleSheetDeactive(QPushButton *button, QColor originalColor)
{
    // Style sheet for a deactive button
    button->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
                              .arg(originalColor.red())
                              .arg(originalColor.green())
                              .arg(originalColor.blue())
                              .arg(originalColor.alpha()));
}

void Palette::setSliders()
{
    // Set the sliders for start
    QString sliderStyle = getSliderStyleSheet();
    ui->redSlider->setStyleSheet(sliderStyle);
    ui->greenSlider->setStyleSheet(sliderStyle);
    ui->blueSlider->setStyleSheet(sliderStyle);
    ui->alphaSlider->setStyleSheet(sliderStyle);
}

void Palette::updateSlider(int value)
{
    // Get the slider that sent the signal
    QSlider *slider = qobject_cast<QSlider *>(sender());

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

    // Update the style for the corresponding slider
    updateSliderStyle(slider, value, colorComponent);
}

void Palette::updateSlidersToColor(QColor color)
{
    // Update each slider with the corresponding rgba value
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
    // Choose which slider and text edit for the passed in color
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
    // Inital style edits for text edits
    updateTextEditStyle(ui->redSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->greenSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->blueSliderIO, "rgb(0, 0, 0)");
    updateTextEditStyle(ui->alphaSliderIO, "rgb(0, 0, 0)");
}

void Palette::updateTextEditStyle(QLineEdit *textEdit, const QString &Color)
{
    // The style for the text edits
    QString style = QString(
                        "QLineEdit {"
                        "    background-color: %1;"
                        "    color: white;"
                        "    border: 1px solid #555;"
                        "    border-radius: 4px;"
                        "    padding: 2px 4px;"
                        "}")
                        .arg(Color);

    textEdit->setStyleSheet(style);
}

void Palette::sliderIOValue()
{
    // Get the sender
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());
    bool ok;

    // Convert the value into an int then check if it is valid
    int value = lineEdit->text().toInt(&ok);

    if (!ok || value < 0)
    {
        value = 0;
    }
    else if (value > 255)
    {
        // Ensure number wont be above 255
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

void Palette::setButtons()
{
    // Button style
    QString style = QString(
        "    background-color: rgb(0, 0, 0);"
        "    color: white;"
        "    border: 1px solid #555;"
        "    border-radius: 4px;"
        "    padding: 2px 4px;");
    ui->addToPaletteButton->setStyleSheet(style);
    ui->deleteFromColoPalette->setStyleSheet(style);
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
    // Deletes all elements inside the QVector
    qDeleteAll(colorButtons);
    colorButtons.clear();
}
