#include "confirmpage.h"
#include "ui_confirmpage.h"
#include <QMessageBox>
#include <userpage.h>
confirmPage::confirmPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::confirmPage)
{
    ui->setupUi(this);
    setFixedSize(800,750);
    setWindowTitle("Done");
    QPixmap logo(":/Logo/Resources/logo.png");
    ui->logo->setPixmap(logo.scaled(170,40,Qt::KeepAspectRatio));
    bookingdb=QSqlDatabase::addDatabase("QSQLITE");
    bookingdb.setDatabaseName("booking.db");
    bookingdb.open();

}

void confirmPage:: receiveseat(QString row,QString column)
{
    QSqlQuery getData(bookingdb);
    getData.prepare("SELECT * FROM BOOKING WHERE ROW=:row AND COLUMN=:column");
    getData.bindValue(":row",row);
    getData.bindValue(":column",column);
    getData.exec();
    while(getData.next())
    {
        ui->From->setText(getData.value("FROM").toString());
        ui->Destination->setText(getData.value("DESTINATION").toString());
        ui->Date->setText(getData.value("DATE").toString());
        ui->Time->setText(getData.value("TIME").toString());
        ui->Price->setText(getData.value("PRICE").toString());
        ui->Firstname->setText(getData.value("FIRSTNAME").toString());
        ui->Lastname->setText(getData.value("LASTNAME").toString());
        ui->Phone->setText(getData.value("PHONE").toString());
        ui->Email->setText(getData.value("EMAIL").toString());
        QString seat = QString("%1%2").arg(char('A' + getData.value("ROW").toInt())).arg(getData.value("COLUMN").toInt()+1);
        ui->Seat->setText(seat);
    }
}
confirmPage::~confirmPage()
{
    delete ui;
}

void confirmPage::on_Confirm_clicked()
{
    bookingdb.close();
    QMessageBox::information(this, "Booking", "Please proceed to the reception desk and make the necessary payment. Thankyou");
    userPage *homePage=new class userPage;
    this->hide();
    homePage->show();
}

