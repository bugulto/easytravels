#include "editbus.h"
#include "ui_editbus.h"
#include "adminpage.h"
#include "QMessageBox"
QString busid;
editBus::editBus(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::editBus)
{
    busdb=QSqlDatabase::addDatabase("QSQLITE");
    busdb.setDatabaseName("bus.db");
    busdb.open();
    ui->setupUi(this);
    setWindowTitle("Edit Bus Route");
    setFixedSize(800, 650);
    ui->FromError->setVisible(false);
    ui->DestinationError->setVisible(false);
    ui->PriceError->setVisible(false);
    ui->DateError->setVisible(false);
}

editBus::~editBus()
{
    delete ui;
}

void editBus::receiveBusid(QString receivedbusid)
{
    busid=receivedbusid;
    autoFill(receivedbusid);
}

void editBus::autoFill(QString busid1)
{
    busdb.open();
    QSqlQuery autoFill(busdb);
    autoFill.prepare("SELECT * FROM BUS WHERE BUSID=:busid");
    autoFill.bindValue(":busid", busid1);
    qDebug() << busid1;

    if (autoFill.exec() && autoFill.next()) { // Check if the query executed successfully and fetched a record
        QString from = autoFill.value("FROM").toString();
        QString destination = autoFill.value("DESTINATION").toString();
        QString date = autoFill.value("DATE").toString();
        QDate convertedDate = QDate::fromString(date, "yyyy-MM-dd");
        QString time = autoFill.value("TIME").toString();
        QTime convertedTime = QTime::fromString(time, "HH:mm");
        QString price = autoFill.value("PRICE").toString();
        qDebug() << from << destination << date << time << price;
        ui->From->setText(from);
        ui->Destination->setText(destination);
        ui->Price->setText(price);
        ui->timeEdit->setTime(convertedTime);
        ui->calendarWidget->setSelectedDate(convertedDate);
    } else {
        qDebug() << "Error: Unable to fetch record.";
    }

    busdb.close();
}
void editBus::on_Edit_clicked()
{
    busdb.open();
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

    else if(!QRegularExpression("^[0-9]").match(price).hasMatch())
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
        QSqlQuery editData(busdb);
        editData.prepare("UPDATE BUS SET `FROM` = :from, `DESTINATION` = :destination, DATE = :date, TIME = :time, PRICE = :price WHERE BUSID = :busid");
        editData.bindValue(":from", from);
        editData.bindValue(":destination", destination);
        editData.bindValue(":date", date);
        editData.bindValue(":time", time);
        editData.bindValue(":price", price);
        editData.bindValue(":busid", busid);
        if (editData.exec())
        {
            busdb.close();
            QMessageBox::information(this, "Edit Bus Route", "Success");
            busdb.close();
            adminPage *adminPage = new class adminPage;
            this->hide();
            adminPage->show();
        }
        else
        {
            busdb.close();
            QSqlError error = editData.lastError();
            QMessageBox::warning(this, "Edit Bus Route", "Failed to edit data");
        }
    }
}


void editBus::on_Cancel_clicked()
{
    busdb.close();
    adminPage *adminPage = new class adminPage;
    this->hide();
    adminPage->show();
}

