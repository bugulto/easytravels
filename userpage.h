#ifndef USERPAGE_H
#define USERPAGE_H

#include <QMainWindow>
#include <QtSql>
#include <getdetails.h>
namespace Ui {
class userPage;
}

class userPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit userPage(QWidget *parent = nullptr);
    ~userPage();

public slots:
    void receiveFirstname(QString Firstname);

private slots:

    void fillTable();

    void onselectButtonClicked(QString Busid);

    void on_Search_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_Clear_clicked();

    void on_Logout_clicked();

    void on_History_clicked();

signals:
    void sendBusid(const QString &message);

private:
    Ui::userPage *ui;
    QSqlDatabase busdb;
    getDetails *getDetails;  
};

#endif // USERPAGE_H
