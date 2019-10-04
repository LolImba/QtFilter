#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class SaveConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();


private:
    Ui::SaveConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
