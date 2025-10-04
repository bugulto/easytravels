#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QMainWindow>
#include <QtSql>
#include <editbus.h>
namespace Ui {
class adminPage;
}

class adminPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminPage(QWidget *parent = nullptr);
    ~adminPage();

private slots:
    void fillTable();
    void fillTable2();
    void ondeleteButtonClicked(QString busid);
    void ondeleteButtonClicked2(QString row,QString column);
    void oneditButtonClicked(QString busid);

    void on_Add_clicked();

    void on_LogOut_clicked();

    void on_Search_clicked();

    void on_Erase_clicked();

    void on_Done_clicked();

signals:
    void sendBusid(QString message);

private:
    Ui::adminPage *ui;
    editBus *editBus;
    QSqlDatabase busdb;
    QSqlDatabase bookingdb;
};

#endif // ADMINPAGE_H
