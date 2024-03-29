#include "filterAlgorithms.h"

#include <QFutureSynchronizer>
#include <QPixmap>
#include <QRgb>
#include <QThread>
#include <QtMath>
#include <QtConcurrent/QtConcurrent>

FilterAlgorithms::FilterAlgorithms()
{
}

QImage& FilterAlgorithms::setUpBlurAlgorithm(QImage &img)
{
    int width = img.width();
    int height = img.height();
    createKernel(Settings::kernelRadius);
    int radius = Settings::kernelRadius * 2 + 1;
    auto imageCopy = QImage(img);
    pixels = new QRgb*[height];
    for (int i = 0; i < height; i++) {
        pixels[i] = reinterpret_cast<QRgb *>(imageCopy.scanLine(i));
    }



    int threadsCount = 8;
    QFutureSynchronizer<void> synchronizer;
    auto blurPixelsH = [&](int firstHeight, int lastHeight){
        for (int i = firstHeight; i < lastHeight; i++) {
            for (int j = Settings::kernelRadius; j < width - Settings::kernelRadius; j++) {
                double red = 0;
                double blue = 0;
                double green = 0;
                int indent = Settings::kernelRadius * (-1);
                    for (int k = 0; k < radius; k++) {
                        red += (double)qRed(pixels[i][j + indent])*kernel[k];
                        //red += (double)qRed(pixels[pixelI][pixelJ + indent])*kernel[i];
                        blue += (double)qBlue(pixels[i][j + indent])*kernel[k];
                        green += (double)qGreen(pixels[i][j + indent])*kernel[k];
                        indent++;
                    }
                   // if(red > 255 || red < 0)
                        //qDebug()<< red;
                img.setPixelColor(j, i, qRgb(red, green, blue));
            }
        }
    };
    for (int i = 0; i < threadsCount; i++) {
        if(i == 0)
            synchronizer.addFuture(QtConcurrent::run(blurPixelsH, Settings::kernelRadius, height/threadsCount));
        else if(i == threadsCount - 1)
            synchronizer.addFuture(QtConcurrent::run(blurPixelsH, i * height/threadsCount, height - Settings::kernelRadius));
        else
            synchronizer.addFuture(QtConcurrent::run(blurPixelsH, i * height/threadsCount, (i + 1) * height/threadsCount));
    }
    synchronizer.waitForFinished();
    delete[] pixels;
    imageCopy = QImage(img);
    pixels = new QRgb*[height];
    for (int i = 0; i < height; i++) {
        pixels[i] = reinterpret_cast<QRgb *>(imageCopy.scanLine(i));
    }
    auto blurPixelsW = [&](int firstHeight, int lastHeight){
        for (int i = firstHeight; i < lastHeight; i++) {
            for (int j = Settings::kernelRadius; j < width - Settings::kernelRadius; j++) {
                double red = 0;
                double blue = 0;
                double green = 0;
                int indent = Settings::kernelRadius * (-1);
                for (int k = 0; k < radius; k++) {
                    red += (double)qRed(pixels[i + indent][j])*kernel[k];
                    blue += (double)qBlue(pixels[i + indent][j])*kernel[k];
                    green += (double)qGreen(pixels[i + indent][j])*kernel[k];
                    indent++;
                }
                img.setPixelColor(j, i, qRgb(red, green, blue));
            }
        }
    };
    for (int i = 0; i < threadsCount; i++) {
        if(i == 0)
            synchronizer.addFuture(QtConcurrent::run(blurPixelsW, Settings::kernelRadius, height/threadsCount));
        else if(i == threadsCount - 1)
            synchronizer.addFuture(QtConcurrent::run(blurPixelsW, i * height/threadsCount, height - Settings::kernelRadius));
        else
            synchronizer.addFuture(QtConcurrent::run(blurPixelsW, i * height/threadsCount, (i + 1) * height/threadsCount));
    }
    synchronizer.waitForFinished();
    delete[] pixels;
    return img;

//    SubFilterAlgorithms *subAlgorithms = new SubFilterAlgorithms[threadsCount];
//    QThread *threads = new QThread[threadsCount];
//    for (int i = 0; i < 5; i++) {
//        subAlgorithms[i].moveToThread(&threads[i]);
//        if(i == 0)
//            subAlgorithms[i].setUpInfo(kernel, radius, pixels, img, Settings::kernelRadius, height/5, width - Settings::kernelRadius);
//        else if(i == 4)
//            subAlgorithms[i].setUpInfo(kernel, radius, pixels, img, i * height/5 + 1, height - Settings::kernelRadius, width - Settings::kernelRadius);
//        else
//            subAlgorithms[i].setUpInfo(kernel, radius, pixels, img, i * height/5 + 1, (i + 1) * height/5, width - Settings::kernelRadius);
//        connect(&threads[i], SIGNAL(started()), &subAlgorithms[i], SLOT(gausAlgorithm()));
//        threads[i].start();
//    }
//    for (int i = Settings::kernelRadius; i < height - Settings::kernelRadius; i++) {
//        for (int j = Settings::kernelRadius; j < width - Settings::kernelRadius; j++) {
//            img.setPixelColor(j, i, getBlurPixelColor(radius, i, j, true));
//            img.setPixelColor(j, i, getBlurPixelColor(radius, i, j, false));
//        }
//    }
}

QImage &FilterAlgorithms::setUpSobelAlgorithm(QImage &img)
{
    int width = img.width();
    int height = img.height();
    auto imageCopy = QImage(img);
    pixels = new QRgb*[height];
    for (int i = 0; i < height; i++) {
        pixels[i] = reinterpret_cast<QRgb *>(imageCopy.scanLine(i));
    }
    int threadsCount = 8;
    auto makeImgGray = [&](int firstHeight, int lastHeight){  //Comment it
        for (int i = firstHeight; i < lastHeight; i++) {      // i = 0; i < height
            for (int j = 0; j < width; j++) {
                int red = qRed(pixels[i][j]);
                int green = qGreen(pixels[i][j]);
                int blue = qBlue(pixels[i][j]);
                double gray = ((double)red * 0.3 + (double)green * 0.59 + (double)blue * 0.11)/3.0;
                pixels[i][j] = qRgb(gray, gray, gray);
            }
        }
    };       //Comment it
    //int threshold = 0;
    QFutureSynchronizer<void> synchronizer;
    for (int i = 0; i < threadsCount; i++) {          //Comment it
        if(i != threadsCount - 1)                     //Comment it
            synchronizer.addFuture(QtConcurrent::run(makeImgGray, i * height/threadsCount, (i + 1) * height/threadsCount));      //Comment it
        else
            synchronizer.addFuture(QtConcurrent::run(makeImgGray, i * height/threadsCount, height));      //Comment it
    }       //Comment it
    synchronizer.waitForFinished();          //Comment it

    auto sobelPixels = [&](int firstHeight, int lastHeight){
        for (int i = firstHeight; i < lastHeight; i++) {
            for (int j = 1; j < width - 1; j++) {
                int redGx = qRed(pixels[i + 1][j - 1]) + qRed(pixels[i + 1][j])* 2 + qRed(pixels[i + 1][j + 1]) -
                        qRed(pixels[i - 1][j - 1]) - qRed(pixels[i - 1][j])*2 - qRed(pixels[i - 1][j + 1]);
                //checkForOverflow(redGx);
                int blueGx = qBlue(pixels[i + 1][j - 1]) + qBlue(pixels[i + 1][j])* 2 + qBlue(pixels[i + 1][j + 1]) -
                        qBlue(pixels[i - 1][j - 1]) - qBlue(pixels[i - 1][j])*2 - qBlue(pixels[i - 1][j + 1]);
               // checkForOverflow(blueGx);
                int greenGx = qGreen(pixels[i + 1][j - 1]) + qGreen(pixels[i + 1][j])* 2 + qGreen(pixels[i + 1][j + 1]) -
                        qGreen(pixels[i - 1][j - 1]) - qGreen(pixels[i - 1][j])*2 - qGreen(pixels[i - 1][j + 1]);
               // checkForOverflow(greenGx);

                int redGy = qRed(pixels[i - 1][j + 1]) + qRed(pixels[i][j + 1])* 2 + qRed(pixels[i + 1][j + 1]) -
                        qRed(pixels[i - 1][j - 1]) - qRed(pixels[i][j - 1])* 2 - qRed(pixels[i + 1][j - 1]);
               // checkForOverflow(redGy);
                int blueGy = qBlue(pixels[i - 1][j + 1]) + qBlue(pixels[i][j + 1])* 2 + qBlue(pixels[i + 1][j + 1]) -
                        qBlue(pixels[i - 1][j - 1]) - qBlue(pixels[i][j - 1])* 2 - qBlue(pixels[i + 1][j - 1]);
               // checkForOverflow(blueGy);
                int greenGy = qGreen(pixels[i - 1][j + 1]) + qGreen(pixels[i][j + 1])* 2 + qGreen(pixels[i + 1][j + 1]) -
                        qGreen(pixels[i - 1][j - 1]) - qGreen(pixels[i][j - 1])* 2 - qGreen(pixels[i + 1][j - 1]);
                //checkForOverflow(greenGy);

                int red = sqrt(pow(redGx, 2) + pow(redGy, 2));
                int green = sqrt(pow(greenGx, 2) + pow(greenGy, 2));
                int blue = sqrt(pow(blueGx, 2) + pow(blueGy, 2));

                img.setPixelColor(j, i, qRgb(red, green, blue));
            }
        }
    };
    for (int i = 0; i < threadsCount; i++) {
        if(i == 0)
            synchronizer.addFuture(QtConcurrent::run(sobelPixels, 1, height/threadsCount));
        else if(i == threadsCount - 1)
            synchronizer.addFuture(QtConcurrent::run(sobelPixels, i * height/threadsCount, height - Settings::kernelRadius));
        else
            synchronizer.addFuture(QtConcurrent::run(sobelPixels, i * height/threadsCount, (i + 1) * height/threadsCount));
    }
    synchronizer.waitForFinished();
    delete[] pixels;
    return img;
}

void FilterAlgorithms::createKernel(int r)
{
    double sigma = r/2.0;
    int kernelRadius = r * 2 + 1;
    //double sigma = kernelRadius/2.0;
    double sum = 0;
    kernel = new double[kernelRadius];
    for (int i = 0; i < kernelRadius; i++) {
        kernel[i] = exp(-(i-r)*(i-r) / (2 * pow(sigma, 2))) / sqrt(2.0 * M_PI);
        sum += kernel[i];
       // qDebug() << kernel[i];
    }
    for (int i = 0; i < kernelRadius; i++) {
        kernel[i] /= sum;
        //qDebug() << kernel[i];
    }

//    switch (r) {
//    case 1: {
//        //double temp[] = {0.1, 0.8, 0.1};
//        kernel = new double[3];
//        kernel[0] = 0.1;
//        kernel[1] = 0.8;
//        kernel[2] = 0.1;
//        break;
//    }
//        //return temp;
//    case 2:{
//        //double temp[] = {0.08, 0.08, 0.68, 0.08, 0.08};
//        kernel = new double[5];
//        kernel[0] = 0.08;
//        kernel[1] = 0.08;
//        kernel[2] = 0.68;
//        kernel[3] = 0.08;
//        kernel[4] = 0.08;
//        break;
//    }
//    case 3:{
//        //double temp[] = {0.07, 0.07, 0.07, 0.58, 0.07, 0.07, 0.07};
//        kernel = new double[7];
//        kernel[0] = 0.09;
//        kernel[1] = 0.09;
//        kernel[2] = 0.09;
//        kernel[3] = 0.46;
//        kernel[4] = 0.09;
//        kernel[5] = 0.09;
//        kernel[6] = 0.09;
//        break;
////    case 10:{
////        kernel = new double[21];
////        kernel[0] = 0.03;
////        kernel[1] = 0.03;
////        kernel[2] = 0.03;
////        kernel[3] = 0.03;
////        kernel[4] = 0.03;
////        kernel[5] = 0.03;
////        kernel[6] = 0.04;
////        kernel[7] = 0.04;
////        kernel[8] = 0.06;
////        kernel[9] = 0.08;
////        kernel[10] = 0.2;
////        kernel[11] = 0.08;
////        kernel[12] = 0.06;
////        kernel[13] = 0.04;
////        kernel[14] = 0.04;
////        kernel[15] = 0.03;
////        kernel[16] = 0.03;
////        kernel[17] = 0.03;
////        kernel[18] = 0.03;
////        kernel[19] = 0.03;
////        kernel[20] = 0.03;
////        break;
////        }
//    }
//    default:{
//        double temp[] = {0.1, 0.8, 0.1};
//        kernel = temp;
//        break;
//    }
//    }
}

QRgb FilterAlgorithms::getBlurPixelColor(int kernelRadius, int pixelI, int pixelJ, bool lineMode)
{
    int red = 0;
    int blue = 0;
    int green = 0;
    int indent = Settings::kernelRadius * (-1);
    if(lineMode){
        for (int i = 0; i < kernelRadius; i++) {
            red += (double)qRed(pixels[pixelI][pixelJ + indent] * kernel[i]);
            //red += (double)qRed(pixels[pixelI][pixelJ + indent])*kernel[i];
            blue += (double)qBlue(pixels[pixelI][pixelJ + indent])*kernel[i];
            green += (double)qGreen(pixels[pixelI][pixelJ + indent])*kernel[i];
            indent++;
        }
        return qRgb(red, green, blue);
    }
    for (int i = 0; i < kernelRadius; i++) {
        red += (double)qRed(pixels[pixelI + indent][pixelJ])*kernel[i];
        blue += (double)qBlue(pixels[pixelI + indent][pixelJ])*kernel[i];
        green += (double)qGreen(pixels[pixelI + indent][pixelJ])*kernel[i];
        indent++;
    }
    return qRgb(red, green, blue);
}

void FilterAlgorithms::checkForOverflow(int &pixelColor)
{
    if(pixelColor < 0)
        pixelColor = 0;
    else if(pixelColor > 255)
        pixelColor = 255;
}



