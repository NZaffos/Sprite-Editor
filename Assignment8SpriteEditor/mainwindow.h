/**
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Includes for different QT Objects
#include <QMainWindow>
using std::string;

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qslider.h>
#include <QGridLayout>

// For all the canvas here
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QRgb>

// Includes for files
#include "models.h"
#include "displays.h"

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

    /**
     * an enum class to hold all possible tools available to the user
     * @brief The Tool enum
     */
    enum class Tool{
        BRUSH,
        ERASER
    };
    Q_ENUM(Tool)

public slots:

private:
    Ui::MainWindow *ui;

    /**
     * Instance of the model object
     * @brief model
     */
    Model* model;
    Displays* displays;

    QColor const defaultColor = QColor(255, 255, 255, 255); // Opaque white

    /**
     * Stores the current color active on the users brush
     * @brief userColor
     */
    QColor userColor; // User chosen color

    // Blank Canvas
    QScrollArea *paletteScrollArea;
    QWidget *paletteContainer;
    QGridLayout *paletteLayout;
    QVector<QPushButton *> colorButtons;
    int paletteCols = 6;
    QImage* qimage;
    QPainter *qpainter;
    QGraphicsScene *scene;

    bool drawing = false;
    QPointF currPixel;

    Tool currTool;

private slots:
    // Sliders and Value Labels
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
    void updateView();


    void on_brushBttn_clicked();

    void on_eraseBttn_clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
