#include "saveConfigdialog.h"
#include "ui_configdialog.h"

SaveConfigDialog::SaveConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
}

SaveConfigDialog::~SaveConfigDialog()
{
    delete ui;
}
