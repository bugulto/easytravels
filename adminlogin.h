#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QMainWindow>
#include <QtSql>
namespace Ui {
class adminLogin;
}

class adminLogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminLogin(QWidget *parent = nullptr);
    ~adminLogin();

private slots:
    void on_Show_clicked();

    void on_LogIn_clicked();

    void on_Cancel_clicked();

private:
    Ui::adminLogin *ui;
    QSqlDatabase admindb;
};

#endif // ADMINLOGIN_H
