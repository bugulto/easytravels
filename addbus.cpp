#include "addbus.h"
#include "ui_addbus.h"
#include <QMessageBox>
#include <adminpage.h>
addBus::addBus(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::addBus)
{

    busdb=QSqlDatabase::addDatabase("QSQLITE");
    busdb.setDatabaseName("bus.db");
    busdb.open();
    ui->setupUi(this);
    setWindowTitle("Add Bus Route");
    setFixedSize(800, 650);
    ui->BusidError->setVisible(false);
    ui->FromError->setVisible(false);
    ui->DestinationError->setVisible(false);
    ui->PriceError->setVisible(false);
    ui->DateError->setVisible(false);
    ui->Busid->setPlaceholderText("Bus ID");
    ui->From->setPlaceholderText("From");
    ui->Destination->setPlaceholderText("Destination");
    ui->Price->setPlaceholderText("Price");
}

addBus::~addBus()
{
    delete ui;
}



void addBus::on_Add_clicked()
{
    busdb.open();
    QString busid=ui->Busid->text();
    QString from=ui->From->text();
    QString destination=ui->Destination->text();
    QString price=ui->Price->text();
    QDate selectedDate = ui->calendarWidget->selectedDate();
    QString date = selectedDate.toString("yyyy-MM-dd");
    QTime selectedTime = ui->timeEdit->time();
    QString time = selectedTime.toString("hh:mm");
    QDateTime busDateTime(selectedDate,selectedTime);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int error=0;
    QSqlQuery checkQuery(busdb);
    checkQuery.prepare("SELECT BUSID FROM BUS WHERE BUSID = :busid");
    checkQuery.bindValue(":busid", busid);
    checkQuery.exec();
    if(busid.isEmpty())
    {
        ui->Busid->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->BusidError->setVisible(true);
        ui->BusidError->setText("Please enter Bus ID");
        error++;
    }
    else if(!QRegularExpression("^[0-9]+$").match(busid).hasMatch())
    {
        ui->Busid->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->BusidError->setVisible(true);
        ui->BusidError->setText("Must contain integer only");
        error++;
    }
    else if (checkQuery.next())
    {
        ui->Busid->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->BusidError->setVisible(true);
        ui->BusidError->setText("Bus ID already taken");
        error++;
    }
    else
    {
        ui->Busid->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->BusidError->setVisible(false);
    }
    if(from.isEmpty())
    {
        ui->From->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FromError->setVisible(true);
        ui->FromError->setText("Fill the field");
        error++;
    }
    else if(from.length()<3)
    {
        ui->From->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FromError->setVisible(true);
        ui->FromError->setText("Must contain at least 3 alphabet characters");
        error++;
    }
    else if(!QRegularExpression("^[a-zA-Z]{3,}$").match(from).hasMatch())
    {
        ui->From->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FromError->setVisible(true);
        ui->FromError->setText("Must contain alphabet characters only");
        error++;
    }
    else
    {
        ui->From->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FromError->setVisible(false);
    }
    if(destination.isEmpty())
    {
        ui->Destination->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->DestinationError->setVisible(true);
        ui->DestinationError->setText("Fill the field");
        error++;
    }
    else if(destination.length()<3)
    {
        ui->Destination->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->DestinationError->setVisible(true);
        ui->DestinationError->setText("Must contain at least 3 alphanumeric characters");
        error++;
    }
    else if(!QRegularExpression("^[a-zA-Z]").match(destination).hasMatch())
    {
        ui->Destination->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->DestinationError->setVisible(true);
        ui->DestinationError->setText("Must contain alphabet characters only");
        error++;
    }
    else
    {
        ui->Destination->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->DestinationError->setVisible(false);
    }
    if(price.isEmpty())
    {
        ui->Price->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceError->setVisible(true);
        ui->PriceError->setText("Please enter the price");
        error++;
    }

    else if(!QRegularExpression("^[0-9]+$").match(price).hasMatch())
    {
        ui->Price->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceError->setVisible(true);
        ui->PriceError->setText("Must contain integer only");
        error++;
    }
    else
    {
        ui->Price->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceError->setVisible(false);
    }
    if (currentDateTime>busDateTime)
    {
        ui->DateError->setVisible(true);
        ui->DateError->setText("Invalid Date");
        error++;
    }
    else
    {
        ui->DateError->setVisible(false);
    }
    if(error==0)
    {
        QSqlQuery addData(busdb);
        addData.prepare("INSERT INTO BUS(BUSID, `FROM`, `DESTINATION`, DATE, TIME, PRICE) VALUES(:busid, :from, :destination, :date, :time, :price)");
        addData.bindValue(":busid",busid);
        addData.bindValue(":from",from);
        addData.bindValue(":destination",destination);
        addData.bindValue(":date",date);
        addData.bindValue(":time",time);
        addData.bindValue(":price",price);
        if (addData.exec())
        {
            busdb.close();
            QMessageBox::information(this, "Add Bus Route", "Success");
            adminPage *adminPage = new class adminPage;
            this->hide();
            adminPage->show();
        }
        else
        {
            busdb.close();
            QSqlError error = addData.lastError();
            QMessageBox::warning(this, "Add", "Failed to add data");
        }
    }
}


void addBus::on_Cancel_clicked()
{
    busdb.close();
    adminPage *adminPage = new class adminPage;
    this->hide();
    adminPage->show();
}

