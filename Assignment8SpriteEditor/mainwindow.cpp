#include "mainwindow.h"
#include "ui_mainwindow.h"

// This is the view class IMPORTANT:
// Delete this comment before submission!!!
MainWindow::MainWindow(Model *model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(model)
{
    ui->setupUi(this);

    // New Displays
    displays = new Displays(model);
    palette = new Palette(ui, model, userColor);

    // Set fps slider ui
    setAnimationFpsSliderAndWindow();

    // Set frame selector ui
    setFrameSelector();

    userColor = QColor(0, 0, 0, 255);
    currTool = Tool::BRUSH;

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Add the pixmap to the scene
    scene->addPixmap(QPixmap::fromImage(*model->getImage()));

    // Ensure the scenes area matches the pixmap
    scene->setSceneRect(scene->itemsBoundingRect());

    // Add default canvas to frame selector
    createFrameButton(0);
    updateFrameButtonStyle();

    // Get user canvas size
    int canvasSize = 725 / model->getCanvasSize();

    // Configure the view
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, false);
    ui->graphicsView->scale(canvasSize, canvasSize);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->viewport()->installEventFilter(this);

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

    // Frame selector
    connect(ui->deleteFrameButton,
            &QPushButton::clicked,
            this,
            &MainWindow::deleteFrame);
    connect(ui->addFrameButton,
            &QPushButton::clicked,
            this,
            &MainWindow::addFrameButtonClicked);
    connect(ui->dublicateFrameButton,
            &QPushButton::clicked,
            this,
            &MainWindow::duplicateFrameButtonClicked);
    connect(ui->shiftDownFrameButton,
            &QPushButton::clicked,
            this,
            &MainWindow::shiftFrameDownClicked);
    connect(ui->shiftUpFrameButton,
            &QPushButton::clicked,
            this,
            &MainWindow::shiftFrameUpClicked);
    connect(ui->clearCanvasButton, &QPushButton::clicked,
            model, &Model::clearCanvas);

    // Animation window and slider
    connect(ui->animationFpsSlider,
            &QSlider::valueChanged,
            model,
            &Model::sliderValueChanged);
    connect(model,
            &Model::updateAnimationIcon,
            this,
            &MainWindow::drawAnimationIcon);
    connect(model,
            &Model::updateFpsSliderIO,
            this,
            &MainWindow::updateFpsText);
    connect(model,
            &Model::togglePlayPauseButtonIcon,
            this,
            &MainWindow::toggleAnimationPlayPauseIcon);
      
    // New/Save/Load connections
    connect(ui->saveButton,
            &QPushButton::clicked,
            model,
            &Model::saveProject);
    connect(ui->loadButton,
            &QPushButton::clicked,
            model,
            &Model::loadProject);
} // End of constructor

void MainWindow::setAnimationFpsSliderAndWindow()
{
    QString sliderStyle = palette->getSliderStyleSheet();
    ui->animationFpsSlider->setStyleSheet(sliderStyle);
    ui->animationFpsSlider->setRange(1, 60);
    updateFpsText(1);
}

void MainWindow::toggleAnimationPlayPauseIcon(bool enabled) 
{
    if (enabled)
        ui->animationPlayPauseButton->setText(QString("| |"));
    else
        ui->animationPlayPauseButton->setText(QString(">"));
}

void MainWindow::updateFpsText(int value) 
{
    ui->animationFpsSliderIO->setText(QString("FPS: ") + QString::number(value));
}

void MainWindow::drawAnimationIcon(int index)
{
    QPixmap newImage = model->getFrameThumbnail(index, 220, 220);
    ui->animationDisplayLabel->setPixmap(newImage);
}

void MainWindow::updateFrameButtonStyle()
{
    for (int i = 0; i < frameButtons.size(); i++)
    {
        if (i == selectedFrameIndex)
            frameButtons[i]->setStyleSheet("QPushButton { border: 2px solid blue; }");
        else
            frameButtons[i]->setStyleSheet("QPushButton { no-border }");
    }
}

void MainWindow::setFrameSelector()
{
    framesScrollArea = ui->frameSelector;
    framesContainer = ui->frameSelectorScrollContent;

    framesLayout = new QVBoxLayout();
    framesLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    framesLayout->setContentsMargins(0, 20, 0, 20);

    framesContainer->setLayout(framesLayout);

    framesScrollArea->setWidget(framesContainer);
    framesScrollArea->setWidgetResizable(true);
}

void MainWindow::createFrameButton(int index)
{
    QPushButton *frameButton = new QPushButton();

    frameButton->setProperty("frameIndex", index);
    updateFrameButtonIcon(frameButton);
    connect(frameButton, &QPushButton::clicked, this, &MainWindow::frameButtonClicked);

    framesLayout->insertWidget(index, frameButton);
    frameButtons.insert(index, frameButton);
}

QPushButton *MainWindow::updateFrameButtonIcon(QPushButton *button)
{
    int index = button->property("frameIndex").toInt();
    QPixmap thumbnail = model->getFrameThumbnail(index, 110, 110);
    button->setIcon(QIcon(thumbnail));
    button->setIconSize(thumbnail.size());
    button->setFixedSize(112, 112);

    return button;
}

void MainWindow::frameButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button)
    {
        int index = button->property("frameIndex").toInt();
        model->selectFrame(index);
        selectedFrameIndex = index;
        updateFrameButtonStyle();
        updateView();
    }
}

void MainWindow::deleteFrame()
{
    QPushButton *buttonToRemove = frameButtons[selectedFrameIndex];
    if (model->getFrames().size() <= 1) {
        model->removeFrame(selectedFrameIndex);
    
    model->removeFrame(selectedFrameIndex);

        updateFrameButtonIcon(buttonToRemove);
        updateView();
        return;
    }

    model->removeFrame(selectedFrameIndex);
    framesLayout->removeWidget(buttonToRemove);
    buttonToRemove->deleteLater();
    frameButtons.remove(selectedFrameIndex);

    for (int i = selectedFrameIndex; i < frameButtons.size(); i++)
    {
        frameButtons[i]->setProperty("frameIndex", i);
    }

    selectedFrameIndex = model->getCurrentFrameIndex();
    updateFrameButtonStyle();
    updateView();
}

void MainWindow::addFrameButtonClicked()
{
    model->addFrame();
    int newIndex = model->getCurrentFrameIndex();
    createFrameButton(newIndex);

    for (int i = newIndex + 1; i < frameButtons.size(); i++)
    {
        frameButtons[i]->setProperty("frameIndex", i);
    }

    selectedFrameIndex = newIndex;
    updateFrameButtonStyle();
    updateView();
}

void MainWindow::duplicateFrameButtonClicked()
{
    model->duplicateFrame();
    int newIndex = model->getCurrentFrameIndex();
    createFrameButton(newIndex);

    for (int i = newIndex + 1; i < frameButtons.size(); i++)
    {
        frameButtons[i]->setProperty("frameIndex", i);
    }

    selectedFrameIndex = newIndex;
    updateFrameButtonStyle();
    updateView();
}

void MainWindow::shiftFrameUpClicked()
{
    if (selectedFrameIndex <= 0)
        return;

    model->shiftFrameUp();
    selectedFrameIndex = model->getCurrentFrameIndex();
    updateFrameButtonIcon(frameButtons[selectedFrameIndex + 1]);

    updateFrameButtonStyle();
    updateView();
}

void MainWindow::shiftFrameDownClicked()
{
    if (selectedFrameIndex >= frameButtons.size() - 1)
        return;

    model->shiftFrameDown();
    selectedFrameIndex = model->getCurrentFrameIndex();
    updateFrameButtonIcon(frameButtons[selectedFrameIndex - 1]);

    updateFrameButtonStyle();
    updateView();
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
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
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
                    model->setPixel(x, y, userColor); // Add brush logic here
                    break;                            // <--- Add this
                case Tool::ERASER:
                    model->erasePixel(x, y);
                    break; // <--- Add this
                }

                updateView();
            }
        }
    }
}

// void MainWindow::mouseMoveEvent(QMouseEvent *event){
//     qDebug() << "mouse is moving";
//     if (drawing && (event->buttons() & Qt::LeftButton)){
//         // map global coordinates to the graphicView's local coordinates
//         QPoint viewPos = ui->graphicsView->mapFromGlobal(event->globalPosition().toPoint());

//         qDebug() << "select pixel at scene Position: " << viewPos.x()/10 << ", " << viewPos.y()/10;

//         if (ui->graphicsView->rect().contains(viewPos)) {
//             // Map to scene coordinates
//             QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

//             // Get pixel position
//             int x = static_cast<int>(scenePos.x());
//             int y = static_cast<int>(scenePos.y());

//             ui->coordinate->setText(QString("(x: %1, y: %2)").arg(x).arg(y));

//             // Check if moving from current pixel position
//             if(x != currPixel.x() || y != currPixel.y()){
//                 // Check if in image bounds
//                 if (x >= 0 && x < model->getImage()->width() &&
//                     y >= 0 && y < model->getImage()->height()) {

//                     // Update pixel
//                     qDebug() << "alpha color is: " << userColor.alpha();
//                     model->setPixel(x, y, userColor.rgba());

//                     // update current pixel
//                     currPixel = scenePos;
//                     updateView();
//                 }
//             }
//         }
//     }
// }

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

            if (me->buttons() & (Qt::LeftButton | Qt::RightButton))
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
                            model->setPixel(x, y, userColor);
                            break; // <--- Add this
                        case Tool::ERASER:
                            qDebug() << "using eraser";
                            model->erasePixel(x, y);
                            break; // <--- Add this
                        }

                        // model->setPixel(x, y, userColor.rgba());

                        // update current pixel
                        currPixel = scenePos;
                        updateView();
                    }
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::updateView()
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(*model->getImage()));
    if (!frameButtons.isEmpty() && selectedFrameIndex >= 0 && selectedFrameIndex < frameButtons.size())
    {
        updateFrameButtonIcon(frameButtons[selectedFrameIndex]);
    }
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
