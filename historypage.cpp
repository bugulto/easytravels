#include "historypage.h"
#include "ui_historypage.h"
#include <userpage.h>
#include<qpushbutton.h>
QString username1;
historyPage::historyPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::historyPage)
{

    ui->setupUi(this);
    setWindowTitle("History");
    setFixedSize(800, 650);
    QPixmap logo(":/Logo/Resources/purchase.png");
    ui->Logo->setPixmap(logo.scaled(45,45,Qt::KeepAspectRatio));
    QPixmap noresults1(":/Logo/Resources/noresults.png");
    ui->noresults1->setPixmap(noresults1.scaled(155,155,Qt::KeepAspectRatio));
    QPixmap noresults2(":/Logo/Resources/noresults.png");
    ui->noresults2->setPixmap(noresults2.scaled(155,155,Qt::KeepAspectRatio));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Done->setIcon(QIcon(QPixmap(":/Logo/Resources/done.png")));
    fillTable();
}
void historyPage::receiveUsername(QString Username)
{
    username1=Username;
    fillTable();
}
void historyPage:: fillTable()
{
    //ui->tableWidget->clearContents();
    //ui->tableWidget_2->clearContents();
    bookingdb=QSqlDatabase::addDatabase("QSQLITE");
    bookingdb.setDatabaseName("booking.db");
    if(bookingdb.open()) qDebug()<<"chalyo";
    int rowNumber1=0,rowNumber2=0;
    QSqlQuery getData(bookingdb);
    getData.prepare("SELECT * FROM BOOKING WHERE USERNAME=:username");
    getData.bindValue(":username",username1);
    getData.exec();
    while(getData.next())
    {
        QString busid=getData.value("BUSID").toString();
        QString from=getData.value("FROM").toString();
        QString destination=getData.value("DESTINATION").toString();
        QString price= getData.value("PRICE").toString();
        QString seat = QString("%1%2").arg(char('A' + getData.value("ROW").toInt())).arg(getData.value("COLUMN").toInt()+1);
        QString date= getData.value("DATE").toString();
        QString time= getData.value("TIME").toString();
        QDate busDate=QDate::fromString(date, "yyyy-MM-dd");
        QTime busTime = QTime::fromString(time, "hh:mm");
        QDateTime busDateTime(busDate,busTime);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        if(busDateTime>currentDateTime)
        {
            ui->tableWidget->setRowCount(rowNumber1+1);
            ui->tableWidget->setItem(rowNumber1, 0, new QTableWidgetItem(busid));
            ui->tableWidget->setItem(rowNumber1, 1, new QTableWidgetItem(from));
            ui->tableWidget->setItem(rowNumber1, 2, new QTableWidgetItem(destination));
            ui->tableWidget->setItem(rowNumber1, 3, new QTableWidgetItem(date));
            ui->tableWidget->setItem(rowNumber1, 4, new QTableWidgetItem(time));
            ui->tableWidget->setItem(rowNumber1, 5, new QTableWidgetItem(price));
            ui->tableWidget->setItem(rowNumber1, 6, new QTableWidgetItem(seat));

            QPushButton *cancelButton = new QPushButton("Cancel", ui->tableWidget);
            cancelButton->setStyleSheet("QPushButton {"
                                        "border-radius: 10px;"
                                        "padding: 10px;"
                                        "background-color: rgb(255, 102, 102);"
                                        "}");
            cancelButton->setIcon(QIcon(QPixmap(":/Logo/Resources/delete.png")));
            QString row = getData.value("ROW").toString();
            QString column = getData.value("COLUMN").toString();
            connect(cancelButton, &QPushButton::clicked, this, [this, row, column]() {
                oncancelButtonClicked(row, column);
            });
            ui->tableWidget->setCellWidget(rowNumber1, 7, cancelButton);

            for (int col = 0; col < 7; col++)
            {
                ui->tableWidget->item(rowNumber1, col)->setTextAlignment(Qt::AlignCenter);
            }
            rowNumber1++;
        }
        else
        {
            qDebug()<<busid<<from<<destination<<price<<date<<time<<seat;
            qDebug() << "busDateTime:" << busDateTime;
            qDebug() << "currentDateTime:" << currentDateTime;
                ui->tableWidget_2->setRowCount(rowNumber2+1);
            ui->tableWidget_2->setItem(rowNumber2, 0, new QTableWidgetItem(busid));
            ui->tableWidget_2->setItem(rowNumber2, 1, new QTableWidgetItem(from));
            ui->tableWidget_2->setItem(rowNumber2, 2, new QTableWidgetItem(destination));
            ui->tableWidget_2->setItem(rowNumber2, 3, new QTableWidgetItem(date));
            ui->tableWidget_2->setItem(rowNumber2, 4, new QTableWidgetItem(time));
            ui->tableWidget_2->setItem(rowNumber2, 5, new QTableWidgetItem(price));
            ui->tableWidget_2->setItem(rowNumber2, 6, new QTableWidgetItem(seat));

            for (int col = 0; col < 7; col++)
            {
                ui->tableWidget_2->item(rowNumber2, col)->setTextAlignment(Qt::AlignCenter);
            }
            rowNumber2++;
        }
    }
    if(rowNumber1==0)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
         ui->stackedWidget->setCurrentIndex(0);
    }
    if(rowNumber2==0)
    {
        ui->stackedWidget_2->setCurrentIndex(1);
    }
    else
    {
        ui->stackedWidget_2->setCurrentIndex(0);
    }

}

historyPage::~historyPage()
{
    delete ui;
}

void historyPage::oncancelButtonClicked(QString row,QString column)
{
    bookingdb=QSqlDatabase::addDatabase("QSQLITE");
    bookingdb.setDatabaseName("booking.db");
    bookingdb.open();
    QSqlQuery delBus(bookingdb);
    delBus.prepare("DELETE FROM BOOKING WHERE ROW=:row AND COLUMN=:column");
    delBus.bindValue(":row",row);
    delBus.bindValue(":column",column);
    delBus.exec();
    bookingdb.close();
    fillTable();
}

void historyPage::on_Done_clicked()
{
    bookingdb.close();
    this->hide();
}

