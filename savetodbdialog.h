#ifndef SAVETODBDIALOG_H
#define SAVETODBDIALOG_H

#include <QDialog>

namespace Ui {
class SaveToDbDialog;
}

class SaveToDbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveToDbDialog(QWidget *parent = nullptr);
    ~SaveToDbDialog();

Q_SIGNALS:
    void saveToDb(QString name, QString desc);


private slots:

    void on_Save_clicked();

    void on_Cancel_clicked();

private:
    Ui::SaveToDbDialog *ui;
};

#endif // SAVETODBDIALOG_H
