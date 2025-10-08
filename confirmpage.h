#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QMainWindow>
#include <QtSql>
namespace Ui {
class confirmPage;
}

class confirmPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit confirmPage(QWidget *parent = nullptr);
    ~confirmPage();

public slots:
    void receiveseat(QString row,QString col);

private slots:
    void on_Confirm_clicked();

private:
    Ui::confirmPage *ui;
    QSqlDatabase bookingdb;
};

#endif // CONFIRMPAGE_H
