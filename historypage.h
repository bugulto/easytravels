#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class historyPage;
}

class historyPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit historyPage(QWidget *parent = nullptr);
    ~historyPage();

public slots:
    void receiveUsername(QString username);

private:
    Ui::historyPage *ui;
    QSqlDatabase bookingdb;

private slots:
    void oncancelButtonClicked(QString row,QString column);
    void fillTable();
    void on_Done_clicked();
};

#endif // HISTORYPAGE_H
