#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include "ui_mainwindow.h"
#include <QtCore/qobject.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qslider.h>

class ColorPalette : public QObject {
    Q_OBJECT
public:
    explicit ColorPalette(Ui::MainWindow* ui, QColor& userColor);
    virtual ~ColorPalette();

    Ui::MainWindow* ui;

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
    void removeColorFromPalette(unsigned int index);
    void setColor();

private:
    QColor& userColor;
    QScrollArea *paletteScrollArea;
    QWidget *paletteContainer;
    QGridLayout *paletteLayout;
    QVector<QPushButton *> colorButtons;
    int paletteCols = 6;
};

#endif // COLORPALETTE_H
