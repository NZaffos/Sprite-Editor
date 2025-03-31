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
 *
 * @authors Noah Zaffos, Nash Hawkins
 * @date 3/28/2025
 */
class Displays : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor to initialize the frame selector and animation preview
     * @param ui A pointer to the ui with the elements to manage
     * @param model The model responsible for logic
     * @param parent
     */
    explicit Displays(Ui::MainWindow *ui, Model *model, QWidget *parent = nullptr);

private slots:
    /**
     * A handler for when one of the frames is clicked
     * Selects the clicked frame
     */
    void frameButtonClicked();

    /**
     * A handler for when the add frame button is clicked
     * Adds a new frame right after the currently selected frame
     */
    void addFrameButtonClicked();

    /**
     * A handler for when the delete frame button is clicked
     * Deletes the currently selected frame if there is more than one. Selects the previous frame
     * Clears the frame if there is only one frame left
     */
    void deleteFrameButtonClicked();

    /**
     * A handler for when the duplicate frame button is clicked
     * Adds a duplicated frame right after the currently selected frame
     */
    void duplicateFrameButtonClicked();

    /**
     * A handler for when the shift frame up button is clicked
     * Moves the currently selected frame up one position
     * Does not move if it is the first frame
     */
    void shiftFrameUpClicked();

    /**
     * A handler for when the shift frame down button is clicked
     * Moves the currently selected frame down one position
     * Does not move if it is the last frame
     */
    void shiftFrameDownClicked();

    /**
     * Sets the fps counter to show the current fps of the animation preview
     * @param value The value to set the fps counter to
     */
    void updateFpsText(int value);

    /**
     * Toggles the play/pause button to display the correct icon
     * @param enabled The position to set the icon to. True means currently playing, false means currently paused
     */
    void toggleAnimationPlayPauseIcon(bool enabled);

    /**
     * Draws the animation for the frame at the given index
     * @param index The index for the frame to display
     */
    void drawAnimationIcon(int index);

    /**
     * Tells the class the canvas has been updated and to subsequently update the frames
     */
    void onCanvasUpdated();

private:
    /**
     * The ui holding all visual elements
     */
    Ui::MainWindow *ui;

    /**
     * The model to handle logic
     */
    Model *model;

    /**
     * Gives the style for all buttons
     * @return A QString to use as the style sheet for all buttons
     */
    QString getButtonStyle();

    /**
     * The layout to display all of the frames
     */
    QVBoxLayout *framesLayout = nullptr;

    /**
     * A vector to hold all of the current frames as QPushButtons
     */
    QVector<QPushButton *> frameButtons;

    /**
     * The index of the currently selected frame
     */
    int selectedFrameIndex = 0;

    /**
     * Initializes the frame selector to its default state
     */
    void initializeFrameSelector();

    /**
     * Creates a new frame at the given index
     * @param index The index of the new frame
     */
    void createFrameButton(int index);

    /**
     * Updates the frame buton to have the most up to date image of the canvas
     * @param button The button for the frame to update
     * @return The button with the updated icon
     */
    QPushButton *updateFrameButtonIcon(QPushButton *button);

    /**
     * Updates all frames to have the correct border for styling
     */
    void updateFrameButtonStyle();

    /**
     * Initializes all animation preview controls
     */
    void initializeAnimationControls();
};

#endif // DISPLAYS_H
