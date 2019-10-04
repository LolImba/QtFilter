#include "loadfromdb.h"
#include "ui_loadfromdb.h"
#include "settings.h"

LoadFromDb::LoadFromDb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadFromDb)
{
    ui->setupUi(this);
    //QVector<QVector<QVariant>> table;
   // ui->ConfigurationsList.
}

LoadFromDb::~LoadFromDb()
{
    delete ui;
}

void LoadFromDb::setInfoForList(QVector<QVector<QVariant> > &table)
{
    if(table.size() > 0 && table[0].size() > 0){
        this->table = table;
        for(auto &row : table){
            QString name = row[1].toString();
            QString desc = row[2].toString();
            QString value = row[4].toString();
            ui->ConfigurationsList->addItem("Name: " + name + " Description: " + desc + " Kernel Radius " + value);
        }
    }
}

void LoadFromDb::on_ConfigurationsList_itemClicked(QListWidgetItem *item)
{
   // Settings::kernelRadius = table[ui->ConfigurationsList->currentRow()][4].toInt();
}


void LoadFromDb::on_Load_clicked()
{
    if(table.size() > 0 && table[0].size() > 0 && table[ui->ConfigurationsList->currentRow()][4].toInt()){
        Settings::kernelRadius = table[ui->ConfigurationsList->currentRow()][4].toInt();
        changeRadius(Settings::kernelRadius);
        close();
    }
}

void LoadFromDb::on_Cancel_clicked()
{
    this->close();
}
