#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Includes for different QT Objects
#include <QMainWindow>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qslider.h>
#include <QGridLayout>

// Includes for files
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
    void addColorToPalette();
    void removeColorFromPalette(unsigned int index);
    void setColor();

    QScrollArea *paletteScrollArea;
    QWidget *paletteContainer;
    QGridLayout *paletteLayout;
    QVector<QPushButton *> colorButtons;
    int paletteCols = 6;

    void addColorToPalette(const QColor &color);

};
#endif // MAINWINDOW_H
