// Reviewed by Ethan Perkins
/**
 * University of Utah - CS 3505
 * @authors Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen
 * @date 3/28/2025
 * @brief Implementation of the View portion of MVC. Displays the canvas and holds elements for the Display and Palette.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>

MainWindow::MainWindow(Model *model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(model)
{
    ui->setupUi(this);

    // New Displays
    displays = new Displays(ui, model);
    palette = new Palette(ui, model, userColor);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    createCanvas();

    // Set palette sliders
    palette->updateSlidersToColor(QColor(0, 0, 0, 255));
    currTool = Tool::BRUSH;
    initializeButtons();

    // Enable mouse tracking
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->setMouseTracking(true);

    // Background
    createBg();

    updateView();

    // Canvas updating
    connect(model, &Model::canvasUpdated,
            this, &MainWindow::updateView);

    // Color picker
    connect(ui->redSlider,
            &QSlider::valueChanged,
            palette,
            &Palette::updateSlider);
    connect(ui->redSliderIO,
            &QLineEdit::textChanged,
            palette,
            &Palette::sliderIOValue);

    connect(ui->greenSlider,
            &QSlider::valueChanged,
            palette,
            &Palette::updateSlider);
    connect(ui->greenSliderIO,
            &QLineEdit::textChanged,
            palette,
            &Palette::sliderIOValue);

    connect(ui->blueSlider,
            &QSlider::valueChanged,
            palette,
            &Palette::updateSlider);
    connect(ui->blueSliderIO,
            &QLineEdit::textChanged,
            palette,
            &Palette::sliderIOValue);

    connect(ui->alphaSlider,
            &QSlider::valueChanged,
            palette,
            &Palette::updateSlider);
    connect(ui->alphaSliderIO,
            &QLineEdit::textChanged,
            palette,
            &Palette::sliderIOValue);

    // Color Palette
    connect(ui->addToPaletteButton,
            &QToolButton::clicked,
            palette,
            [this]()
            { palette->addColorToPalette(); });
    connect(ui->deleteFromColoPalette,
            &QToolButton::clicked,
            palette,
            &Palette::removeColorFromPalette);

    // New/Save/Load connections
    connect(ui->saveButton,
            &QPushButton::clicked,
            model,
            &Model::saveProject);
    connect(ui->loadButton,
            &QPushButton::clicked,
            model,
            &Model::loadProject);
    connect(model,
            &Model::requestNewFrame,
            displays,
            &Displays::addFrameButtonClicked);
    connect(model,
            &Model::requestNewSelectedFrameIndex,
            displays,
            &Displays::setSelectedFrameIndex);
    connect(model,
            &Model::requestDeleteFrame,
            displays,
            &Displays::setSelectedFrameIndex);
    connect(model,
            &Model::framesReloaded,
            displays,
            &Displays::rebuildFrameButtonsFromModel);
    connect(model,
            &Model::requestWindowResize,
            this,
            &MainWindow::resizeWindow);

    // Mirror/Rotate connections
    connect(ui->mirrorBttn,
            &QPushButton::clicked,
            model,
            &Model::mirrorFrame);
    connect(ui->rotateBttn,
            &QPushButton::clicked,
            model,
            &Model::rotateFrame);
} // End of constructor

void MainWindow::initializeButtons()
{
    QString style = QString(
        "    background-color: rgb(0, 0, 0);"
        "    color: white;" // Brighten text
        "    border: 1px solid #555;"
        "    border-radius: 4px;"
        "    padding: 2px 4px;");
    ui->toolBox->setStyleSheet(QString("QToolButton {") + style + QString("}"));

    ui->saveButton->setStyleSheet(style);
    ui->newButton->setStyleSheet(style);
    ui->loadButton->setStyleSheet(style);
    ui->mirrorBttn->setStyleSheet(style);
    ui->rotateBttn->setStyleSheet(style);

    updateToolBorderSelection(currTool);
}

MainWindow::~MainWindow()
{
    // UI and other classes
    delete ui;
    delete displays;
    delete model;

    // Palette objects
    delete palette;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // convert mouse click to global coordinates in mainwindow
        QPoint globalPos = event->globalPosition().toPoint();

        // map global coordinates to the graphicView's local coordinates
        QPoint viewPos = ui->graphicsView->mapFromGlobal(globalPos);

        if (ui->graphicsView->rect().contains(viewPos))
        {
            // Map to scene coordinates
            QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

            // Get pixel position
            int x = static_cast<int>(scenePos.x());
            int y = static_cast<int>(scenePos.y());

            // Check if in image bounds
            if (x >= 0 && x < model->getImage()->width() &&
                y >= 0 && y < model->getImage()->height())
            {
                drawing = true;
                currPixel = scenePos;

                // Handle tool-specific actions
                switch (currTool)
                {
                case Tool::BRUSH:
                    model->setPixelTracker(x, y, userColor);
                    break;
                case Tool::ERASER:
                    model->erasePixel(x, y);
                    break;
                case Tool::EYE:
                    palette->updateSlidersToColor(model->getImage()->pixelColor(x, y));
                    currTool = Tool::BRUSH;
                    setCursorIcon();
                    updateToolBorderSelection(Tool::BRUSH);
                    break;
                case Tool::RECTANGLE:
                case Tool::ELLIPSE:
                    model->shapeStart(x, y);
                    break;
                case Tool::PAINT:
                    model->paintBucket(x, y, userColor);
                default:
                    break;
                }
                updateView();
            }
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->graphicsView->viewport())
    {
        if (event->type() == QEvent::Enter)
        {
            setCursorIcon();
        }

        // When the mouse leaves the canvas, revert to the default cursor.
        else if (event->type() == QEvent::Leave)
        {
            ui->graphicsView->viewport()->unsetCursor();
        }

        // Existing handling for MouseMove events.
        else if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            QPoint localPos = mouseEvent->pos();
            QPoint globalPos = ui->graphicsView->viewport()->mapToGlobal(localPos);
            QPoint viewPos = ui->graphicsView->mapFromGlobal(globalPos);
            QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

            // Get pixel position
            int x = static_cast<int>(scenePos.x());
            int y = static_cast<int>(scenePos.y());
            ui->coordinate->setText(QString("(x: %1, y: %2)").arg(x).arg(y));

            if (mouseEvent->buttons() & Qt::LeftButton)
            {
                // Check if moving from current pixel position & if in image bounds
                if ((x != currPixel.x() || y != currPixel.y()) && x >= 0 && x < model->getImage()->width() &&
                        y >= 0 && y < model->getImage()->height())
                {
                    // Handle tool-specific actions
                    switch (currTool)
                    {
                    case Tool::BRUSH:
                        model->setPixelTracker(x, y, userColor);
                        break;
                    case Tool::ERASER:
                        model->erasePixel(x, y);
                        break;
                    case Tool::RECTANGLE:
                        model->rectangleShape(x, y, userColor);
                        break;
                    case Tool::ELLIPSE:
                        model->ellipseShape(x, y, userColor);
                        break;
                    default:
                        break;
                    }
                    // update current pixel
                    currPixel = scenePos;
                    updateView();
                }
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                QPoint localPos = mouseEvent->pos();
                QPoint globalPos = ui->graphicsView->viewport()->mapToGlobal(localPos);
                QPoint viewPos = ui->graphicsView->mapFromGlobal(globalPos);
                QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

                // Check if in bounds, then handle tool-specific actions
                int x = static_cast<int>(scenePos.x());
                int y = static_cast<int>(scenePos.y());
                if (x >= 0 && x < model->getImage()->width() &&
                    y >= 0 && y < model->getImage()->height() &&
                    (currTool == Tool::RECTANGLE || currTool == Tool::ELLIPSE))
                {
                    model->mergeShapePreview();
                }
                model->clearNonCanvas();
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::updateView()
{
    scene->clear();
    scene->addPixmap(background);
    scene->addPixmap(QPixmap::fromImage(*model->getImage()));
    scene->addPixmap(QPixmap::fromImage(*model->getShapePreview()));
}

void MainWindow::on_brushBttn_clicked()
{
    updateToolBorderSelection(Tool::BRUSH);
    currTool = Tool::BRUSH;
    model->setSelectColor(QColor(0, 0, 0, 255)); // Black (to trigger blending)
}

void MainWindow::on_eraseBttn_clicked()
{
    updateToolBorderSelection(Tool::ERASER);
    currTool = Tool::ERASER;
    model->setSelectColor(QColor(255, 255, 255, 255)); // Non-black (to bypass blending)
}

void MainWindow::on_eyeBttn_clicked()
{
    updateToolBorderSelection(Tool::EYE);
    currTool = Tool::EYE;
}

void MainWindow::on_rectangleBttn_clicked()
{
    updateToolBorderSelection(Tool::RECTANGLE);
    currTool = Tool::RECTANGLE;
}

void MainWindow::on_ellipseBttn_clicked()
{
    updateToolBorderSelection(Tool::ELLIPSE);
    currTool = Tool::ELLIPSE;
}

void MainWindow::on_paintBttn_clicked()
{
    updateToolBorderSelection(Tool::PAINT);
    currTool = Tool::PAINT;
}

void MainWindow::on_newButton_clicked()
{
    bool inputReceived;
    // Ask for the number of rows
    QString rowText = QInputDialog::getText(this,
                                            tr("Canvas Size"),
                                            tr("Please enter the size of the canvas:\n(Max size: 250)\nSizes over 250 will be capped."),
                                            QLineEdit::Normal,
                                            QString(),
                                            &inputReceived);
    if (!inputReceived || rowText.isEmpty())
    {
        return; // User cancelled or left empty
    }

    bool conversionOK;
    int rows = std::min(rowText.toInt(&conversionOK), 250); // Max canvas size is 250
    if (!conversionOK)
    {
        QMessageBox::warning(this, tr("Invalid Input"), tr("Please enter a valid integer"));
        return;
    }

    resizeWindow(rows);
}

void MainWindow::resizeWindow(unsigned int size)
{
    // Send update to Model, Display
    model->createImage(size);
    displays->rebuildFrameButtonsFromModel();

    // Update Canvas
    createCanvas();
    createBg();
    updateView();
}

void MainWindow::createCanvas()
{
    // Add the pixmap to the scene
    scene->addPixmap(QPixmap::fromImage(*model->getImage()));

    // Ensure the scenes area matches the pixmap
    scene->setSceneRect(scene->itemsBoundingRect());

    // Get user canvas size
    double canvasSize = 725.0 / model->getCanvasSize();

    // Configure the view
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, false);
    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(canvasSize, canvasSize);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->viewport()->installEventFilter(this);
}

void MainWindow::createBg()
{
    int width = model->getCanvasSize();

    QImage bgImage = QImage(width, width, QImage::Format_ARGB32);
    bgImage.fill(QColor(150, 150, 150, 100));

    // Scale checkerboard pattern with canvas size
    int checkerboardWidth = std::max(2, int(qNextPowerOfTwo(width)) / 8);

    QPainter painter(&bgImage);
    QPen pen;
    pen.setColor(QColor(150, 150, 150, 255));
    painter.setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(150, 150, 150, 255));
    painter.setBrush(brush);

    // Paint light squares
    int boxCount = width / checkerboardWidth + 1;
    for (int i = 0; i < boxCount; i++)
    {
        for (int j = 0; j < boxCount; j++)
        {
            if ((i + j) % 2 == 1)
            {
                continue;
            }
            painter.drawRect(i * checkerboardWidth, j * checkerboardWidth, checkerboardWidth - 1, checkerboardWidth - 1);
        }
    }
    background = QPixmap::fromImage(bgImage);
}

void MainWindow::updateToolBorderSelection(Tool newTool)
{
    // Deselect Tools
    ui->brushBttn->setStyleSheet("");
    ui->eraseBttn->setStyleSheet("");
    ui->eyeBttn->setStyleSheet("");
    ui->paintBttn->setStyleSheet("");
    ui->rectangleBttn->setStyleSheet("");
    ui->ellipseBttn->setStyleSheet("");

    // Set border of selected tool
    switch (newTool)
    {
    case Tool::BRUSH:
        ui->brushBttn->setStyleSheet("border: 2px solid blue");
        break;
    case Tool::ERASER:
        ui->eraseBttn->setStyleSheet("border: 2px solid blue");
        break;
    case Tool::ELLIPSE:
        ui->ellipseBttn->setStyleSheet("border: 2px solid blue");
        break;
    case Tool::EYE:
        ui->eyeBttn->setStyleSheet("border: 2px solid blue");
        break;
    case Tool::PAINT:
        ui->paintBttn->setStyleSheet("border: 2px solid blue");
        break;
    case Tool::RECTANGLE:
        ui->rectangleBttn->setStyleSheet("border: 2px solid blue");
        break;
    }
}

void MainWindow::setCursorIcon(){
    QPixmap toolPixmap;
    // Select the appropriate icon based on the current tool.
    if (currTool == Tool::BRUSH)
    {
        toolPixmap.load(":/icons/icons/brush.png");
    }
    else if (currTool == Tool::ERASER)
    {
        toolPixmap.load(":/icons/icons/eraser.png");
    }
    else if (currTool == Tool::EYE)
    {
        toolPixmap.load(":/icons/icons/eyedropper.png");
    }
    else if (currTool == Tool::PAINT)
    {
        toolPixmap.load(":/icons/icons/bucket.png");
    }
    else
    {
        // Fallback icon if needed.
        toolPixmap.load(":/icons/icons/brush.png");
    }

    // Scale the pixmap to 32x32 while keeping the aspect ratio.
    QPixmap scaledPixmap = toolPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Set the hotspot to the center of the pixmap.
    QCursor toolCursor(scaledPixmap, 0, scaledPixmap.height() / 2);
    ui->graphicsView->viewport()->setCursor(toolCursor);
}
