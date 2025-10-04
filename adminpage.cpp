#include "adminpage.h"
#include "ui_adminpage.h"
#include "addbus.h"
#include "editbus.h"
#include "userlogin.h"
adminPage::adminPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminPage)
{
    busdb=QSqlDatabase::addDatabase("QSQLITE");
    busdb.setDatabaseName("bus.db");
    busdb.open();
    ui->setupUi(this);
    setWindowTitle("Admin");
    setFixedSize(1300, 750);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->LogOut->setIcon(QIcon(QPixmap(":/Logo/Resources/logout.png")));
    ui->Search->setIcon(QIcon(QPixmap(":/Logo/Resources/search.png")));
    ui->Erase->setIcon(QIcon(QPixmap(":/Logo/Resources/erase.png")));
    ui->Add->setIcon(QIcon(QPixmap(":/Logo/Resources/plus.png")));
    ui->Done->setIcon(QIcon(QPixmap(":/Logo/Resources/done.png")));
    ui->Busid->setPlaceholderText("Bus ID");
    ui->BusidError->setVisible(false);
    fillTable();
    fillTable2();
}


adminPage::~adminPage()
{
    delete ui;
}

void adminPage::fillTable()
{
    busdb.open();
        ui->tableWidget->clearContents();
        ui->tableWidget_2->clearContents();
    QString busid,from,destination,date,time,price;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int rowNumber1=0,rowNumber2=0;
    QSqlQuery getData(busdb);
    getData.prepare("SELECT * FROM BUS");
    getData.exec();
    while(getData.next())
    {
        busid=getData.value("BUSID").toString();
        from=getData.value("FROM").toString();
        destination = getData.value("DESTINATION").toString();
        date=getData.value("DATE").toString();
        time=getData.value("TIME").toString();
        price=getData.value("PRICE").toString();
        QDate busDate=QDate::fromString(date, "yyyy-MM-dd");
        QTime busTime = QTime::fromString(time, "hh:mm");
        QDateTime busDateTime(busDate,busTime);
        if(currentDateTime<busDateTime)
        {
                    ui->tableWidget->setRowCount(rowNumber1+1);
            ui->tableWidget->setItem(rowNumber1,0,new QTableWidgetItem(QString(busid)));
            ui->tableWidget->setItem(rowNumber1,1,new QTableWidgetItem(QString(from)));
            ui->tableWidget->setItem(rowNumber1,2,new QTableWidgetItem(QString(destination)));
            ui->tableWidget->setItem(rowNumber1,3,new QTableWidgetItem(QString(date)));
            ui->tableWidget->setItem(rowNumber1,4,new QTableWidgetItem(QString(time)));
            ui->tableWidget->setItem(rowNumber1,5,new QTableWidgetItem(QString(price)));
            QPushButton *deleteButton = new QPushButton("Remove", ui->tableWidget);
            deleteButton->setStyleSheet("QPushButton {"
                                        "border-radius: 10px;"
                                        "padding: 10px;"
                                        "background-color: rgb(255, 102, 102);"
                                        "}" );
            deleteButton->setIcon(QIcon(QPixmap(":/Logo/Resources/delete.png")));
            connect(deleteButton, &QPushButton::clicked, this,[this,busid]() {
                ondeleteButtonClicked(busid);
            });
            ui->tableWidget->setCellWidget(rowNumber1, 6, deleteButton);
            QPushButton *editButton = new QPushButton("Edit", ui->tableWidget);
            editButton->setStyleSheet("QPushButton {"
                                        "   border-radius: 10px;"
                                        "   padding: 10px;"
                                        "   background-color: rgb(160, 255, 101);"
                                        "}");
            editButton->setIcon(QIcon(QPixmap(":/Logo/Resources/edit.png")));
            connect(editButton, &QPushButton::clicked, this,[this,busid]() {
                oneditButtonClicked(busid);
            });
            ui->tableWidget->setCellWidget(rowNumber1, 7, editButton);

            for (int col = 0; col < 6; col++)
            {
                ui->tableWidget->item(rowNumber1, col)->setTextAlignment(Qt::AlignCenter);
            }
            rowNumber1++;
        }
        else
        {
            ui->tableWidget_2->setRowCount(rowNumber2+1);
            ui->tableWidget_2->setItem(rowNumber2,0,new QTableWidgetItem(QString(busid)));
            ui->tableWidget_2->setItem(rowNumber2,1,new QTableWidgetItem(QString(from)));
            ui->tableWidget_2->setItem(rowNumber2,2,new QTableWidgetItem(QString(destination)));
            ui->tableWidget_2->setItem(rowNumber2,3,new QTableWidgetItem(QString(date)));
            ui->tableWidget_2->setItem(rowNumber2,4,new QTableWidgetItem(QString(time)));
            ui->tableWidget_2->setItem(rowNumber2,5,new QTableWidgetItem(QString(price)));
            QPushButton *deleteButton = new QPushButton("Stash", ui->tableWidget_2);
            deleteButton->setStyleSheet("QPushButton {"
                                        "border-radius: 10px;"
                                        "padding: 10px;"
                                        "background-color: rgb(255, 102, 102);"
                                        "}" );
            deleteButton->setIcon(QIcon(QPixmap(":/Logo/Resources/stash.png")));
            connect(deleteButton, &QPushButton::clicked, this,[this,busid]() {
                ondeleteButtonClicked(busid);
            });
            ui->tableWidget_2->setCellWidget(rowNumber2, 6, deleteButton);

            for (int col = 0; col < 6; col++)
            {
                ui->tableWidget_2->item(rowNumber2, col)->setTextAlignment(Qt::AlignCenter);
            }
            rowNumber2++;
        }
    }
    busdb.close();
}

void adminPage::ondeleteButtonClicked(QString busid)
{
    busdb=QSqlDatabase::addDatabase("QSQLITE");
    busdb.setDatabaseName("bus.db");
    if (!busdb.isOpen())
        busdb.open();
    qDebug()<<"bhaixa";
    qDebug()<<busid;
    QSqlQuery delBus(busdb);
    delBus.prepare("DELETE FROM BUS WHERE BUSID=:busid");
    delBus.bindValue(":busid",busid);
    delBus.exec();
    busdb.close();
    fillTable();
}

void adminPage::oneditButtonClicked(QString busid)
{  
    editBus= new class editBus;
    connect(this, &adminPage::sendBusid, editBus, &editBus::receiveBusid);
    emit sendBusid(busid);
    this->hide();
    editBus->show();
}

void adminPage::on_Add_clicked()
{
    addBus *addBus=new class addBus;
    this->hide();
    addBus->show();
}


void adminPage::on_LogOut_clicked()
{
    userLogin *userLogin= new class userLogin;
    this->hide();
    userLogin->show();
}


void adminPage::on_Search_clicked()
{
    ui->tableWidget->clearContents();
    ui->tableWidget_2->clearContents();
    busdb=QSqlDatabase::addDatabase("QSQLITE");
    busdb.setDatabaseName("bus.db");
    busdb.open();
    QString busid=ui->Busid->text();
    QString from,destination,date,time,price;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int rowNumber1=0,rowNumber2=0,error=0;
    ui->tableWidget->setRowCount(10);
    ui->tableWidget_2->setRowCount(10);
    if(busid.isEmpty())
    {
        ui->Busid->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->BusidError->setVisible(true);
        ui->BusidError->setText("Enter Bus ID");
        error++;
    }
    else if(!QRegularExpression("^[0-9]").match(busid).hasMatch())
    {
        ui->Busid->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->BusidError->setVisible(true);
        ui->BusidError->setText("Use Integer only");
        error++;
    }
    else
    {
        ui->Busid->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->BusidError->setVisible(false);
    }
    if(error==0)
    {
        QSqlQuery getData(busdb);
        getData.prepare("SELECT * FROM BUS WHERE BUSID=:busid");
        getData.bindValue(":busid",busid);
        getData.exec();
        while(getData.next())
        {

            from=getData.value("FROM").toString();
            destination = getData.value("DESTINATION").toString();
            date=getData.value("DATE").toString();
            time=getData.value("TIME").toString();
            price=getData.value("PRICE").toString();
            QDate busDate=QDate::fromString(date, "yyyy-MM-dd");
            QTime busTime = QTime::fromString(time, "hh:mm");
            QDateTime busDateTime(busDate,busTime);
            if(currentDateTime<busDateTime)
            {
                ui->tableWidget->setRowCount(rowNumber1+1);
                ui->tableWidget->setItem(rowNumber1,0,new QTableWidgetItem(QString(busid)));
                ui->tableWidget->setItem(rowNumber1,1,new QTableWidgetItem(QString(from)));
                ui->tableWidget->setItem(rowNumber1,2,new QTableWidgetItem(QString(destination)));
                ui->tableWidget->setItem(rowNumber1,3,new QTableWidgetItem(QString(date)));
                ui->tableWidget->setItem(rowNumber1,4,new QTableWidgetItem(QString(time)));
                ui->tableWidget->setItem(rowNumber1,5,new QTableWidgetItem(QString(price)));
                QPushButton *deleteButton = new QPushButton("Remove", ui->tableWidget);
                deleteButton->setStyleSheet("QPushButton {"
                                            "border-radius: 10px;"
                                            "padding: 10px;"
                                            "background-color: rgb(255, 102, 102);"
                                            "}" );
                deleteButton->setIcon(QIcon(QPixmap(":/Logo/Resources/delete.png")));
                connect(deleteButton, &QPushButton::clicked, this,[this,busid]() {
                    ondeleteButtonClicked(busid);
                });
                ui->tableWidget->setCellWidget(rowNumber1, 6, deleteButton);
                QPushButton *editButton = new QPushButton("Edit", ui->tableWidget);
                editButton->setStyleSheet("QPushButton {"
                                          "   border-radius: 10px;"
                                          "   padding: 10px;"
                                          "   background-color: rgb(160, 255, 101);"
                                          "}");
                editButton->setIcon(QIcon(QPixmap(":/Logo/Resources/edit.png")));
                connect(editButton, &QPushButton::clicked, this,[this,busid]() {
                    oneditButtonClicked(busid);
                });
                ui->tableWidget->setCellWidget(rowNumber1, 7, editButton);  

                for (int col = 0; col < 6; col++)
                {
                    ui->tableWidget->item(rowNumber1, col)->setTextAlignment(Qt::AlignCenter);
                }
                rowNumber1++;
            }
            else
            {
                             ui->tableWidget_2->setRowCount(rowNumber2+1);
                ui->tableWidget_2->setItem(rowNumber2,0,new QTableWidgetItem(QString(busid)));
                ui->tableWidget_2->setItem(rowNumber2,1,new QTableWidgetItem(QString(from)));
                ui->tableWidget_2->setItem(rowNumber2,2,new QTableWidgetItem(QString(destination)));
                ui->tableWidget_2->setItem(rowNumber2,3,new QTableWidgetItem(QString(date)));
                ui->tableWidget_2->setItem(rowNumber2,4,new QTableWidgetItem(QString(time)));
                ui->tableWidget_2->setItem(rowNumber2,5,new QTableWidgetItem(QString(price)));
                QPushButton *deleteButton = new QPushButton("Stash", ui->tableWidget_2);
                deleteButton->setStyleSheet("QPushButton {"
                                            "border-radius: 10px;"
                                            "padding: 10px;"
                                            "background-color: rgb(255, 102, 102);"
                                            "}" );
                deleteButton->setIcon(QIcon(QPixmap(":/Logo/Resources/stash.png")));
                connect(deleteButton, &QPushButton::clicked, this,[this,busid]() {
                    ondeleteButtonClicked(busid);
                });
                ui->tableWidget_2->setCellWidget(rowNumber2, 6, deleteButton);

                for (int col = 0; col < 6; col++)
                {
                    ui->tableWidget_2->item(rowNumber2, col)->setTextAlignment(Qt::AlignCenter);
                }
                rowNumber2++;
            }
        }
        if (rowNumber1 == 0 && rowNumber2 == 0)
        {
            ui->Busid->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
            ui->BusidError->setVisible(true);
            ui->BusidError->setText("No Bus found");
        }
    }
}


void adminPage::on_Erase_clicked()
{
    ui->Busid->clear();
    fillTable();
}

void adminPage::fillTable2()
{
    busdb.close();
    ui->tableWidget_3->clearContents();
    int rowNumber=0;
    bookingdb=QSqlDatabase::addDatabase("QSQLITE");
    bookingdb.setDatabaseName("booking.db");
    bookingdb.open();
    QSqlQuery getData(bookingdb);
    getData.prepare("SELECT BUSID,DATE,TIME,USERNAME,FIRSTNAME,LASTNAME,PHONE,EMAIL,ROW,COLUMN,PAYMENT FROM BOOKING");
    getData.exec();
        while(getData.next())
    {
         ui->tableWidget_3->setRowCount(rowNumber+1);
        QString busid=getData.value("BUSID").toString();
        QString username= getData.value("USERNAME").toString();
        QString firstname=getData.value("FIRSTNAME").toString();
        QString  lastname=getData.value("LASTNAME").toString();
        QString phone= getData.value("PHONE").toString();
        QString email= getData.value("EMAIL").toString();
        QString seat = QString("%1%2").arg(char('A' + getData.value("ROW").toInt())).arg(getData.value("COLUMN").toInt()+1);
        QString payment=getData.value("PAYMENT").toString();
        QString date= getData.value("DATE").toString();
        QString time= getData.value("TIME").toString();
        QDate busDate=QDate::fromString(date, "yyyy-MM-dd");
        QTime busTime = QTime::fromString(time, "hh:mm");
        QDateTime busDateTime(busDate,busTime);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        if(busDateTime>currentDateTime)
        {
            if(payment=='0')
            {
            ui->tableWidget_3->setItem(rowNumber,0,new QTableWidgetItem(QString(busid)));
            ui->tableWidget_3->setItem(rowNumber,1,new QTableWidgetItem(QString(username)));
            ui->tableWidget_3->setItem(rowNumber,2,new QTableWidgetItem(QString(firstname)));
            ui->tableWidget_3->setItem(rowNumber,3,new QTableWidgetItem(QString(lastname)));
            ui->tableWidget_3->setItem(rowNumber,4,new QTableWidgetItem(QString(phone)));
            ui->tableWidget_3->setItem(rowNumber,5,new QTableWidgetItem(QString(email)));
            ui->tableWidget_3->setItem(rowNumber,6,new QTableWidgetItem(QString(seat)));
            QTableWidgetItem* checkBoxItem = new QTableWidgetItem();
            checkBoxItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkBoxItem->setCheckState(Qt::Unchecked);
            checkBoxItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_3->setItem(rowNumber, 7, checkBoxItem);
            QPushButton *deleteButton = new QPushButton("Remove", ui->tableWidget_3);
            deleteButton->setStyleSheet("QPushButton {"
                                        "border-radius: 10px;"
                                        "padding: 10px;"
                                        "background-color: rgb(255, 102, 102);"
                                        "}" );
            deleteButton->setIcon(QIcon(QPixmap(":/Logo/Resources/delete.png")));
            QString row=getData.value("ROW").toString();
            QString column= getData.value("COLUMN").toString();
            deleteButton->setIcon(QIcon(QPixmap(":/Logo/Resources/delete.png")));
            connect(deleteButton, &QPushButton::clicked, this, [this, row, column]() {
                ondeleteButtonClicked2(row, column);
            });
            ui->tableWidget_3->setCellWidget(rowNumber, 8, deleteButton);

            for (int col = 0; col < 8; col++)
            {
                ui->tableWidget_3->item(rowNumber, col)->setTextAlignment(Qt::AlignCenter);
            }
            rowNumber++;
            }
        }
        if (busDateTime <= currentDateTime && payment.toInt() == 0)
        {
            QSqlQuery delItem;
            delItem.prepare("DELETE FROM BOOKING WHERE DATE <= :currentDate AND PAYMENT = :payment");
            delItem.bindValue(":currentDate", currentDateTime.toString("yyyy-MM-dd hh:mm:ss"));
            delItem.bindValue(":payment", 0);
            if (delItem.exec())
            {
                qDebug() << "Records deleted successfully.";
            }
            else
            {
                qDebug() << "Error deleting records:" << delItem.lastError().text();
            }
        }
    }
        bookingdb.close();
}

void adminPage::on_Done_clicked()
{
    bookingdb.open();
    int rowCount = ui->tableWidget_3->rowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        QTableWidgetItem* checkBoxItem = ui->tableWidget_3->item(row, 7);

        if (checkBoxItem && checkBoxItem->checkState() == Qt::Checked)
        {
            QString busId = ui->tableWidget_3->item(row, 0)->text();
            QSqlQuery updatePayment(bookingdb);
            updatePayment.prepare("UPDATE BOOKING SET PAYMENT = 1 WHERE BUSID = :busId");
            updatePayment.bindValue(":busId", busId);
            updatePayment.exec();

            ui->tableWidget_3->removeRow(row);
            --rowCount;
            --row;
        }
    }
    bookingdb.close();
}

void adminPage::ondeleteButtonClicked2(QString row,QString column)
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
    fillTable2();
}



