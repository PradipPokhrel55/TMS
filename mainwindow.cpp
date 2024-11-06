#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "home.h"
#include "register.h"


QString MainWindow::username = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , homeWindow(nullptr)
{

    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::openRegisterDialog);
    connect(ui->pushButton_login, &QPushButton::clicked, this, &MainWindow::on_pushButton_login_clicked);
    ui->password->setEchoMode(QLineEdit::Password);

}

MainWindow::~MainWindow()
{
    delete ui;
    if (homeWindow) delete homeWindow;
}

void MainWindow::openRegisterDialog()
{
    Register *registerDialog = new Register(this);
    registerDialog->setAttribute(Qt::WA_DeleteOnClose);

    registerDialog->show();
}



void MainWindow::on_pushButton_login_clicked()
{
    QString username = ui->username->text().trimmed();
    QString password = ui->password->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both username and password.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "SQL Error:" << query.lastError().text();
        QMessageBox::critical(this, "SQL Error", query.lastError().text());
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            MainWindow::username = username;
            this->hide();
            if (!homeWindow) {
                homeWindow = new Home();
            }
            homeWindow->show();
        } else {
            QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        }
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid user or password.");
    }
}
