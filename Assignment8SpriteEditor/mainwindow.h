#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qslider.h>
#include "controller.h"
#include "models.h"
#include "displays.h"
#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model* model, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Model* model;
    Displays* displays;
    Controller* controller;

    QColor const defaultColor = QColor(255, 255, 255, 255); // Opaque white
    QColor userColor; // User chosen color

    // Sliders and Value Labels
    void setSliders();
    void updateSlider(int value);
    void updateSliderStyle(QSlider *slider, int value, const QString &colorComponent);
    QString getSliderStyleSheet(QString color = "rgb(0, 0, 0)");

    void setSLiderTextEdits();
    void updateTextEditStyle(QLineEdit *lineEdit, const QString &Color);
    void sliderIOValue();


    // Color palette
    void setColorPalette();
    void addColorToPalette(QColor color);
    void removeColorFromPalette(unsigned int index);
    void setColor();
};
#endif // MAINWINDOW_H
