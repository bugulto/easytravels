#ifndef EDITBUS_H
#define EDITBUS_H

#include <QMainWindow>
#include <QtSql>
namespace Ui {
class editBus;
}

class editBus : public QMainWindow
{
    Q_OBJECT

public:
    explicit editBus(QWidget *parent = nullptr);
    ~editBus();

public slots:
    void receiveBusid(QString busid);

private slots:

    void on_Edit_clicked();

    void on_Cancel_clicked();

    void autoFill(QString busid1);

private:
    Ui::editBus *ui;
    QSqlDatabase busdb;
};

#endif // EDITBUS_H
