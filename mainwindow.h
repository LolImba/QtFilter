#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QtMultimediaWidgets>
#include "videowidgetsurface.h"

#include <QPixmap>
#include <QSqlDatabase>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase& getDb();

private slots:
//    void onMediaPlayerStop(QPixmap pix);
//    void onMediaStateChanged(QMediaPlayer::MediaStatus status);
    void onFrameChanged(QPixmap pix);

    void on_actionPlay_triggered();

    void on_actionOpen_video_triggered();

    void on_actionPause_triggered();

    void on_actionFilter_Settings_triggered();

    void on_actionOpen_triggered();

    void on_actionUse_filter_triggered();

Q_SIGNALS:
    void fnClearPixmap();

private:
    QSqlDatabase db;
    Ui::MainWindow *ui;
    QMediaPlayer *mplayer;
    myQAbstractVideoSurface *vsurface;
    QProgressBar* bar;
    QVideoWidget* vw;
    QSlider* slider;
    bool blnCanStop=true;
};

#endif // MAINWINDOW_H
