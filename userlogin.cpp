#include "userlogin.h"
#include "ui_userlogin.h"
#include "adminlogin.h"
#include "userpage.h"
#include <QPixmap>
#include <QRegularExpression>
#include <QMessageBox>
userLogin::userLogin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::userLogin)
{
    userdb=QSqlDatabase::addDatabase("QSQLITE");
    userdb.setDatabaseName("user.db");
    userdb.open();
    ui->setupUi(this);
    QString seat=QString("%1%2").arg(char('A'+3)).arg(0+1);
    qDebug()<<seat;
    setWindowTitle("User Login");
    setFixedSize(800, 650);
    ui->stackedWidget->setCurrentIndex(1);
    QPixmap logo(":/Logo/Resources/logo.png");
    ui->logo->setPixmap(logo.scaled(170,40,Qt::KeepAspectRatio));
    ui->Username->setPlaceholderText("Username");
    ui->Password->setPlaceholderText("Password");
    ui->Firstname->setPlaceholderText("First Name");
    ui->Lastname->setPlaceholderText("Last Name");
    ui->Username_2->setPlaceholderText("Username");
    ui->Password_2->setPlaceholderText("Password");
    ui->Password->setEchoMode(QLineEdit::Password);
    ui->Password_2->setEchoMode(QLineEdit::Password);
    ui->Show->setIcon(QIcon(QPixmap(":/Logo/Resources/show.png")));
    ui->Show_2->setIcon(QIcon(QPixmap(":/Logo/Resources/show.png")));
    ui->Admin->setIcon(QIcon(QPixmap(":/Logo/Resources/admin.png")));
    ui->LogIn->setIcon(QIcon(QPixmap(":/Logo/Resources/login.png")));    
    ui->Register->setIcon(QIcon(QPixmap(":/Logo/Resources/register.png")));
    ui->Register_2->setIcon(QIcon(QPixmap(":/Logo/Resources/register.png")));
    ui->UsernameError->setVisible(false);
    ui->PasswordError->setVisible(false);
    ui->FirstnameError->setVisible(false);
    ui->LastnameError->setVisible(false);
    ui->UsernameError_2->setVisible(false);
    ui->PasswordError_2->setVisible(false);
}

userLogin::~userLogin()
{
    delete ui;
}

void userLogin::on_Show_clicked()
{
    if (ui->Password->echoMode() == QLineEdit::Password) {
        ui->Password->setEchoMode(QLineEdit::Normal);
        ui->Show->setIcon(QIcon(QPixmap(":/Logo/Resources/hide.png")));
    } else {
        ui->Password->setEchoMode(QLineEdit::Password);
        ui->Show->setIcon(QIcon(QPixmap(":/Logo/Resources/show.png")));
    }
}

void userLogin::on_Show_2_clicked()
{
    if (ui->Password_2->echoMode() == QLineEdit::Password) {
        ui->Password_2->setEchoMode(QLineEdit::Normal);
        ui->Show_2->setIcon(QIcon(QPixmap(":/Logo/Resources/hide.png")));
    } else {
        ui->Password_2->setEchoMode(QLineEdit::Password);
        ui->Show_2->setIcon(QIcon(QPixmap(":/Logo/Resources/show.png")));
    }
}

void userLogin::on_LogIn_clicked()
{
    userdb.open();
    QString firstname;
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
    QSqlQuery getData(userdb);
    getData.prepare("SELECT USERNAME FROM USER WHERE USERNAME=:username");
    getData.bindValue(":username",username);
    getData.exec();
    if(!getData.next())
    {
        ui->Username->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->UsernameError->setVisible(true);
        ui->UsernameError->setText("Username doesn't exist");
        error++;
    }
    }
    if(error==0)
    {
        QSqlQuery getData(userdb);
        getData.prepare("SELECT * FROM USER");
        getData.exec();
        while(getData.next())
        {
            if(username==getData.value("USERNAME").toString() && password==getData.value("PASSWORD").toString())
        {
                firstname=getData.value("FIRSTNAME").toString();
            correct++;
            break;
        }
        }
        if(correct==1)
        {
            qDebug()<<"Logged In as User";
            userdb.close();
            QMessageBox::information(this, "Login", "Login Successful");
            userPage= new class userPage;
            getDetails = new class getDetails;
            historyPage= new class historyPage;
            connect(this, &userLogin::sendFirstname, userPage, &userPage::receiveFirstname);
            connect(this, &userLogin::sendUsername, getDetails, &getDetails::receiveUsername);
             connect(this, &userLogin::sendUsername, historyPage, &historyPage::receiveUsername);
            emit sendFirstname(firstname);
            emit sendUsername(username);
            this->hide();
            userPage->show();
        }
        else
        {
            userdb.close();
            ui->Password->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
            ui->PasswordError->setVisible(true);
            ui->PasswordError->setText("Incorrect Password");
        }
    }
}


void userLogin::on_Register_clicked()
{
    ui->Username->clear();
    ui->Username->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
    ui->UsernameError->setVisible(false);
    ui->Password->clear();
    ui->Password->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
    ui->PasswordError->setVisible(false);
    ui->stackedWidget->setCurrentIndex(0);
}


void userLogin::on_Register_2_clicked()
{
    userdb.open();
    QString firstname=ui->Firstname->text();
    QString lastname=ui->Lastname->text();
    QString username_2=ui->Username_2->text();
    QString password_2=ui->Password_2->text();
    int error=0;
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
    else if(!QRegularExpression("^[a-zA-Z]+$").match(firstname).hasMatch())
    {
        ui->Firstname->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->FirstnameError->setVisible(true);
        ui->FirstnameError->setText("Must contain alphabet characters only");
        error++;
    }
    else if(!username_2.isEmpty())
    {
        QSqlQuery checkUsername(userdb);
        checkUsername.prepare("SELECT USERNAME FROM USER WHERE USERNAME = :username");
        checkUsername.bindValue(":username", username_2);
        checkUsername.exec();
        if (checkUsername.next())
        {
            ui->Username_2->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
            ui->UsernameError_2->setVisible(true);
            ui->UsernameError_2->setText("Username already taken");
            error++;
        }
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
    if(username_2.isEmpty())
    {
        ui->Username_2->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->UsernameError_2->setVisible(true);
        ui->UsernameError_2->setText("Please enter the username");
        error++;
    }
    else if(username_2.length()<5)
    {
        ui->Username_2->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->UsernameError_2->setVisible(true);
        ui->UsernameError_2->setText("Must contain at least 5 alphanumeric characters");
        error++;
    }
    else if(!QRegularExpression("^[a-zA-Z0-9]").match(username_2).hasMatch())
    {
        ui->Username_2->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->UsernameError_2->setVisible(true);
        ui->UsernameError_2->setText("Must contain alphanumeric characters only");
        error++;
    }

    else
    {
        ui->Username_2->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->UsernameError_2->setVisible(false);
    }
    if(password_2.isEmpty())
    {
        ui->Password_2->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PasswordError_2->setVisible(true);
        ui->PasswordError_2->setText("Please enter the password");
        error++;
    }
    else if(password_2.length()<8)
    {
        ui->Password_2->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PasswordError_2->setVisible(true);
        ui->PasswordError_2->setText("Must contain at least 8 chracters");
        error++;
    }
    else if(QRegularExpression("\\s").match(password_2).hasMatch())
    {
        ui->Password_2->setStyleSheet("border: 1px solid red;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PasswordError_2->setVisible(true);
        ui->PasswordError_2->setText("Can't contain whitespaces");
        error++;
    }
    else
    {
        ui->Password_2->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
        ui->PasswordError_2->setVisible(false);
    }
    if(error==0)
    {
        QSqlQuery insertData(userdb);
        insertData.prepare("INSERT INTO USER (FIRSTNAME, LASTNAME, USERNAME, PASSWORD) VALUES (:firstname, :lastname, :username, :password)");
        insertData.bindValue(":firstname", firstname);
        insertData.bindValue(":lastname", lastname);
        insertData.bindValue(":username", username_2);
        insertData.bindValue(":password", password_2);
        insertData.exec();
        userdb.close();
        QMessageBox::information(this, "Register", "Registration Successful");
        ui->stackedWidget->setCurrentIndex(1);
        ui->Firstname->clear();
        ui->Lastname->clear();
        ui->Username_2->clear();
        ui->Password_2->clear();
    }
}


void userLogin::on_Cancel_clicked()
{
    userdb.close();
    ui->stackedWidget->setCurrentIndex(1);
    ui->Firstname->clear();
    ui->Firstname->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
    ui->FirstnameError->setVisible(false);
    ui->Lastname->clear();
    ui->Lastname->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
    ui->LastnameError->setVisible(false);
    ui->Username_2->clear();
    ui->Username_2->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
    ui->UsernameError_2->setVisible(false);
    ui->Password_2->clear();
    ui->Password_2->setStyleSheet("border: 1px solid black;border-radius: 10px;padding: 10px;font: 10pt 'Sans Serif Collection';color: black;");
    ui->PasswordError_2->setVisible(false);
}


void userLogin::on_Admin_clicked()
{
    userdb.close();
    adminLogin *adminLogin= new class adminLogin;
    this ->hide();
    adminLogin->show();
}

