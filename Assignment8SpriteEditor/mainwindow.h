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
#include "palette.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model *model, QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * an enum class to hold all possible tools available to the user
     * @brief The Tool enum
     */
    enum class Tool
    {
        BRUSH,
        ERASER
    };
    Q_ENUM(Tool)

public slots:
    void drawAnimationIcon(int index);

private:
    Ui::MainWindow *ui;

    /**
     * Instance of the model object
     * @brief model
     */
    Model *model;
    Displays *displays;
    Palette *palette;

    QColor const defaultColor = QColor(255, 255, 255, 255); // Opaque white

    /**
     * Stores the current color active on the users brush
     * @brief userColor
     */
    QColor userColor; // User chosen color

    // Frame selector elements
    QScrollArea *framesScrollArea;
    QWidget *framesContainer;
    QVBoxLayout *framesLayout;
    QVector<QPushButton *> frameButtons;
    int selectedFrameIndex = 0;
    void updateFrameButtonStyle();

    // Animation elements
    void setAnimationFpsSliderAndWindow();

    // Canvas
    QImage *qimage;
    QPainter *qpainter;
    QGraphicsScene *scene;

    bool drawing = false;
    QPointF currPixel;

    Tool currTool;

private slots:

    // Frame selector
    void setFrameSelector();
    void frameButtonClicked();
    void deleteFrame();
    void addFrameButtonClicked();
    void duplicateFrameButtonClicked();
    void createFrameButton(int index);
    QPushButton *updateFrameButtonIcon(QPushButton *button);
    void shiftFrameUpClicked();
    void shiftFrameDownClicked();
    void updateFpsText(int value);

    // Canvas
    void updateView();

    void on_brushBttn_clicked();
    void on_eraseBttn_clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
