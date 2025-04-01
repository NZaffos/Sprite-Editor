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
    connect(animationTimer, &QTimer::timeout, this, &Model::updateAnimationFrame);
    createImage(32);
}

Model::~Model()
{
    delete image;
    delete shapePreview;
    delete tracker;
}

void Model::createImage(int inputSize)
{
    size = inputSize;

    image = new QImage(size, size, QImage::Format_ARGB32);
    clearCanvas();

    frames.clear();
    frames.push_back(*image);
    currentFrameIndex = 0;
    animationIndex = 0;

    if (animationTimer && animationTimer->isActive())
        animationTimer->stop();
    animationPlaying = true;
    animationTimer->start(1000 / animationFps);
    updateAnimationFrame();

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
    image->fill(0);
    if (frames.size() > 0)
        frames[currentFrameIndex] = *image;
    emit canvasUpdated();
}

void Model::clearNonCanvas()
{
    tracker->fill(0);
    shapePreview->fill(0);
    emit canvasUpdated();
}

void Model::addFrame()
{
    QImage newFrame(size, size, QImage::Format_ARGB32);
    newFrame.fill(0);

    auto pos = frames.begin() + currentFrameIndex + 1;
    frames.insert(pos, newFrame);
    selectFrame(++currentFrameIndex);
}

void Model::duplicateFrame()
{
    QImage newFrame = frames[currentFrameIndex].copy();

    auto pos = frames.begin() + currentFrameIndex + 1;
    frames.insert(pos, newFrame);
    selectFrame(++currentFrameIndex);
}

void Model::removeFrame(unsigned int index)
{
    if (frames.size() <= 1 || index >= frames.size())
    {
        clearCanvas();
        frames[0] = *image;
        return;
    }

    frames.erase(frames.begin() + index);

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
    swapFrame(true);
}

void Model::shiftFrameDown()
{
    swapFrame(false);
}

void Model::swapFrame(bool swapUp)
{
    int offset;
    if (swapUp)
        offset = -1;
    else
        offset = 1;

    QImage &selectedImage = frames[currentFrameIndex];
    QImage &swappingImage = frames[currentFrameIndex + offset];

    QImage tempImage = selectedImage;
    selectedImage = swappingImage.copy();
    swappingImage = tempImage.copy();

    currentFrameIndex += offset;

    *image = frames[currentFrameIndex].copy();
    emit frameModified(currentFrameIndex);
    emit canvasUpdated();
}

void Model::mirrorFrame()
{
    QImage temp = image->copy();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            image->setPixelColor(i, j, temp.pixelColor(size - 1 - i, j));
        }
    }
    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::rotateFrame()
{
    QImage temp = image->copy();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            image->setPixelColor(i, j, temp.pixelColor(j, size - 1 - i));
        }
    }
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
        animationTimer->setInterval(1000 / animationFps);
}

void Model::toggleAnimation()
{
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
    if (!animationPlaying)
        return;

    emit updateAnimationIcon(animationIndex);

    if (animationIndex >= static_cast<int>(frames.size()) - 1)
        animationIndex = 0;
    else
        animationIndex++;
}

void Model::setPixel(int x, int y, QColor userColor)
{
    getPixel(x, y);

    if (selectColor.red() == 0 && selectColor.green() == 0 && selectColor.blue() == 0 && selectColor.alpha() == 0)
    {
        image->setPixelColor(x, y, userColor);
    }
    else
    {
        QColor blendedColor = blendAdditive(userColor, selectColor);
        image->setPixelColor(x, y, blendedColor); // Apply blended color
    }

    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }

    emit canvasUpdated();
    tracker->setPixelColor(x, y, userColor);
}

void Model::setPixelTracker(int x, int y, QColor userColor)
{
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
    shapeStartX = x;
    shapeStartY = y;
}

void Model::rectangleShape(int x, int y, QColor userColor)
{
    shapePreview->fill(QColor(0, 0, 0, 0));
    QPainter painter(shapePreview);
    QPen pen(userColor);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(std::min(shapeStartX, x),
                     std::min(shapeStartY, y),
                     qAbs(shapeStartX - x),
                     qAbs(shapeStartY - y));
    painter.end();
}

void Model::ellipseShape(int x, int y, QColor userColor)
{
    shapePreview->fill(QColor(0, 0, 0, 0));
    QPainter painter(shapePreview);
    QPen pen(userColor);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawEllipse(std::min(shapeStartX, x),
                        std::min(shapeStartY, y),
                        qAbs(shapeStartX - x),
                        qAbs(shapeStartY - y));
    painter.end();
}

void Model::mergeShapePreview()
{
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
    getPixel(x, y); // Get the color at the pixel the user selected
    QColor colorToReplace = selectColor;
    if (colorToReplace == userColor)
    {
        return;
    }
    paintBucketRecursive(x, y, userColor, colorToReplace);
    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::paintBucketRecursive(int x, int y, QColor userColor, QColor colorToReplace)
{
    if (x >= size || x < 0 || y >= size || y < 0)
    {
        return;
    }
    getPixel(x, y);
    if (selectColor != colorToReplace)
    {
        return;
    }
    image->setPixelColor(x, y, userColor);
    emit canvasUpdated();
    // X
    paintBucketRecursive(x + 1, y, userColor, colorToReplace);
    paintBucketRecursive(x - 1, y, userColor, colorToReplace);

    // Y
    paintBucketRecursive(x, y + 1, userColor, colorToReplace);
    paintBucketRecursive(x, y - 1, userColor, colorToReplace);
}

void Model::erasePixel(int x, int y)
{
    image->setPixelColor(x, y, QColor(0, 0, 0, 0));

    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
    emit canvasUpdated();
}

void Model::getPixel(int x, int y)
{
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
    int redDest = dest.red(), greenDest = dest.green(), blueDest = dest.blue(), alphaDest = dest.alpha();
    int redSrc = src.red(), greenSrc = src.green(), blueSrc = src.blue(), alphaSrc = src.alpha();

    // Compute new alpha
    int alphaOverride = alphaSrc + alphaDest * (255 - alphaSrc) / 255;
    if (alphaOverride == 0)
        return QColor(0, 0, 0, 0); // Fully transparent

    // Compute new RGB values
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
    for (QImage &frame : frames)
    {
        QJsonArray singleFrame;
        for (int y = 0; y < image->width(); ++y)
        {
            for (int x = 0; x < image->width(); ++x)
            {
                QColor color = frame.pixelColor(x, y);
                singleFrame.append(color.red());
                singleFrame.append(color.green());
                singleFrame.append(color.blue());
                singleFrame.append(color.alpha());
            }
        }
        frameData.append(singleFrame);
    }
    json["frames"] = frameData;

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

    QString filePath = QFileDialog::getOpenFileName(nullptr,
                                                    "Load Project",
                                                    "QDir::rootPath()",
                                                    "SSP Files (.ssp);;All Files (*)");
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        // Attempt to open the file
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
            // Extract the width and frameCount from the doc
            QJsonObject jsonObject = doc.object();
            int width = jsonObject["width"].toInt();
            size = width;
            emit requestWindowResize(size);
            int frameCount = jsonObject["frameCount"].toInt();

            QJsonArray jsonFrames = jsonObject["frames"].toArray();

            for (int i = 0; i < frameCount - 1; i++)
            {
                emit requestNewFrame();
            }

            int frameIndex = 0;
            for (const QJsonValue &frameValue : jsonFrames)
            {
                selectFrame(frameIndex);
                QJsonArray frameArray = frameValue.toArray();

                // Loop through the RGBA values for the frame
                for (int i = 0; i < frameArray.size() / 4; i++)
                {
                    int red = frameArray[4 * i].toInt();
                    int green = frameArray[4 * i + 1].toInt();
                    int blue = frameArray[4 * i + 2].toInt();
                    int alpha = frameArray[4 * i + 3].toInt();

                    QColor color(red, green, blue, alpha);
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
