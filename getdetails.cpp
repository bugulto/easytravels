#include "getdetails.h"
#include "ui_getdetails.h"
#include "userpage.h"
#include <confirmpage.h>
int srow;
int scolumn;
int seatSelected=0;
QString busid1;
QString username;
getDetails::getDetails(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::getDetails)
{
    ui->setupUi(this);
    setFixedSize(1300, 750);
    setWindowTitle("Booking Page");
    takenSeats.clear();
    ui->Firstname->setPlaceholderText("First Name");
    ui->Lastname->setPlaceholderText("Last Name");
    ui->Phoneno->setPlaceholderText("Phone Number");
    ui->Email->setPlaceholderText("Email");
    QPixmap logo(":/Logo/Resources/logo.png");
    ui->logo->setPixmap(logo.scaled(170,40,Qt::KeepAspectRatio));
    ui->FirstnameError->setVisible(false);
    ui->LastnameError->setVisible(false);
    ui->PhonenoError->setVisible(false);
    ui->EmailError->setVisible(false);
    ui->SeatError->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);

}

getDetails::~getDetails()
{
    delete ui;
}

void getDetails:: receiveUsername(QString Username)
{
    username= Username;
}

void getDetails:: receiveBusid(QString Busid)
{
    bookingdb=QSqlDatabase::addDatabase("QSQLITE");
    bookingdb.setDatabaseName("booking.db");
    bookingdb.open();
    busid1=Busid;
    bookingdb.open();
    takenSeats.clear();
    QSqlQuery getData(bookingdb);
    getData.prepare("SELECT ROW, COLUMN FROM BOOKING WHERE BUSID =:busid");
    getData.bindValue(":busid",busid1);
    getData.exec();
    while (getData.next())
    {
        int row = getData.value("ROW").toInt();
        int column = getData.value("COLUMN").toInt();
        qDebug() << "Taken Seat: " << row << column;
        takenSeats.insert(QPoint(row, column));
    }
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 4; ++col)
            {
                if ((row == 2 || row == 3) && (col == 0 || col == 1))
                {
                    continue;
                }
                QPushButton *seatButton = new QPushButton();
                seatButton->setIcon(QIcon(QPixmap(":/Logo/Resources/seat.png")));
                seatButton->setFixedSize(60,60);
                if (takenSeats.contains(QPoint(row, col)))
                {
                    seatButton->setStyleSheet("background-color: rgb(218, 0, 0);");

                    seatButton->setDisabled(true);
                }

                connect(seatButton, &QPushButton::clicked, this, [this, seatButton, row, col]() {
                    onSeatClicked(seatButton, row, col);
                });

                ui->gridLayout->addWidget(seatButton, row, col);
            }
        }
    bookingdb.close();
}

void getDetails::onSeatClicked(QPushButton *clickedButton, int row, int col)
{
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    {
        for (const QPoint &seat : selectedSeats)
        {
            QPushButton *seatButton = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(seat.x(), seat.y())->widget());
            seatButton->setStyleSheet("");  //rgb(255, 255, 204);
            seatButton->setProperty("selected", false);
        }

        if (clickedButton)
        {
            clickedButton->setStyleSheet("background-color: rgb(30, 222, 0);");
            clickedButton->setProperty("selected", true);
            selectedSeats.clear();
            selectedSeats.insert(QPoint(row, col));
        }

        if (!selectedSeats.isEmpty())
        {
            ui->label_3->setVisible(true);
            ui->label_4->setVisible(true);
            const QPoint &seat = *selectedSeats.begin();
            QString seatStr = QString("%1%2").arg(char('A' + seat.x())).arg(seat.y() + 1);
            srow=row;
            scolumn=col;
            ui->label_4->setText(seatStr);
            seatSelected=1;
        }
        else
        {
            seatSelected=0;
        }
    }
}

void getDetails::on_Cancel_clicked()
{
    userPage *userPage= new class userPage;
    this->hide();
    userPage->show();
}


void getDetails::on_Next_clicked()
{
    busdb=QSqlDatabase::addDatabase("QSQLITE");
    busdb.setDatabaseName("bus.db");
    busdb.open();
        int error=0;
    QString from,destination,date,time,price;
    QSqlQuery getData(busdb);
    getData.prepare("SELECT * FROM BUS WHERE BUSID=:busid");
    getData.bindValue(":busid",busid1);
    getData.exec();
    while(getData.next())
    {
        from = getData.value("FROM").toString();
        destination = getData.value("DESTINATION").toString();
        date = getData.value("DATE").toString();
        time = getData.value("TIME").toString();
        price = getData.value("PRICE").toString();
    }
    busdb.close();
    bookingdb=QSqlDatabase::addDatabase("QSQLITE");
    bookingdb.setDatabaseName("booking.db");
    bookingdb.open();
    QString firstname=ui->Firstname->text();
    QString lastname=ui->Lastname->text();
    QString phoneno=ui->Phoneno->text();
    QString email=ui->Email->text();
    if(seatSelected==0)
    {
        ui->SeatError->setVisible(true);
        ui->SeatError->setText("Select a seat");
        error++;
    }
    else
    {
        ui->SeatError->setVisible(false);
    }
    if(firstname.isEmpty())
    {
        ui->Firstname->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FirstnameError->setVisible(true);
        ui->FirstnameError->setText("Please enter your first name");
        error++;
    }
    else if(firstname.length()<3)
    {
        ui->Firstname->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FirstnameError->setVisible(true);
        ui->FirstnameError->setText("Must contain at least 3 alphabet characters");
        error++;
    }
    else if(!QRegularExpression("^[a-zA-Z]{3,}$").match(firstname).hasMatch())
    {
        ui->Firstname->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FirstnameError->setVisible(true);
        ui->FirstnameError->setText("Must contain alphabet characters only");
        error++;
    }
    else
    {
        ui->Firstname->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FirstnameError->setVisible(false);
    }
    if(lastname.isEmpty())
    {
        ui->Lastname->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->LastnameError->setVisible(true);
        ui->LastnameError->setText("Please enter your lastname");
        error++;
    }
    else if(lastname.length()<3)
    {
        ui->Lastname->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->LastnameError->setVisible(true);
        ui->LastnameError->setText("Must contain at least 3 alphabet characters");
        error++;
    }
    else if(!QRegularExpression("^[a-zA-Z]{3,}$").match(lastname).hasMatch())
    {
        ui->Lastname->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->LastnameError->setVisible(true);
        ui->LastnameError->setText("Must contain alphabet characters only");
        error++;
    }
    else
    {
        ui->Lastname->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->LastnameError->setVisible(false);
    }
    if(phoneno.isEmpty())
    {
        ui->Phoneno->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PhonenoError->setVisible(true);
        ui->PhonenoError->setText("Please enter your phone number");
        error++;
    }
    else if(phoneno.length()<10 || phoneno.length()>10)
    {
        ui->Phoneno->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PhonenoError->setVisible(true);
        ui->PhonenoError->setText("Phone Number can only contain 10 digits");
        error++;
    }
    else if (!QRegularExpression("^[0-9]+$").match(phoneno).hasMatch())
    {
        ui->Phoneno->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PhonenoError->setVisible(true);
        ui->PhonenoError->setText("Phone number can only have integers");
        error++;
    }
    else
    {
        ui->Phoneno->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PhonenoError->setVisible(false);
    }
    if(email.isEmpty())
    {
        ui->Email->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->EmailError->setVisible(true);
        ui->EmailError->setText("Please enter your email");
        error++;
    }
    else if(email.length()<10)
    {
        ui->Email->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->EmailError->setVisible(true);
        ui->EmailError->setText("Invalid Email");
        error++;
    }
    else if(QRegularExpression("\\s").match(email).hasMatch())
    {
        ui->Email->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->EmailError->setVisible(true);
        ui->EmailError->setText("Email can't contain whitespaces");
        error++;
    }
    else
    {
        ui->Email->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->EmailError->setVisible(false);
    }
    if(error==0)
    {
        if(bookingdb.open())
        {
            qDebug()<<"ok";
            QSqlQuery insertData(bookingdb);
            insertData.prepare("INSERT INTO BOOKING(USERNAME, BUSID, 'FROM', DESTINATION, 'DATE', 'TIME', PRICE, ROW, COLUMN, FIRSTNAME, LASTNAME, PHONE, EMAIL, PAYMENT)"
                               "VALUES(:USERNAME, :BUSID, :FROM, :DESTINATION, :DATE, :TIME, :PRICE, :ROW, :COLUMN, :FIRSTNAME, :LASTNAME, :PHONE, :EMAIL, :PAYMENT)");
            insertData.bindValue(":USERNAME",username);
            insertData.bindValue(":BUSID",busid1);
            insertData.bindValue(":FROM",from);
            insertData.bindValue(":DESTINATION",destination);
            insertData.bindValue(":DATE",date);
            insertData.bindValue(":TIME",time);
            insertData.bindValue(":PRICE",price);
            insertData.bindValue(":ROW",srow);
            insertData.bindValue(":COLUMN",scolumn);
            insertData.bindValue(":FIRSTNAME",firstname);
            insertData.bindValue(":LASTNAME",lastname);
            insertData.bindValue(":PHONE",phoneno);
            insertData.bindValue(":EMAIL",email);
            insertData.bindValue(":PAYMENT",0);
            if(insertData.exec())
            {
                qDebug()<<"done";
                confirmPage= new class confirmPage;
                connect(this, &getDetails::sendseat, confirmPage, &confirmPage::receiveseat);
                emit sendseat(QString::number(srow),QString::number(scolumn));
                this->hide();
                confirmPage->show();
            }
            else
            {
                insertData.lastError();
            }
        }
    }
}

