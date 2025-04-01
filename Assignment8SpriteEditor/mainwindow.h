// Reviewed By Nash Hawkins
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

// For Cursor
#include <QCursor>
#include <QStyle>

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
 * @file mainwindow.h
 * @brief Defines the MainWindow class, which provides the main interface for the application.
 *
 * @author Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen
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
        // Brush tool for drawing.
        BRUSH,
        // Eraser tool for removing parts of the drawing.
        ERASER,
        // Paint bucket tool for filling areas.
        PAINT,
        // Eyedropper tool for color selection.
        EYE,
        // Rectangle tool for drawing rectangles.
        RECTANGLE,
        // Ellipse tool for drawing ellipses.
        ELLIPSE
    };
    Q_ENUM(Tool)

private:
    // UI elements of the MainWindow.
    Ui::MainWindow *ui;
    // Pointer to the application data model.
    Model *model;

    // Displays component handling UI elements.
    Displays *displays;
    // Color palette component.
    Palette *palette;
    // Background image.
    QPixmap background;

    /**
     * @brief Stores the current active brush color.
     */
    QColor userColor;

    /**
     * @brief Stores the currently selected drawing tool.
     */
    Tool currTool;

    // Canvas
    // Image buffer for drawing.
    QImage *qimage;
    // Painter for rendering graphics.
    QPainter *qpainter;
    // Scene for handling graphics items.
    QGraphicsScene *scene;
    // Flag indicating if the user is drawing.
    bool drawing = false;
    // Current pixel position for drawing.
    QPointF currPixel;

    /**
     * @brief Initializes buttons with appropriate styles.
     */
    void initializeButtons();

    /**
     * @brief Creates a new canvas for drawing.
     */
    void createCanvas();

    /**
     * Switches the border fo the tool selector to the new tool
     * @param newTool The new tool to be selected
     */
    void updateToolBorderSelection(Tool newTool);

    /**
     * @brief Sets the appropriate cursor icon
     */
    void setCursorIcon();

private slots:
    // Canvas
    /**
     * @brief Updates the view with the latest changes.
     */
    void updateView();

    /**
     * @brief Creates a background for the canvas.
     */
    void createBg();

    // Tool button click handlers

    /**
     * @brief on_brushBttn_clicked Select brush
     */
    void on_brushBttn_clicked();

    /**
     * @brief on_brushBttn_clicked Select erase
     */
    void on_eraseBttn_clicked();

    /**
     * @brief on_brushBttn_clicked Select eye dropper
     */
    void on_eyeBttn_clicked();

    /**
     * @brief on_brushBttn_clicked Select rectangle
     */
    void on_rectangleBttn_clicked();

    /**
     * @brief on_brushBttn_clicked Select ellipse
     */
    void on_ellipseBttn_clicked();

    /**
     * @brief on_brushBttn_clicked Select paintbucket
     */
    void on_paintBttn_clicked();

    /**
     * @brief on_brushBttn_clicked Select new button
     */
    void on_newButton_clicked();

    /**
     * @brief Handles the event when the new file button is clicked.
     */
    void resizeWindow(unsigned int size);

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
