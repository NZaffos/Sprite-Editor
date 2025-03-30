#include "models.h"
#include "qpixmap.h"
#include "QTimer"
#include "QJsonObject"
#include "QJsonArray"
#include "QFileDialog"
#include "QJsonDocument"
#include "QMessageBox"

Model::Model(QObject *parent) : QObject(parent)
{
    image = new QImage(size, size, QImage::Format_ARGB32);
    clearCanvas();

    frames.push_back(*image);
    updateAnimationFrame();
}

Model::~Model()
{
    delete image;
}

QImage *Model::getImage()
{
    return image;
}

void Model::clearCanvas()
{
    image->fill(QColor(0, 0, 0, 50));
    if (frames.size() > 0)
        frames[currentFrameIndex] = *image;
    emit canvasUpdated();
}

void Model::addFrame()
{
    QImage newFrame(size, size, QImage::Format_ARGB32);
    newFrame.fill(QColor(0, 0, 0, 50));

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

    emit frameSelected(currentFrameIndex);
    emit canvasUpdated();
}

void Model::selectFrame(unsigned int index)
{
    if (index < frames.size())
    {
        *image = frames[index];
        currentFrameIndex = index;
        emit frameSelected(index);
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
    emit frameSelected(currentFrameIndex);
    emit canvasUpdated();
}

void Model::sliderValueChanged(int value)
{
    animationFps = value;
    emit updateFpsSliderIO(value);
}

void Model::toggleAnimation(){
    if(animationPlaying) {
        animationPlaying = false;
        emit togglePlayPauseButtonIcon(false);
    }
    else {
        animationPlaying = true;
        updateAnimationFrame();
        emit togglePlayPauseButtonIcon(true);
    }
}

void Model::updateAnimationFrame()
{
    if(!animationPlaying)
        return;

    emit updateAnimationIcon(animationIndex);

    if (animationIndex >= static_cast<int>(frames.size()) - 1)
        animationIndex = 0;
    else
        animationIndex++;
    QTimer::singleShot(1000 / animationFps, this, [this]()
                       { updateAnimationFrame(); });
}

// #include "tools.h"

// void Tools::paintBucket(QImage& image, int x, int y, QColor newColor) {
//     // Implement paint bucket logic
// }

// void Tools::drawRectangle(QImage& image, QRect rect, QColor color) {
//     // Implement drawing rectangle logic
// }

// void Tools::rotateImage(QImage& image, int angle) {
//     // Implement image rotation logic
// }

void Model::setPixel(int x, int y, QColor userColor)
{
    getPixel(x, y);

    // image->setPixelColor(x, y, userColor);

    // if the pixel selected is empty
    if (selectColor.red() == 0 && selectColor.green() == 0 && selectColor.blue() == 0 && selectColor.alpha() == 255)
    {
        // qDebug() << "Pixel is empty";
        image->setPixelColor(x, y, userColor);
    }
    else
    {
        QColor blendedColor = blendAdditive(userColor, selectColor);
        image->setPixelColor(x, y, blendedColor); // Apply blended color
    }

    // qDebug() << (selectColor == QColor(0, 0, 0, 255));

    // qDebug() << "color is: " << "red: " << userColor.red() << "green: " << userColor.green() << "blue: " << userColor.blue() << "alpha: " << userColor.alpha();

    if (currentFrameIndex < frames.size())
    {
        frames[currentFrameIndex] = *image;
    }
}

void Model::erasePixel(int x, int y)
{
    image->setPixelColor(x, y, QColor(0, 0, 0, 50));
}

void Model::getPixel(int x, int y)
{
    selectColor = image->pixelColor(x, y);
    // qDebug() << "Color at coords: " << selectColor.red() << ", " << selectColor.blue() << ", " << selectColor.green() << ", " << selectColor.alpha();
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

    // qDebug() << "Blended color is: " << "red: " << redOverride << "green: " << greenOverride << "blue: " << blueOverride << "alpha: " << alphaOverride;

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

        // File has been choosen so now we clear.
        frames.clear();
        clearCanvas();
        palette.clear();

        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        if (doc.isObject())
        {
            // Extract the width and frameCount from the doc
            QJsonObject jsonObject = doc.object();
            int width = jsonObject["width"].toInt();
            int frameCount = jsonObject["frameCount"].toInt();

            qDebug() << "Width:" << width << "Frame Count:" << frameCount;
            QJsonArray jsonFrames = jsonObject["frames"].toArray();

            // SET SIZE OF CANVAS HERE
            for (int i = 0; i < frameCount; i++)
            {
                addFrame();
            }

            int frameIndex = 0;
            for (const QJsonValue &frameValue : jsonFrames)
            {
                selectFrame(frameIndex);
                QJsonArray frameArray = frameValue.toArray();

                // Loop through the RGBA values for the frame
                for (int i = 0; i < frameArray.size() / 4; i += 4)
                {
                    int red = frameArray[4 * i].toInt();
                    int green = frameArray[4 * i + 1].toInt();
                    int blue = frameArray[4 * i + 2].toInt();
                    int alpha = frameArray[4 * i + 3].toInt();

                    QColor color(red, green, blue, alpha);
                    setPixel(i % size, i / size, color);
                }
                frameIndex++;
            }
        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "The JSON format is incorrect.");
        }
    }
}
