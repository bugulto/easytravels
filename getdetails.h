#ifndef GETDETAILS_H
#define GETDETAILS_H

#include <QMainWindow>
#include <QtSql>
#include <QPushButton>
#include <confirmpage.h>

namespace Ui {
class getDetails;
}

class getDetails : public QMainWindow
{
    Q_OBJECT

public:
    explicit getDetails(QWidget *parent = nullptr);
    ~getDetails();

public slots:
    void receiveUsername(QString Username);
    void receiveBusid(QString busid);
    void onSeatClicked(QPushButton *clickedButton, int row, int col);

private slots:
    void on_Cancel_clicked();

    void on_Next_clicked();

signals:
        void sendseat(QString row, QString col);
private:
    Ui::getDetails *ui;
    confirmPage *confirmPage;
    QSqlDatabase bookingdb;
    QSqlDatabase busdb;
    QSet<QPoint> selectedSeats;
    QSet<QPoint> takenSeats;

};

#endif // GETDETAILS_H
