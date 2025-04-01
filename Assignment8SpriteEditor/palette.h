// Reviewed by Caleb Standfield
#ifndef PALETTE_H
#define PALETTE_H

#include "ui_mainwindow.h"
#include "models.h"

#include <QObject>
#include <QtWidgets/qgridlayout.h>

/**
 * University of Utah - CS 3505
 * @authors Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen
 * @date 3/28/2025
 * @brief This class is the header class for the palette. Displays and holds logic for the color palette
 */
class Palette : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Palette A custom constructor for the palette class
     * @param ui A pointer to the MainWindow ui to allow changes to the view
     * @param model A pointer to the model so the palette can share information to be held in the model
     * @param userColor The users current color as a reference
     * @param parent The parent to this class. Default is nullptr
     */
    Palette(Ui::MainWindow *ui, Model *model, QColor &userColor, QObject *parent = nullptr);

    /**
     * @brief The deconstructor for the palette class
     */
    ~Palette();

    /**
     * @brief setSliders Sets the sliders for program start
     */
    void setSliders();

    /**
     * @brief updateSlider Updates the slider value to the passed in int
     * @param value The value to update to
     */
    void updateSlider(int value);

    /**
     * @brief updateSliderStyle Updates the slider style
     * @param slider A pointer to the slider to change
     * @param value The value to update to
     * @param colorComponent Which color slider to update
     */
    void updateSliderStyle(QSlider *slider, int value, const QString &colorComponent);

    /**
     * @brief updateSlidersToColor Updates the slider color
     * @param color Color to update to
     */
    void updateSlidersToColor(QColor color);

    /**
     * @brief getSliderStyleSheet Get the slider style sheet
     * @param color The color to update with
     * @return The slider style sheet as a string
     */
    QString getSliderStyleSheet(QString color = "rgba(0, 0, 0, 0)");

    /**
     * @brief setSliderTextEdits On start set the slider text edits
     */
    void setSliderTextEdits();

    /**
     * @brief updateTextEditStyle Update the text edit style
     * @param lineEdit A pointer to which slider to change
     * @param Color Which color slider to change
     */
    void updateTextEditStyle(QLineEdit *lineEdit, const QString &Color);

    /**
     * @brief sliderIOValue Change the slider IO value
     */
    void sliderIOValue();

    // Color palette

    /**
     * @brief setColorPalette Set the color palette on start
     */
    void setColorPalette();

    /**
     * @brief addColorToPalette Add a button and color to the palette
     */
    void addColorToPalette();

    /**
     * @brief removeColorFromPalette Remove a color from the palette
     */
    void removeColorFromPalette();

    /**
     * @brief setColor Sets the color of button
     */
    void setColor();

    /**
     * @brief setButtons Sets the button style
     */
    void setButtons();

private:
    /**
     * @brief ui A pointer to the main window ui
     */
    Ui::MainWindow *ui;

    /**
     * @brief userColor A reference to the userColor
     */
    QColor &userColor;

    /**
     * @brief model A pointer to the model
     */
    Model *model;

    /**
     * @brief deleteButtonActive A bool to ensure delete only words when true
     */
    bool deleteButtonActive = false;

    /**
     * @brief currentColorButtonIndex The current color button selected
     */
    unsigned int currentColorButtonIndex = 0;

    /**
     * @brief paletteScrollArea A pointer to the ui palette scroll area
     */
    QScrollArea *paletteScrollArea;

    /**
     * @brief paletteContainer A pointer to the ui palette scroll area container
     */
    QWidget *paletteContainer;

    /**
     * @brief paletteLayout A pointer to the ui palette scroll area layout
     */
    QGridLayout *paletteLayout;

    /**
     * @brief colorButtons A vector to hold the color buttons
     */
    QVector<QPushButton *> colorButtons;

    /**
     * @brief paletteCols The set size for the color palette column count
     */
    int paletteCols = 6;

    /**
     * @brief colorButtonPress Called when a color button is pushed to change the style sheet and update userColor
     * @param button The color button pushed
     */
    void colorButtonPress(QPushButton *button);

    /**
     * @brief setButtonStyleSheetActive A style sheet to show a color button is active
     * @param button The button to change its style sheet
     * @param originalColor The origingal color of what this button looked like
     */
    void setButtonStyleSheetActive(QPushButton *button, QColor originalColor);

    /**
     * @brief setButtonStyleSheetDeactive A style sheet to show a color button is deactive
     * @param button The button to change its style sheet
     * @param originalColor The orginal color of what this button looked like
     */
    void setButtonStyleSheetDeactive(QPushButton *button, QColor originalColor);
};

#endif // PALETTE_H
