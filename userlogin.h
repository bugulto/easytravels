#ifndef USERLOGIN_H
#define USERLOGIN_H
#include <userpage.h>
#include <getdetails.h>
#include <historypage.h>
#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui {
class userLogin;
}
QT_END_NAMESPACE

class userLogin : public QMainWindow
{
    Q_OBJECT

public:
    userLogin(QWidget *parent = nullptr);
    ~userLogin();


private slots:
    void on_Show_clicked();

    void on_LogIn_clicked();

    void on_Register_clicked();

    void on_Cancel_clicked();

    void on_Register_2_clicked();

    void on_Show_2_clicked();

    void on_Admin_clicked();


signals:
    void sendFirstname(const QString &message);
    void sendUsername(const QString &message);

private:
    Ui::userLogin *ui;
    QSqlDatabase userdb;
    userPage *userPage;
    getDetails *getDetails;
    historyPage *historyPage;
};
#endif // USERLOGIN_H
