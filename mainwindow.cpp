#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRgb>
#include <QRgb>
#include <qdebug.h>
#include <filterAlgorithms.h>
#include "settingsdialog.h"
#include <QtSql/QSqlDatabase>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  //,mplayer(parent, QMediaPlayer::VideoSurface)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("MyDatabase");
       db.setHostName("localhost");
       db.setUserName("postgres");
       db.setPassword("123");
       qDebug() << db.open();

    mplayer = new QMediaPlayer();
    //vw = new QVideoWidget(this);
    vsurface = new myQAbstractVideoSurface();
    vsurface->setLocationForImage(*ui->lblActual);
    mplayer->setVideoOutput(vsurface);
    state = LabelState::Nothing;
  //  mplayer->setVideoOutput(vw);
    //this->setCentralWidget(vsurface);
}

MainWindow::~MainWindow()
{
    if(mplayer->state()==QMediaPlayer::PausedState){
        vsurface->stop();
        vsurface->deleteLater();
        mplayer->stop();
    }

    delete ui;
}

//void MainWindow::on_cmdOpenVideoFile_clicked()
//{
//    QFileDialog fd;
//    fd.show();
//    fd.exec();
//    QString strFileName = fd.selectedFiles().at(0);
//    vsurface = new myQAbstractVideoSurface();
//    mplayer->setVideoOutput(vsurface);

//    connect(vsurface, SIGNAL(fnSurfaceStopped(QPixmap)),
//            this, SLOT(onMediaPlayerStop(QPixmap)),Qt::QueuedConnection);

//    connect(mplayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
//            this, SLOT(onMediaStateChanged(QMediaPlayer::MediaStatus)));

//    connect(this, SIGNAL(fnClearPixmap()),
//            vsurface, SLOT(fnClearPixmap()),Qt::QueuedConnection);

//    mplayer->setMedia(QUrl::fromLocalFile(strFileName));
//    QPixmap curImage;
//    curImage.load(strFileName);
//    ui->lblActual->setPixmap(curImage);
//}

//void MainWindow::on_cmdCapture_clicked()
//{
//    emit fnClearPixmap();
//    mplayer->setPosition(ui->sldDuration->value()*1000);
//    //mplayer->setMuted(true);
//    mplayer->play();
//}

//void MainWindow::onMediaPlayerStop(QPixmap pix)
//{
    //ui->lblActual->setPixmap(pix);
   // ui->lblActual->update();
   // emit fnClearPixmap();
   // mplayer.pause();
//}

//void MainWindow::onMediaStateChanged(QMediaPlayer::MediaStatus status)
//{
//    if(status==QMediaPlayer::LoadedMedia)
//    {
//        qDebug()<<"duration="<<mplayer->duration()/1000.0f;
//        ui->sldDuration->setMaximum(static_cast<int>(mplayer->duration()/1000));
//        ui->sldDuration->setMinimum(0);
//    }else{
//        qDebug()<<"media loading";
//    }
//}

//void MainWindow::on_cmdSave_clicked()
//{
//    FilterAlgorithms algorithm;
//    auto img = ui->lblActual->pixmap()->toImage();
//    ui->lblActual->setPixmap(QPixmap::fromImage(algorithm.setUpSobelAlgorithm(img)));
//}

void MainWindow::onFrameChanged(QPixmap pix)
{
     ui->lblActual->setPixmap(pix);
}

void MainWindow::on_actionPlay_triggered()
{
    //emit fnClearPixmap();
    mplayer->setMuted(true);
    if(!mplayer->media().isNull())
        mplayer->play();
}

//void MainWindow::on_actionOpen_video_triggered()
//{
//    QFileDialog fd;
//    fd.show();
//    fd.exec();
//    QString strFileName = fd.selectedFiles().at(0);
//    connect(vsurface, SIGNAL(signalFrameChanged(QPixmap)),
//            this, SLOT(onFrameChanged(QPixmap)));

//    mplayer->setMedia(QUrl::fromLocalFile(strFileName));
//    state = LabelState::Video;
//}

void MainWindow::on_actionPause_triggered()
{
    if(!mplayer->media().isNull())
        mplayer->pause();
}

//void MainWindow::on_actionFilter_Settings_triggered()
//{
//    SettingsDialog* settings = new SettingsDialog(this);
//    settings->show();
//}

QSqlDatabase& MainWindow::getDb()
{
    return db;
}


void MainWindow::on_actionOpen_triggered()
{
    QFileDialog fd;
    fd.show();
    fd.exec();
    QString strFileName = fd.selectedFiles().at(0);
    QPixmap curImage;
    curImage.load(strFileName);
    ui->lblActual->setPixmap(curImage);
    state = LabelState::Image;
}

//void MainWindow::on_actionUse_filter_triggered()
//{
//    FilterAlgorithms algorithm;
//    auto img = ui->lblActual->pixmap()->toImage();

//    switch (Settings::filter) {
//    case Settings::Filters::None:{
//        ui->lblActual->setPixmap(QPixmap::fromImage(img));
//        break;
//    }
//    case Settings::Filters::Gaus:{
//        ui->lblActual->setPixmap(QPixmap::fromImage(algorithm.setUpBlurAlgorithm(img)));
//        break;
//    }
//    case Settings::Filters::Sobel:{
//        ui->lblActual->setPixmap(QPixmap::fromImage(algorithm.setUpSobelAlgorithm(img)));
//        break;
//    }
//    }
//    //ui->lblActual->setPixmap(QPixmap::fromImage(algorithm.setUpSobelAlgorithm(img)));
//}

void MainWindow::on_PlayButton_clicked()
{
    on_actionPlay_triggered();
}

void MainWindow::on_StopButton_clicked()
{
    on_actionPause_triggered();
}

void MainWindow::on_actionGaus_triggered()
{
    switch (state) {
    case LabelState::Nothing:{ return; }
    case LabelState::Image:{
        FilterAlgorithms algorithm;
        auto img = ui->lblActual->pixmap()->toImage();
        ui->lblActual->setPixmap(QPixmap::fromImage(algorithm.setUpBlurAlgorithm(img)));
        return;
    }
    case LabelState::Video:{
        Settings::filter = Settings::Filters::Gaus;
        on_actionPlay_triggered();
        return;
    }
    }
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog* settings = new SettingsDialog(this);
    settings->show();
}

void MainWindow::on_actionSobel_triggered()
{
    switch (state) {
    case LabelState::Nothing:{ return; }
    case LabelState::Image:{
        FilterAlgorithms algorithm;
        auto img = ui->lblActual->pixmap()->toImage();
        ui->lblActual->setPixmap(QPixmap::fromImage(algorithm.setUpSobelAlgorithm(img)));
        return;
    }
    case LabelState::Video:{
        Settings::filter = Settings::Filters::Sobel;
        on_actionPlay_triggered();
        return;
    }
    }
}

void MainWindow::on_actionOpen_Video_triggered()
{
    QFileDialog fd;
    fd.show();
    fd.exec();
    QString strFileName = fd.selectedFiles().at(0);
    connect(vsurface, SIGNAL(signalFrameChanged(QPixmap)),
            this, SLOT(onFrameChanged(QPixmap)));

    mplayer->setMedia(QUrl::fromLocalFile(strFileName));
    state = LabelState::Video;
}
