#include "displays.h"
#include <QPainter>

Displays::Displays(Ui::MainWindow *ui, Model *model, QWidget *parent)
    : QWidget(parent), ui(ui), model(model)
{
    initializeFrameSelector();
    initializeAnimationControls();

    connect(model,
            &Model::updateAnimationIcon,
            this,
            &Displays::drawAnimationIcon);
    connect(model,
            &Model::updateFpsSliderIO,
            this,
            &Displays::updateFpsText);
    connect(model,
            &Model::togglePlayPauseButtonIcon,
            this,
            &Displays::toggleAnimationPlayPauseIcon);
    connect(ui->clearCanvasButton,
            &QPushButton::clicked,
            model,
            &Model::clearCanvas);
    connect(model,
            &Model::canvasUpdated,
            this,
            &Displays::onCanvasUpdated);
}

void Displays::initializeFrameSelector()
{
    framesLayout = new QVBoxLayout();
    framesLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    framesLayout->setContentsMargins(0, 20, 0, 20);

    ui->frameSelectorScrollContent->setLayout(framesLayout);
    ui->frameSelector->setWidget(ui->frameSelectorScrollContent);
    ui->frameSelector->setWidgetResizable(true);

    connect(ui->addFrameButton,
            &QPushButton::clicked,
            this,
            &Displays::addFrameButtonClicked);
    connect(ui->deleteFrameButton,
            &QPushButton::clicked,
            this,
            &Displays::deleteFrameButtonClicked);
    connect(ui->dublicateFrameButton,
            &QPushButton::clicked,
            this,
            &Displays::duplicateFrameButtonClicked);
    connect(ui->shiftUpFrameButton,
            &QPushButton::clicked,
            this,
            &Displays::shiftFrameUpClicked);
    connect(ui->shiftDownFrameButton,
            &QPushButton::clicked,
            this,
            &Displays::shiftFrameDownClicked);

    createFrameButton(0);
    updateFrameButtonStyle();
}

void Displays::createFrameButton(int index)
{
    QPushButton* frameButton = new QPushButton(ui->frameSelectorScrollContent);
    frameButton->setProperty("frameIndex", index);

    updateFrameButtonIcon(frameButton);
    connect(frameButton, &QPushButton::clicked, this, &Displays::frameButtonClicked);

    framesLayout->insertWidget(index, frameButton);
    frameButtons.insert(index, frameButton);
}

QPushButton* Displays::updateFrameButtonIcon(QPushButton* button)
{
    int index = button->property("frameIndex").toInt();
    QPixmap thumbnail = model->getFrameThumbnail(index, 110, 110);

    button->setIcon(QIcon(thumbnail));
    button->setIconSize(thumbnail.size());
    button->setFixedSize(112, 112);

    return button;
}

void Displays::updateFrameButtonStyle()
{
    for (int i = 0; i < frameButtons.size(); i++)
    {
        if (i == selectedFrameIndex)
            frameButtons[i]->setStyleSheet("QPushButton { border: 2px solid blue; }");
        else
            frameButtons[i]->setStyleSheet("");
    }
}

void Displays::frameButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    int index = button->property("frameIndex").toInt();
    model->selectFrame(index);
    selectedFrameIndex = index;
    updateFrameButtonStyle();

    updateFrameButtonIcon(button);
}

void Displays::addFrameButtonClicked()
{
    model->addFrame();
    int newIndex = model->getCurrentFrameIndex();
    createFrameButton(newIndex);

    // Reassign properties for following frameButtons
    for (int i = newIndex + 1; i < frameButtons.size(); i++) {
        frameButtons[i]->setProperty("frameIndex", i);
    }

    selectedFrameIndex = newIndex;
    updateFrameButtonStyle();
}

void Displays::duplicateFrameButtonClicked()
{
    model->duplicateFrame();
    int newIndex = model->getCurrentFrameIndex();
    createFrameButton(newIndex);

    for (int i = newIndex + 1; i < frameButtons.size(); i++) {
        frameButtons[i]->setProperty("frameIndex", i);
    }

    selectedFrameIndex = newIndex;
    updateFrameButtonStyle();
}

void Displays::deleteFrameButtonClicked()
{
    QPushButton* buttonToRemove = frameButtons[selectedFrameIndex];
    if (model->getFrames().size() <= 1) {
        model->removeFrame(selectedFrameIndex);

        updateFrameButtonIcon(buttonToRemove);
        return;
    }

    model->removeFrame(selectedFrameIndex);
    framesLayout->removeWidget(buttonToRemove);
    buttonToRemove->deleteLater();
    frameButtons.remove(selectedFrameIndex);

    // Reassign properties for following frameButtons
    for (int i = selectedFrameIndex; i < frameButtons.size(); i++) {
        frameButtons[i]->setProperty("frameIndex", i);
    }

    selectedFrameIndex = model->getCurrentFrameIndex();
    updateFrameButtonStyle();
}

void Displays::shiftFrameUpClicked()
{
    if (selectedFrameIndex <= 0)
        return;

    model->shiftFrameUp();
    selectedFrameIndex = model->getCurrentFrameIndex();
    updateFrameButtonIcon(frameButtons[selectedFrameIndex + 1]);
    updateFrameButtonIcon(frameButtons[selectedFrameIndex]);

    updateFrameButtonStyle();
}

void Displays::shiftFrameDownClicked()
{
    if (selectedFrameIndex >= frameButtons.size() - 1)
        return;

    model->shiftFrameDown();
    selectedFrameIndex = model->getCurrentFrameIndex();
    updateFrameButtonIcon(frameButtons[selectedFrameIndex - 1]);
    updateFrameButtonIcon(frameButtons[selectedFrameIndex]);

    updateFrameButtonStyle();
}

void Displays::onCanvasUpdated()
{
    if (selectedFrameIndex >= 0 && selectedFrameIndex < frameButtons.size()) {
        updateFrameButtonIcon(frameButtons[selectedFrameIndex]);
    }
}

void Displays::initializeAnimationControls()
{
    ui->animationFpsSlider->setRange(1, 60);
    updateFpsText(1);
    // For style, you can re‑use palette->getSliderStyleSheet() or do your own.

    connect(ui->animationFpsSlider,
            &QSlider::valueChanged,
            model,
            &Model::sliderValueChanged);
    connect(ui->animationPlayPauseButton,
            &QPushButton::clicked,
            model,
            &Model::toggleAnimation);
}

void Displays::updateFpsText(int value)
{
    ui->animationFpsSliderIO->setText(QString("FPS: %1").arg(value));
}

void Displays::toggleAnimationPlayPauseIcon(bool enabled)
{
    if (enabled)
        ui->animationPlayPauseButton->setText("| |");
    else
        ui->animationPlayPauseButton->setText(">");
}

void Displays::drawAnimationIcon(int index)
{
    QPixmap newImage = model->getFrameThumbnail(index, 220, 220);
    ui->animationDisplayLabel->setPixmap(newImage);
}
