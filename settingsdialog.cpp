#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

#include <QSqlQuery>
#include <QSqlResult>
#include "savetodbdialog.h"
#include "loadfromdb.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->KernelRadiusBox->setValue(Settings::kernelRadius);
//    int index = 0;
//    if(Settings::filter == Settings::Filters::Gaus)
//        index = 1;
//    else if (Settings::filter == Settings::Filters::Sobel)
//        index = 2;
 //   ui->FilterBox->setCurrentIndex(index);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_Ok_button_clicked()
{
    Settings::kernelRadius = ui->KernelRadiusBox->value();
    //auto selectedFilter = ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
//    switch (ui->FilterBox->currentIndex()) {
//    case 0:{
//        Settings::filter = Settings::Filters::None;
//        break;
//    }
//    case 1:{
//        Settings::filter = Settings::Filters::Gaus;
//        break;
//    }
//    case 2:{
//        Settings::filter = Settings::Filters::Sobel;
//        break;
//    }
//    default:
//        Settings::filter = Settings::Filters::None;
//    }
    this->close();
}

void SettingsDialog::on_SaveToDbButton_clicked()
{
    SaveToDbDialog* saveToDb = new SaveToDbDialog(this);
    connect(saveToDb, SIGNAL(saveToDb(QString, QString)),
            this, SLOT(saveToDb(QString, QString)));
    saveToDb->show();
    saveToDb->exec();
}

void SettingsDialog::on_LoadFromDbButton_clicked()
{
    auto db = static_cast<MainWindow*>(parent())->getDb();
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("Select \"Id\", \"Configurations\".\"Name\", \"Description\", \"Configurations\".\"AttrId\", \"Value\" from \"MyScheme\".\"Configurations\" "
                "join \"MyScheme\".\"Attributes\" on \"Configurations\".\"AttrId\" = \"Attributes\".\"AttrId\"");
    QVector<QVector<QVariant>> configurations;
    int i = 0;
    while(query->next()){
        configurations.push_back(QVector<QVariant>());
        for (int j = 0; j < 5; j++) {
            configurations[i].push_back(query->value(j));
        }
        i++;
    }
   // Settings::kernelRadius = query->value(0).toInt();
    //ui->KernelRadiusBox->setValue(Settings::kernelRadius);

    LoadFromDb* loadFromDb = new LoadFromDb(this);
    loadFromDb->setInfoForList(configurations);
    connect(loadFromDb, SIGNAL(changeRadius(int)), this, SLOT(onRadiusPicked(int)));
    loadFromDb->show();
    loadFromDb->exec();
    delete query;
}

void SettingsDialog::saveToDb(QString name, QString desc)
{
    auto db = static_cast<MainWindow*>(parent())->getDb();
    QSqlQuery *query = new QSqlQuery(db);
    //QString request = QString("INSERT INTO") + "\"" + "Attributes (Name, Value) VALUES (?, ?)";
    query->prepare("INSERT INTO \"MyScheme\".\"Attributes\" (\"Name\", \"Value\") "
                   "VALUES (?, ?)");
   // query->exec("Insert into MyScheme.Attributes (Name, Value) values (123, 3);");
   // query->addBindValue()
    query->addBindValue("radius");
    query->addBindValue(ui->KernelRadiusBox->value());
    query->exec();
    query->exec("Select max(\"AttrId\") from \"MyScheme\".\"Attributes\"");
    query->next();
    int maxAttrId = query->value(0).toInt();
    query->prepare("Insert into \"MyScheme\".\"Configurations\" (\"Name\", \"Description\", \"AttrId\") values (?, ?, ?)");
    query->addBindValue(name);
    query->addBindValue(desc);
    query->addBindValue(maxAttrId);
    query->exec();
    delete query;
   // close();
}

void SettingsDialog::onRadiusPicked(int radius)
{
    ui->KernelRadiusBox->setValue(radius);
}
