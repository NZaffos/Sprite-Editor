#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include "models.h"
#include "ui_mainwindow.h"

/**
 * This class handles displaying the frame selector and animation preview
 */
class Displays : public QWidget
{
    Q_OBJECT

public:
    explicit Displays(Ui::MainWindow *ui, Model *model, QWidget *parent = nullptr);
public slots:
    void addFrameButtonClicked(); // Moved to public for model to request a new frame
    void setSelectedFrameIndex(unsigned int index);
private slots:
    // Frame selector
    void frameButtonClicked();
    void deleteFrameButtonClicked();
    void duplicateFrameButtonClicked();
    void shiftFrameUpClicked();
    void shiftFrameDownClicked();

    // Animation
    void updateFpsText(int value);
    void toggleAnimationPlayPauseIcon(bool enabled);
    void drawAnimationIcon(int index);

    // Canvas updated
    void onCanvasUpdated();

private:
    Ui::MainWindow *ui;
    Model *model;

    QString getButtonStyle();

    // Frame selector
    QVBoxLayout *framesLayout = nullptr;
    QVector<QPushButton *> frameButtons;
    int selectedFrameIndex = 0;
    void initializeFrameSelector();
    void createFrameButton(int index);
    QPushButton *updateFrameButtonIcon(QPushButton *button);
    void updateFrameButtonStyle();

    // Animation
    void initializeAnimationControls();
};

#endif // DISPLAYS_H
