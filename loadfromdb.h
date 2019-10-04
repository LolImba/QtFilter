#ifndef LOADFROMDB_H
#define LOADFROMDB_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class LoadFromDb;
}

class LoadFromDb : public QDialog
{
    Q_OBJECT

public:
    explicit LoadFromDb(QWidget *parent = nullptr);
    ~LoadFromDb();
    void setInfoForList(QVector<QVector<QVariant>> & table);
Q_SIGNALS:
    void changeRadius(int r);

private slots:
    void on_ConfigurationsList_itemClicked(QListWidgetItem *item);

    void on_Load_clicked();

    void on_Cancel_clicked();

private:
    Ui::LoadFromDb *ui;
    QVector<QVector<QVariant>> table;
};

#endif // LOADFROMDB_H
