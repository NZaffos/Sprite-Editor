// Reviewed by John Chen
/**
 * University of Utah – CS 3505
 * @authors Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen
 * @date 03/25/2025
 * @brief This class is responsible for handling the backend logic of the sprite editor such as painting, erasing, creating new canvas sizes and more
 */

#include "models.h"
#include "qpainter.h"
#include "qpixmap.h"
#include "QTimer"
#include "QJsonObject"
#include "QJsonArray"
#include "QFileDialog"
#include "QJsonDocument"
#include "QMessageBox"

Model::Model(QObject *parent) : QObject(parent)
{
    animationTimer = new QTimer(this);
    // Connect the timer's timeout signal to the updateAnimationFrame slot
    connect(animationTimer, &QTimer::timeout, this, &Model::updateAnimationFrame);
    createImage(32); // Initialize a new canvas of size 32x32
}

Model::~Model()
{
    // Clean up allocated images to avoid memory leaks
    delete image;
    delete shapePreview;
    delete tracker;
}

void Model::createImage(int inputSize)
{
    size = inputSize;

    image = new QImage(size, size, QImage::Format_ARGB32);
    clearCanvas();

    // Reset frames and add the first frame (the new image)
    frames.clear();
    frames.push_back(*image);
    currentFrameIndex = 0;
    animationIndex = 0;

    // Restart the animation timer if active
    if (animationTimer && animationTimer->isActive())
        animationTimer->stop();
    animationPlaying = true;
    animationTimer->start(1000 / animationFps);
    updateAnimationFrame();

    // Initialize additional images used for tracking and shape previews
    tracker = new QImage(size, size, QImage::Format_ARGB32);
    shapePreview = new QImage(size, size, QImage::Format_ARGB32);
    clearNonCanvas();
}

QImage *Model::getImage()
{
    return image;
}

void Model::clearCanvas()
{
    // Clear the main canvas (set all pixels to 0)
    image->fill(0);
    if (frames.size() > 0)
        frames[currentFrameIndex] = *image;
    emit canvasUpdated();
}

void Model::clearNonCanvas()
{
    // Clear auxiliary images used for tracker and shape preview
    tracker->fill(0);
    shapePreview->fill(0);
    emit canvasUpdated();
}

void Model::addFrame()
{
    QImage newFrame(size, size, QImage::Format_ARGB32);
    newFrame.fill(0);

    // Insert the new frame right after the current frame
    auto pos = frames.begin() + currentFrameIndex + 1;
    frames.insert(pos, newFrame);
    selectFrame(++currentFrameIndex);
}

void Model::duplicateFrame()
{
    // Create a copy of the current frame and insert it as a new frame
    QImage newFrame = frames[currentFrameIndex].copy();

    auto pos = frames.begin() + currentFrameIndex + 1;
    frames.insert(pos, newFrame);
    selectFrame(++currentFrameIndex);
}

void Model::removeFrame(unsigned int index)
{
    // Ensure at least one frame remains in the project
    if (frames.size() <= 1 || index >= frames.size())
    {
        clearCanvas();
        frames[0] = *image;
        return;
    }

    frames.erase(frames.begin() + index);

    // Adjust current frame index after deletion
    if (currentFrameIndex > 0)
        currentFrameIndex--;
    else
        currentFrameIndex = 0;

    *image = frames[currentFrameIndex];

    emit frameModified(currentFrameIndex);
    emit canvasUpdated();
}

void Model::selectFrame(unsigned int index)
{
    // Change the current frame if the index is valid
    if (index < frames.size())
    {
        *image = frames[index];
        currentFrameIndex = index;
        emit frameModified(index);
        emit requestNewSelectedFrameIndex(index);
        emit canvasUpdated();
    }
}

unsigned int Model::getCurrentFrameIndex() const
{
    return currentFrameIndex;
}

QPixmap Model::getFrameThumbnail(int index, int width, int height) const
{
    // Generate a scaled thumbnail from the frame, if valid
    if (index < static_cast<int>(frames.size()) && index >= 0)
    {
        return QPixmap::fromImage(frames[index]).scaled(width, height, Qt::KeepAspectRatio, Qt::FastTransformation);
    }
    return QPixmap();
}

const std::vector<QImage> &Model::getFrames() const
{
    return frames;
}

void Model::shiftFrameUp()
{
    swapFrame(true); // Swap with the frame above
}

void Model::shiftFrameDown()
{
    swapFrame(false); // Swap with the frame below
}

void Model::swapFrame(bool swapUp)
{
    // Determine the offset: -1 for up, +1 for down
    int offset;
    if (swapUp)
        offset = -1;
    else
        offset = 1;

    // Save references to the selected and target frames for swapping
    QImage &selectedImage = frames[currentFrameIndex];
    QImage &swappingImage = frames[currentFrameIndex + offset];

    // Swap the contents using a temporary image copy
    QImage tempImage = selectedImage;
    selectedImage = swappingImage.copy();
    swappingImage = tempImage.copy();

    // Update the current frame index to reflect the swap
    currentFrameIndex += offset;

    *image = frames[currentFrameIndex].copy();
    emit frameModified(currentFrameIndex);
    emit canvasUpdated();
}

void Model::mirrorFrame()
{
    QImage temp = image->copy();
    // Mirror the image horizontally by swapping pixels from left to right
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // The pixel from the mirrored horizontal position is used
            image->setPixelColor(i, j, temp.pixelColor(size - 1 - i, j));
        }
    }
    // Update the frame if the index is valid
    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::rotateFrame()
{
    QImage temp = image->copy();
    // Rotate the image 90 degrees clockwise by remapping pixel positions
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // New position comes from transposing and flipping the indices
            image->setPixelColor(i, j, temp.pixelColor(j, size - 1 - i));
        }
    }
    // Update the frame storage with the rotated image
    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::sliderValueChanged(int value)
{
    animationFps = value;
    emit updateFpsSliderIO(value);
    if (animationTimer->isActive())
        animationTimer->setInterval(1000 / animationFps); // Update timer interval based on FPS
}

void Model::toggleAnimation()
{
    // Toggle the play/pause state of the animation
    if (animationPlaying)
    {
        animationPlaying = false;
        animationTimer->stop();
        emit togglePlayPauseButtonIcon(false);
    }
    else
    {
        animationPlaying = true;
        animationTimer->start(1000 / animationFps);
        emit togglePlayPauseButtonIcon(true);
    }
}

void Model::updateAnimationFrame()
{
    // Do nothing if the animation is paused
    if (!animationPlaying)
        return;

    emit updateAnimationIcon(animationIndex);

    // Cycle through frames; if at the last frame, restart at the beginning
    if (animationIndex >= static_cast<int>(frames.size()) - 1)
        animationIndex = 0;
    else
        animationIndex++;
}

void Model::setPixel(int x, int y, QColor userColor)
{
    // Retrieve the current color at (x, y) into selectColor
    getPixel(x, y);

    // If the current pixel is transparent (all channels 0), simply set the user color
    if (selectColor.red() == 0 && selectColor.green() == 0 && selectColor.blue() == 0 && selectColor.alpha() == 0)
    {
        image->setPixelColor(x, y, userColor);
    }
    else
    {
        // Otherwise, blend the colors additively
        QColor blendedColor = blendAdditive(userColor, selectColor);
        image->setPixelColor(x, y, blendedColor); // Apply blended color
    }

    // Update the current frame in the frames vector
    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }

    emit canvasUpdated();
    // Also update the tracker image with the user color
    tracker->setPixelColor(x, y, userColor);
}

void Model::setPixelTracker(int x, int y, QColor userColor)
{
    // Only set the pixel if the tracker doesn't already have the desired color
    if (tracker->pixelColor(x, y) == userColor)
    {
        return;
    }
    setPixel(x, y, userColor);
}

QImage *Model::getShapePreview()
{
    return shapePreview;
}

void Model::shapeStart(int x, int y)
{
    // Record the starting position for a shape draw
    shapeStartX = x;
    shapeStartY = y;
}

void Model::rectangleShape(int x, int y, QColor userColor)
{
    // Clear previous shape preview content
    shapePreview->fill(QColor(0, 0, 0, 0));
    QPainter painter(shapePreview);
    QPen pen(userColor);
    pen.setWidth(1);
    painter.setPen(pen);
    // Draw a rectangle defined by the start point and current mouse position
    painter.drawRect(std::min(shapeStartX, x),
                     std::min(shapeStartY, y),
                     qAbs(shapeStartX - x),
                     qAbs(shapeStartY - y));
    painter.end();
}

void Model::ellipseShape(int x, int y, QColor userColor)
{
    // Clear previous shape preview content
    shapePreview->fill(QColor(0, 0, 0, 0));
    QPainter painter(shapePreview);
    QPen pen(userColor);
    pen.setWidth(1);
    painter.setPen(pen);
    // Draw an ellipse defined by the start point and current mouse position
    painter.drawEllipse(std::min(shapeStartX, x),
                        std::min(shapeStartY, y),
                        qAbs(shapeStartX - x),
                        qAbs(shapeStartY - y));
    painter.end();
}

void Model::mergeShapePreview()
{
    // Merge the shape preview into the main image
    QPainter painter(image);
    painter.drawImage(0, 0, *shapePreview);
    painter.end();
    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::paintBucket(int x, int y, QColor userColor)
{
    // Get the color at the starting pixel (the region to be filled)
    getPixel(x, y);
    QColor colorToReplace = selectColor;
    // If the target color is the same as the user-selected color, nothing to do
    if (colorToReplace == userColor)
    {
        return;
    }
    // Start recursive flood fill
    paintBucketRecursive(x, y, userColor, colorToReplace);
    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::paintBucketRecursive(int x, int y, QColor userColor, QColor colorToReplace)
{
    // Base conditions for recursion: if (x, y) is out of bounds, return immediately
    if (x >= size || x < 0 || y >= size || y < 0)
    {
        return;
    }
    // Update selectColor with the color at the current pixel
    getPixel(x, y);
    // If the current pixel is not the target color, stop the recursion along this path
    if (selectColor != colorToReplace)
    {
        return;
    }
    // Set the current pixel to the user-selected color
    image->setPixelColor(x, y, userColor);
    emit canvasUpdated();
    // Recursively call for neighboring pixels (4-connected flood fill)
    paintBucketRecursive(x + 1, y, userColor, colorToReplace);
    paintBucketRecursive(x - 1, y, userColor, colorToReplace);
    paintBucketRecursive(x, y + 1, userColor, colorToReplace);
    paintBucketRecursive(x, y - 1, userColor, colorToReplace);
}

void Model::erasePixel(int x, int y)
{
    // Set the pixel at (x, y) to fully transparent
    image->setPixelColor(x, y, QColor(0, 0, 0, 0));

    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::getPixel(int x, int y)
{
    // Store the color at (x, y) in selectColor
    selectColor = image->pixelColor(x, y);
}

int Model::getCanvasSize()
{
    return size;
}

void Model::setSelectColor(QColor color)
{
    selectColor = color;
}

QColor Model::blendAdditive(QColor src, QColor dest)
{
    // Extract individual color components from the source and destination colors
    int redDest = dest.red(), greenDest = dest.green(), blueDest = dest.blue(), alphaDest = dest.alpha();
    int redSrc = src.red(), greenSrc = src.green(), blueSrc = src.blue(), alphaSrc = src.alpha();

    // Compute the new alpha value using an additive blend formula
    int alphaOverride = alphaSrc + alphaDest * (255 - alphaSrc) / 255;
    if (alphaOverride == 0)
        return QColor(0, 0, 0, 0); // Fully transparent if resulting alpha is zero

    // Compute new RGB values based on the relative contribution of each color
    int redOverride = (redSrc * alphaSrc + redDest * alphaDest * (255 - alphaSrc) / 255) / alphaOverride;
    int greenOverride = (greenSrc * alphaSrc + greenDest * alphaDest * (255 - alphaSrc) / 255) / alphaOverride;
    int blueOverride = (blueSrc * alphaSrc + blueDest * alphaDest * (255 - alphaSrc) / 255) / alphaOverride;

    return QColor(redOverride, greenOverride, blueOverride, alphaOverride);
}

void Model::addToPalette(QColor color)
{
    palette.push_back(color);
}

void Model::removeFromPalette(unsigned int index)
{
    palette.removeAt(index);
}

QColor Model::getColorFromPalette(unsigned int index)
{
    return palette.at(index);
}

void Model::saveProject()
{
    QJsonObject json;
    json["width"] = image->width();
    json["frameCount"] = int(frames.size());

    QJsonArray frameData;
    // Iterate over each frame and each pixel to store RGBA values in JSON
    for (QImage &frame : frames)
    {
        QJsonArray singleFrame;
        for (int y = 0; y < image->width(); ++y)
        {
            for (int x = 0; x < image->width(); ++x)
            {
                QColor color = frame.pixelColor(x, y);
                // Append each color component in order: red, green, blue, alpha
                singleFrame.append(color.red());
                singleFrame.append(color.green());
                singleFrame.append(color.blue());
                singleFrame.append(color.alpha());
            }
        }
        frameData.append(singleFrame);
    }
    json["frames"] = frameData;

    // Open a save file dialog to get the file path
    QString filePath = QFileDialog::getSaveFileName(nullptr,
                                                    "Save Image as SSP",
                                                    "",
                                                    "SSP Files (.ssp);;All Files (*)");

    if (!filePath.isEmpty())
    {
        QJsonDocument doc(json);
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(doc.toJson());
            file.close();
        }
        else
        {
            QMessageBox::warning(nullptr, "Save Error", "Failed to save SSP file.");
        }
    }
}

void Model::loadProject()
{
    // Open a file dialog to select the project file
    QString filePath = QFileDialog::getOpenFileName(nullptr,
                                                    "Load Project",
                                                    "QDir::rootPath()",
                                                    "SSP Files (.ssp);;All Files (*)");
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        // Attempt to open the file for reading
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(nullptr, "Error", "Unable to open the file.");
            return;
        }

        // Read the entire file content into a QByteArray
        QByteArray fileData = file.readAll();
        file.close();

        frames.clear();
        frames.push_back(*image);
        currentFrameIndex = 0;

        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        if (doc.isObject())
        {
            // Extract width and frameCount from the JSON object
            QJsonObject jsonObject = doc.object();
            int width = jsonObject["width"].toInt();
            size = width;
            emit requestWindowResize(size);
            int frameCount = jsonObject["frameCount"].toInt();

            QJsonArray jsonFrames = jsonObject["frames"].toArray();

            // Create additional frames if needed
            for (int i = 0; i < frameCount - 1; i++)
            {
                emit requestNewFrame();
            }

            int frameIndex = 0;
            // Load each frame's pixel data from the JSON array
            for (const QJsonValue &frameValue : jsonFrames)
            {
                selectFrame(frameIndex);
                QJsonArray frameArray = frameValue.toArray();

                // Loop through every set of 4 values (RGBA) for the frame
                for (int i = 0; i < frameArray.size() / 4; i++)
                {
                    int red = frameArray[4 * i].toInt();
                    int green = frameArray[4 * i + 1].toInt();
                    int blue = frameArray[4 * i + 2].toInt();
                    int alpha = frameArray[4 * i + 3].toInt();

                    QColor color(red, green, blue, alpha);
                    // Determine pixel coordinates using modulus and division
                    setPixel(i % (int)size, i / size, color);
                }
                frameIndex++;
            }
            selectFrame(0);
            emit framesReloaded();
        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "The JSON format is incorrect.");
        }
    }
}
