#include "userpage.h"
#include "ui_userpage.h"
#include "userlogin.h"
#include "QPushButton"
#include "getdetails.h"
#include "QMessageBox"
QString firstname1;
userPage::userPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::userPage)
{
    busdb=QSqlDatabase::addDatabase("QSQLITE");
    busdb.setDatabaseName("bus.db");
    busdb.open();
    ui->setupUi(this);
    ui->stackedWidget_2->setCurrentIndex(0);
    setFixedSize(1300, 750);
    setWindowTitle("Home Page");
    QPixmap logo(":/Logo/Resources/logo.png");
    ui->logo->setPixmap(logo.scaled(170,40,Qt::KeepAspectRatio));
        ui->tableWidget->setRowCount(10);
        ui->tableWidget_2->setRowCount(10);
    ui->FromError->setVisible(false);
    ui->DestinationError->setVisible(false);
    ui->PriceLowError->setVisible(false);
    ui->PriceHighError->setVisible(false);
    ui->DateError->setVisible(false);
    ui->From->setPlaceholderText("Enter Source City (Eg:Dhulikhel)");
    ui->Destination->setPlaceholderText("Enter Destination (Eg:Melamchi)");
    ui->PriceLow->setPlaceholderText("Min Price");
    ui->PriceHigh->setPlaceholderText("Max Price");
    QPixmap bus(":/Logo/Resources/Bus1.png");
    ui->label_6->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->label_6->setPixmap(bus.scaled(45,45,Qt::KeepAspectRatio));
    ui->Search->setIcon(QIcon(QPixmap(":/Logo/Resources/search.png")));
    ui->Clear->setIcon(QIcon(QPixmap(":/Logo/Resources/erase.png")));
    ui->History->setIcon(QIcon(QPixmap(":/Logo/Resources/history.png")));
    ui->Logout->setIcon(QIcon(QPixmap(":/Logo/Resources/logout.png")));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fillTable();
}

userPage::~userPage()
{
    delete ui;
}

void userPage:: receiveFirstname(QString Firstname)
{
    qDebug()<<Firstname;
    firstname1=Firstname;
    fillTable();
}


void userPage::fillTable()
{
    ui->label_10->setText(firstname1);
    QString busid,from,destination,date,time,price;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int rowNumber=0;

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
            ui->tableWidget->setItem(rowNumber,0,new QTableWidgetItem(QString(busid)));
            ui->tableWidget->setItem(rowNumber,1,new QTableWidgetItem(QString(from)));
            ui->tableWidget->setItem(rowNumber,2,new QTableWidgetItem(QString(destination)));
            ui->tableWidget->setItem(rowNumber,3,new QTableWidgetItem(QString(date)));
            ui->tableWidget->setItem(rowNumber,4,new QTableWidgetItem(QString(time)));
            ui->tableWidget->setItem(rowNumber,5,new QTableWidgetItem(QString(price)));
            QPushButton *selectButton = new QPushButton("Select", ui->tableWidget);
            selectButton->setStyleSheet("QPushButton {"
                                        "border-radius: 10px;"
                                        "padding: 10px;"
                                        "background-color: rgb(30, 222, 0);"
                                        "}" );
            selectButton->setIcon(QIcon(QPixmap(":/Logo/Resources/select.png")));
            connect(selectButton, &QPushButton::clicked, this,[this,busid]() {
                onselectButtonClicked(busid);
            });
            ui->tableWidget->setCellWidget(rowNumber, 6, selectButton);

            for (int col = 0; col < 6; col++)
            {
                ui->tableWidget->item(rowNumber, col)->setTextAlignment(Qt::AlignCenter);
            }
            rowNumber++;
        }
    }
}

void userPage:: onselectButtonClicked(QString Busid)
{
    qDebug()<<"Pressed";
    getDetails=new class getDetails;
    connect(this, &userPage::sendBusid, getDetails, &getDetails::receiveBusid);
    emit sendBusid(Busid);
    this->hide();
    getDetails->show();

}



void userPage::on_Search_clicked()
{
    int row=0;
    QString from=ui->From->text();
    QString destination=ui->Destination->text();
    QString pricelow=ui->PriceLow->text();
    QString pricehigh=ui->PriceHigh->text();
    QDate selectedDate = ui->calendarWidget->selectedDate();
    QString date = selectedDate.toString("yyyy-MM-dd");
    QDate currentDate= QDate:: currentDate();
    int error=0,results=0;
    if(pricelow.isEmpty())
    {
        ui->PriceLow->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceLowError->setVisible(true);
        ui->PriceLowError->setText("Enter min price");
        error++;
    }
    else if(!QRegularExpression("^[0-9]+$").match(pricelow).hasMatch())
    {
        ui->PriceLow->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceLowError->setVisible(true);
        ui->PriceLowError->setText("Price must contain integers only");
        error++;
    }
    else
    {
        ui->PriceLow->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceLowError->setVisible(false);
    }
    if(pricehigh.isEmpty())
    {
        ui->PriceHigh->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceHighError->setVisible(true);
        ui->PriceHighError->setText("Enter max price");
        error++;
    }
    else if(!QRegularExpression("^[0-9]+$").match(pricehigh).hasMatch())
    {
        ui->PriceHigh->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceHighError->setVisible(true);
        ui->PriceHighError->setText("Price must contain integers only");
        error++;
    }
    else
    {
        ui->PriceHigh->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PriceHighError->setVisible(false);
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
    if (currentDate>selectedDate)
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

        int plow=pricelow.toInt();
        int phigh=pricehigh.toInt();
        if(busdb.open())
        {
        QSqlQuery getData(busdb);
        if(ui->comboBox->currentIndex()==0)
        {
            getData.prepare("SELECT * FROM BUS ORDER BY PRICE ASC");
        }
        if(ui->comboBox->currentIndex()==1)
        {
            getData.prepare("SELECT * FROM BUS ORDER BY PRICE DESC");
        }
        if(ui->comboBox->currentIndex()==2)
        {
            getData.prepare("SELECT * FROM BUS ORDER BY TIME ASC");
        }
        if(ui->comboBox->currentIndex()==3)
        {
            getData.prepare("SELECT * FROM BUS ORDER BY TIME DESC");
        }
        getData.exec();
        while(getData.next())
        {
            if ((from.toLower() == getData.value("FROM").toString().toLower()) &&
                (destination.toLower() == getData.value("DESTINATION").toString().toLower()) &&
                (date == getData.value("DATE").toString()) &&
                (getData.value("PRICE").toString().toInt() >= plow) &&
                (getData.value("PRICE").toString().toInt() <= phigh))
            {
                 results++;
                ui->stackedWidget_2->setCurrentIndex(1);
                QString busid=getData.value("BUSID").toString();
                QString price=getData.value("PRICE").toString();
                QString time=getData.value("TIME").toString();
                ui->tableWidget_2->setItem(row,0,new QTableWidgetItem(QString(busid)));
                ui->tableWidget_2->setItem(row,1,new QTableWidgetItem(QString(from)));
                ui->tableWidget_2->setItem(row,2,new QTableWidgetItem(QString(destination)));
                ui->tableWidget_2->setItem(row,3,new QTableWidgetItem(QString(date)));
                ui->tableWidget_2->setItem(row,4,new QTableWidgetItem(QString(time)));
                ui->tableWidget_2->setItem(row,5,new QTableWidgetItem(QString(price)));
                QPushButton *selectButton = new QPushButton("Select", ui->tableWidget_2);
                selectButton->setStyleSheet("QPushButton {"
                                            "border-radius: 10px;"
                                            "padding: 10px;"
                                            "background-color: rgb(30, 222, 0);"
                                            "}" );
                selectButton->setIcon(QIcon(QPixmap(":/Logo/Resources/select.png")));
                connect(selectButton, &QPushButton::clicked, this,[this,busid]() {
                    onselectButtonClicked(busid);
                });
                ui->tableWidget_2->setCellWidget(row, 6, selectButton);

                for (int col = 0; col < 6; col++)
                {
                    ui->tableWidget_2->item(row, col)->setTextAlignment(Qt::AlignCenter);
                }
                row++;
            }
        }
        if(results==0)
        {
            QMessageBox::warning(this, "Search", "No results found");
        }
        }
    }
}

void userPage::on_comboBox_currentIndexChanged(int index)
{
    ui->tableWidget_2->clearContents();
    on_Search_clicked();
}


void userPage::on_Clear_clicked()
{
    QDate currentDate=QDate::currentDate();
    ui->From->clear();
    ui->Destination->clear();
    ui->PriceLow->clear();
    ui->PriceHigh->clear();
    ui->calendarWidget->setSelectedDate(currentDate);
    ui->stackedWidget_2->setCurrentIndex(0);
}


void userPage::on_Logout_clicked()
{
    userLogin *userLogin=new class userLogin;
    this->hide();
    userLogin->show();
}


void userPage::on_History_clicked()
{
    busdb.close();
    historyPage *historyPage=new class historyPage;
    historyPage->show();
}

