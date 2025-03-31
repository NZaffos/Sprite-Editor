#include "mainwindow.h"
#include "ui_mainwindow.h"

// This is the view class IMPORTANT:
// Delete this comment before submission!!!
MainWindow::MainWindow(Model *model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(model)
{
    ui->setupUi(this);

    // New Displays
    displays = new Displays(ui, model);
    palette = new Palette(ui, model, userColor);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Add the pixmap to the scene
    scene->addPixmap(QPixmap::fromImage(*model->getImage()));

    // Ensure the scenes area matches the pixmap
    scene->setSceneRect(scene->itemsBoundingRect());

    // Get user canvas size
    int canvasSize = 725 / model->getCanvasSize();

    // Configure the view
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, false);
    ui->graphicsView->scale(canvasSize, canvasSize);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->viewport()->installEventFilter(this);

    // Set palette sliders
    palette->updateSlidersToColor(QColor(0, 0, 0, 255));
    initializeButtons();
    currTool = Tool::BRUSH;

    // Enable mouse tracking
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->setMouseTracking(true);

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

void MainWindow::initializeButtons() {
    QString style = QString(
                        "    background-color: rgb(0, 0, 0);"
                        "    color: white;" // Brighten text
                        "    border: 1px solid #555;"
                        "    border-radius: 4px;"
                        "    padding: 2px 4px;"
    );
    ui->toolBox->setStyleSheet(QString("QToolButton {") + style + QString("}"));
    ui->saveButton->setStyleSheet(style);
    ui->newButton->setStyleSheet(style);
    ui->loadButton->setStyleSheet(style);
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

// void drawLineTo(const QPoint &endPoint){
//     QPainter painter(&image);
//     painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//     pointer.drawLine(lastPoint, endPoint);
//     modified = true;
//     int rad = (myPenWidth / 2) + 2;

//     update(QRect(lastPoint, endPoint).normal)
// }

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // convert mouse click to global coordinates in mainwindow
        QPoint globalPos = event->globalPosition().toPoint();

        // map global coordinates to the graphicView's local coordinates
        QPoint viewPos = ui->graphicsView->mapFromGlobal(globalPos);

        qDebug() << "select pixel at scene Position: " << viewPos.x() / 10 << ", " << viewPos.y() / 10;

        if (ui->graphicsView->rect().contains(viewPos))
        {
            // Map to scene coordinates
            QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

            // Get pixel position
            int x = static_cast<int>(scenePos.x());
            int y = static_cast<int>(scenePos.y());

            // ui->coordinate->setText(QString("(x: %1, y: %2)").arg(x).arg(y));

            // qDebug() << "select pixel at scene Position: " << scenePos.x() << ", " << scenePos.y();

            // Check if in image bounds
            if (x >= 0 && x < model->getImage()->width() &&
                y >= 0 && y < model->getImage()->height())
            {

                drawing = true;
                currPixel = scenePos;

                // Update pixel
                // Handle tool-specific actions
                switch (currTool)
                {
                case Tool::BRUSH:
                    model->setPixelTracker(x, y, userColor); // Add brush logic here
                    break;                            // <--- Add this
                case Tool::ERASER:
                    model->erasePixel(x, y);
                    break; // <--- Add this
                case Tool::EYE:
                    palette->updateSlidersToColor(model->getImage()->pixelColor(x, y));
                    currTool = Tool::BRUSH;
                    break;
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
        if (event->type() == QEvent::MouseMove)
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

            QMouseEvent *me = static_cast<QMouseEvent *>(event);

            if (me->buttons() & (Qt::LeftButton))
            {
                // Check if moving from current pixel position
                if (x != currPixel.x() || y != currPixel.y())
                {
                    // Check if in image bounds
                    if (x >= 0 && x < model->getImage()->width() &&
                        y >= 0 && y < model->getImage()->height())
                    {

                        // Update pixel
                        // qDebug() << "alpha color is: " << userColor.alpha();

                        switch (currTool)
                        {
                        case Tool::BRUSH:
                            qDebug() << "using brush";
                            model->setPixelTracker(x, y, userColor);
                            break; // <--- Add this
                        case Tool::ERASER:
                            qDebug() << "using eraser";
                            model->erasePixel(x, y);
                            break; // <--- Add this
                        default:
                            break;
                        }

                        // model->setPixel(x, y, userColor.rgba());

                        // update current pixel
                        currPixel = scenePos;
                        updateView();
                    }
                }
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                qDebug() << "clearing tracker";
                model->clearTracker();
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        qDebug() << "clearing tracker";
        model->clearTracker();
    }
}

void MainWindow::updateView()
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(*model->getImage()));
}

void MainWindow::on_brushBttn_clicked()
{
    currTool = Tool::BRUSH;
    model->setSelectColor(QColor(0, 0, 0, 255)); // Black (to trigger blending)
}

void MainWindow::on_eraseBttn_clicked()
{
    currTool = Tool::ERASER;
    model->setSelectColor(QColor(255, 255, 255, 255)); // Non-black (to bypass blending)
}

void MainWindow::on_eyeBttn_clicked()
{
    currTool = Tool::EYE;
}

void MainWindow::on_newButton_clicked()
{
    bool ok;
    // Ask for the number of rows
    QString rowText = QInputDialog::getText(this, tr("Canvas Size"), tr("Please enter the size of the canvas:"), QLineEdit::Normal, QString(), &ok);
    if (!ok || rowText.isEmpty())
        return;  // User cancelled or left empty

    bool conversionOK;
    int rows = rowText.toInt(&conversionOK);
    if (!conversionOK) {
        QMessageBox::warning(this, tr("Invalid Input"), tr("Please enter a valid integer"));
        return;
    }
}
