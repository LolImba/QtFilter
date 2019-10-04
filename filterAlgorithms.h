#ifndef GAUSBLURALGORITHM_H
#define GAUSBLURALGORITHM_H

#include <QObject>
#include <QRgb>
#include <settings.h>

class FilterAlgorithms : public QObject
{
Q_OBJECT
public:
    FilterAlgorithms();
   // GausBlurAlgorithm(QImage &img);
    QImage& setUpBlurAlgorithm(QImage &img);
    QImage& setUpSobelAlgorithm(QImage& img);
    void createKernel(int r);
private:
    int radius;
    //int sobelMatr;
    double* kernel;
    QRgb getBlurPixelColor(int kernelRadius, int pixelI, int pixelJ, bool lineMode);
    QRgb **pixels;
    void checkForOverflow(int& pixelColor);
};

#endif // GAUSBLURALGORITHM_H
