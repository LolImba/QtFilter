#include "videowidgetsurface.h"

#include <QtWidgets>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>

myQAbstractVideoSurface::myQAbstractVideoSurface(QObject *parent)
    : QAbstractVideoSurface(parent)
    , imageFormat(QImage::Format_Invalid)
{
    gausAlgorithm = new FilterAlgorithms();
}

QList<QVideoFrame::PixelFormat> myQAbstractVideoSurface::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool myQAbstractVideoSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool myQAbstractVideoSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat = imageFormat;
        QAbstractVideoSurface::start(format);
//        QTimer timer;
//        timer.start();
//        timer.sin
        return true;
    } else {
        return false;
    }
}

void myQAbstractVideoSurface::stop()
{
    QAbstractVideoSurface::stop();
}

QPixmap &myQAbstractVideoSurface::getImage()
{
    return imageCaptured;
}

void myQAbstractVideoSurface::setLocationForImage(QLabel &label)
{
    this->locationForImage = &label;
}

void myQAbstractVideoSurface::fnClearPixmap()
{
   // image = gausAlgorithm->setUpAlgorithm(image);
   // locationForImage->setPixmap(imageCaptured);
   // locationForImage->update();
   imageCaptured = QPixmap();
}

bool myQAbstractVideoSurface::present(const QVideoFrame &frame)
{
//    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
//            || surfaceFormat().frameSize() != frame.size()) {
//        setError(IncorrectFormatError);
//        stop();

//        return false;
//    }
    //else {
//        if(!imageCaptured.isNull()){
//            emit fnSurfaceStopped(imageCaptured);
//        }

        currentFrame = frame;
        if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {

            QImage image(
                    currentFrame.bits(),
                    currentFrame.width(),
                    currentFrame.height(),
                    currentFrame.bytesPerLine(),
                    imageFormat);
            //image = gausAlgorithm->setUpAlgorithm(image);
        //    if(imageCaptured.isNull()){
               // imageCaptured = QPixmap::fromImage(image.copy(image.rect()));
                //image = gausAlgorithm->setUpAlgorithm(image);
                switch (Settings::filter) {
                case Settings::Filters::None:{
                   // locationForImage->setPixmap(QPixmap::fromImage(image));
                    signalFrameChanged(QPixmap::fromImage(image));
                    break;
                }
                case Settings::Filters::Gaus:{
                    //locationForImage->setPixmap(QPixmap::fromImage(gausAlgorithm->setUpBlurAlgorithm(image)));
                    signalFrameChanged(QPixmap::fromImage(gausAlgorithm->setUpBlurAlgorithm(image)));
                    break;
                }
                case Settings::Filters::Sobel:{
                  //  locationForImage->setPixmap(QPixmap::fromImage(gausAlgorithm->setUpSobelAlgorithm(image)));
                    signalFrameChanged(QPixmap::fromImage(gausAlgorithm->setUpSobelAlgorithm(image)));
                    break;
                }
                }
              //  locationForImage->update();
      //      }
            currentFrame.unmap();
        }
    //    emit fnClearPixmap();
        return true;
    }
//}
