#ifndef VIDEOWIDGETSURFACE_H
#define VIDEOWIDGETSURFACE_H

#include <QAbstractVideoSurface>
#include <QImage>
#include <QRect>
#include <QVideoFrame>
#include <QLabel>
#include <filterAlgorithms.h>

class myQAbstractVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    myQAbstractVideoSurface(QObject *parent = nullptr);
    void stop();
    QPixmap& getImage();
    void setLocationForImage(QLabel &label);
Q_SIGNALS:
    void fnSurfaceStopped(QPixmap pix);
    void signalFrameChanged(QPixmap pix);

private slots:
    void fnClearPixmap();

private:
    QImage::Format imageFormat;
    QPixmap imageCaptured;
    QRect targetRect;
    QRect sourceRect;
    QVideoFrame currentFrame;
    FilterAlgorithms* gausAlgorithm;
    QLabel* locationForImage;

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;

    bool start(const QVideoSurfaceFormat &format);


    bool present(const QVideoFrame &frame);

    QRect videoRect() const { return targetRect; }
};

#endif
