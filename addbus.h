#ifndef ADDBUS_H
#define ADDBUS_H

#include <QMainWindow>
#include <QtSql>
namespace Ui {
class addBus;
}

class addBus : public QMainWindow
{
    Q_OBJECT

public:
    explicit addBus(QWidget *parent = nullptr);
    ~addBus();

private slots:

    void on_Add_clicked();

    void on_Cancel_clicked();

private:
    Ui::addBus *ui;
    QSqlDatabase busdb;
};

#endif // ADDBUS_H
