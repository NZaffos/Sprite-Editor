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


    void updateSlider(int value);
    void updateSliderStyle(QSlider *slider, int value, const QString &colorComponent);
    void updateSlidersToColor(QColor color);
    QString getSliderStyleSheet(QString color = "rgba(0, 0, 0, 0)");

    void setSliderTextEdits();
    void updateTextEditStyle(QLineEdit *lineEdit, const QString &Color);
    void sliderIOValue();

    // Color palette
    void setColorPalette();
    void addColorToPalette();
    void removeColorFromPalette();
    void setColor();
    void setButtons();

private:
    Ui::MainWindow *ui;
    QColor &userColor;
    Model *model;

    bool deleteButtonActive = false;
    unsigned int currentColorButtonIndex = 0;

    QScrollArea *paletteScrollArea;
    QWidget *paletteContainer;
    QGridLayout *paletteLayout;
    QVector<QPushButton *> colorButtons;
    int paletteCols = 6;

    void colorButtonPress(QPushButton* button);

    void setButtonStyleSheetActive(QPushButton *button, QColor originalColor);
    void setButtonStyleSheetDeactive(QPushButton *button, QColor originalColor);
};

#endif // PALETTE_H
