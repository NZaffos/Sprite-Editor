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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model *model, QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * an enum class to hold all possible tools available to the user
     */
    enum class Tool
    {
        BRUSH,
        ERASER,
        PAINT,
        EYE,
        RECTANGLE,
        ELLIPSE
    };
    Q_ENUM(Tool)

private:
    Ui::MainWindow *ui;
    Model *model;

    Displays *displays;
    Palette *palette;

    QColor const defaultColor = QColor(255, 255, 255, 255); // Opaque white

    /**
     * Stores the current color active on the users brush
     */
    QColor userColor;
    Tool currTool;

    // Canvas
    QImage *qimage;
    QPainter *qpainter;
    QGraphicsScene *scene;
    bool drawing = false;
    QPointF currPixel;

    /**
     * Initialize the buttons with the proper style
     */
    void initializeButtons();

    /**
     * Switches the border fo the tool selector to the new tool
     * @param newTool The new tool to be selected
     */
    void updateToolBorderSelection(Tool newTool);

private slots:
    // Canvas
    void updateView();

    void on_brushBttn_clicked();
    void on_eraseBttn_clicked();
    void on_eyeBttn_clicked();

    // New Button
    void on_newButton_clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
