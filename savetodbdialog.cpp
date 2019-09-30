#include "savetodbdialog.h"
#include "ui_savetodbdialog.h"

SaveToDbDialog::SaveToDbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveToDbDialog)
{
    ui->setupUi(this);
}

SaveToDbDialog::~SaveToDbDialog()
{
    delete ui;
}

void SaveToDbDialog::on_Save_clicked()
{
    saveToDb(ui->NameText->toPlainText(), ui->DescText->toPlainText());
    close();
}

void SaveToDbDialog::on_Cancel_clicked()
{
    this->close();
}
