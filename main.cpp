#include "mainwindow.h"
#include <QApplication>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QDebug>
#include <QVideoFrame>
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;

    w.show();

    return a.exec();
}
