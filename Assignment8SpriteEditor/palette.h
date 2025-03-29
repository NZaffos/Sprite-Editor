#ifndef PALETTE_H
#define PALETTE_H

#include "ui_mainwindow.h"
#include "models.h"

#include <QObject>
#include <QtWidgets/qgridlayout.h>

class Palette : public QObject
{
    Q_OBJECT
public:
    Palette(Ui::MainWindow *ui, Model *model, QColor &userColor, QObject *parent = nullptr);
    ~Palette();

    void setSliders();
    void updateSlider(int value);
    void updateSliderStyle(QSlider *slider, int value, const QString &colorComponent);
    QString getSliderStyleSheet(QString color = "rgba(0, 0, 0, 0)");

    void setSLiderTextEdits();
    void updateTextEditStyle(QLineEdit *lineEdit, const QString &Color);
    void sliderIOValue();

    // Color palette
    void setColorPalette();
    void addColorToPalette();
    void removeColorFromPalette();
    void setColor();

private:
    Ui::MainWindow *ui;
    QColor &userColor;
    Model* model;

    bool deleteButtonActive = false;
    unsigned int currentColorButtonIndex = 0;

    QScrollArea *paletteScrollArea;
    QWidget *paletteContainer;
    QGridLayout *paletteLayout;
    QVector<QPushButton *> colorButtons;
    int paletteCols = 6;

    void colorButtonPress(int index);

};

#endif // PALETTE_H
