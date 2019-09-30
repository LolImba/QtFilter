#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_Ok_button_clicked();

    void on_SaveToDbButton_clicked();

    void on_LoadFromDbButton_clicked();

    void saveToDb(QString name, QString desc);
    void onRadiusPicked(int radius);


private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
