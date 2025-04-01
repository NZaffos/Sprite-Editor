/**
 * @reviewedby Nash Hawkins
 * @file mainwindow.h
 * @brief Defines the MainWindow class, which provides the main interface for the application.
 *
 * @author Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen
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

// For creating new file
#include <QInputDialog>
#include <QMessageBox>

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

/**
 * @class MainWindow
 * @brief The main application window handling UI and interactions.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow.
     * @param model Pointer to the application model.
     * @param parent Parent widget (default is nullptr).
     */
    MainWindow(Model *model, QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow.
     */
    ~MainWindow();

    /**
     * @enum Tool
     * @brief Enum class representing available tools in the application.
     */
    enum class Tool
    {
        BRUSH, // Brush tool for drawing.
        ERASER, //Eraser tool for removing parts of the drawing.
        PAINT, //Paint bucket tool for filling areas.
        EYE, //Eyedropper tool for color selection.
        RECTANGLE, //Rectangle tool for drawing rectangles.
        ELLIPSE //Ellipse tool for drawing ellipses.
    };
    Q_ENUM(Tool)

private:
    Ui::MainWindow *ui; //UI elements of the MainWindow.
    Model *model; //Pointer to the application data model.

    Displays *displays; //Displays component handling UI elements.
    Palette *palette; //Color palette component.
    QPixmap background; //Background image.

    /**
     * @brief Stores the current active brush color.
     */
    QColor userColor;

    /**
     * @brief Stores the currently selected drawing tool.
     */
    Tool currTool;

    // Canvas
    QImage *qimage; // Image buffer for drawing.
    QPainter *qpainter; // Painter for rendering graphics.
    QGraphicsScene *scene; // Scene for handling graphics items.
    bool drawing = false; // Flag indicating if the user is drawing.
    QPointF currPixel; // Current pixel position for drawing.

    /**
     * @brief Initializes buttons with appropriate styles.
     */
    void initializeButtons();

    /**
     * @brief Creates a new canvas for drawing.
     */
    void createCanvas();


private slots:
    /**
     * @brief Updates the view with the latest changes.
     */
    void updateView();

    /**
     * @brief Creates a background for the canvas.
     */
    void createBg();

    // Tool button click handlers
    void on_brushBttn_clicked();
    void on_eraseBttn_clicked();
    void on_eyeBttn_clicked();
    void on_rectangleBttn_clicked();
    void on_ellipseBttn_clicked();

    /**
     * @brief Handles the event when the new file button is clicked.
     */
    void on_newButton_clicked();

protected:
    /**
     * @brief Handles mouse press events on the canvas.
     * @param event Mouse event details.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Filters events for objects in the UI.
     * @param obj The object receiving the event.
     * @param event The event occurring.
     * @return true if the event is handled, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
