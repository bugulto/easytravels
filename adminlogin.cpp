#include "adminlogin.h"
#include "ui_adminlogin.h"
#include "userlogin.h"
#include "adminpage.h"
#include <QMessageBox>
adminLogin::adminLogin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminLogin)
{
    admindb=QSqlDatabase::addDatabase("QSQLITE");
    admindb.setDatabaseName("admin.db");
    admindb.open();
    ui->setupUi(this);
    setWindowTitle("Admin Login");
    setFixedSize(800, 650);
    QPixmap logo(":/Logo/Resources/logo2.png");
    ui->logo->setPixmap(logo.scaled(170,40,Qt::KeepAspectRatio));
    QPixmap admin(":/Logo/Resources/admin.png");
    ui->adminLogo->setPixmap(admin.scaled(60,60,Qt::KeepAspectRatio));
    ui->Username->setPlaceholderText("Username");
    ui->Password->setPlaceholderText("Password");
    ui->Password->setEchoMode(QLineEdit::Password);
    ui->Show->setIcon(QIcon(QPixmap(":/Logo/Resources/show.png")));
    ui->LogIn->setIcon(QIcon(QPixmap(":/Logo/Resources/login.png")));
    ui->UsernameError->setVisible(false);
    ui->PasswordError->setVisible(false);
}

adminLogin::~adminLogin()
{
    delete ui;
}

void adminLogin::on_Show_clicked()
{
    if (ui->Password->echoMode() == QLineEdit::Password) {
        ui->Password->setEchoMode(QLineEdit::Normal);
        ui->Show->setIcon(QIcon(QPixmap(":/Logo/Resources/hide.png")));
    } else {
        ui->Password->setEchoMode(QLineEdit::Password);
        ui->Show->setIcon(QIcon(QPixmap(":/Logo/Resources/show.png")));
    }
}


void adminLogin::on_LogIn_clicked()
{
    admindb.open();
    QString username=ui->Username->text();
    QString password=ui->Password->text();
    int error=0;
    int correct=0;
    if(username.isEmpty())
    {
        ui->Username->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->UsernameError->setVisible(true);
        ui->UsernameError->setText("Please enter the username");
        error++;
    }
    else
    {
        ui->Username->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->UsernameError->setVisible(false);
    }
    if(password.isEmpty())
    {
        ui->Password->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PasswordError->setVisible(true);
        ui->PasswordError->setText("Please enter the password");
        error++;
    }
    else
    {
        ui->Password->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PasswordError->setVisible(false);
    }
    if(!username.isEmpty())
    {
        QSqlQuery getData(admindb);
        getData.prepare("SELECT USERNAME FROM ADMIN WHERE USERNAME=:username");
        getData.bindValue(":username",username);
        getData.exec();
        if(!getData.next())
        {
            ui->Username->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
            ui->UsernameError->setVisible(true);
            ui->UsernameError->setText("Admin username doesn't exist");
            error++;
        }
    }
    if(error==0)
    {
        QSqlQuery getData(admindb);
        getData.prepare("SELECT * FROM ADMIN");
        getData.exec();
        while(getData.next())
        {
            if(password==getData.value("PASSWORD").toString())
            {
                correct++;
                break;
            }
        }
        if(correct==1)
        {
            qDebug()<<"Logged In as Admin";
            admindb.close();
            QMessageBox::information(this, "Login", "Login Successful");
            adminPage *adminPage= new class adminPage;
            this->hide();
            adminPage->show();
        }
        else
        {
            admindb.close();
            ui->Password->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
            ui->PasswordError->setVisible(true);
            ui->PasswordError->setText("Incorrect Password");
        }
    }
}



void adminLogin::on_Cancel_clicked()
{
    admindb.close();
    userLogin *userLogin= new class userLogin;
    this->hide();
    userLogin->show();
}

