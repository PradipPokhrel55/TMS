#include "register.h"
#include "ui_register.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include "mainwindow.h"

Register::Register(QWidget *parent)
    : QDialog(mainWindowParent)
    , ui(new Ui::Register)
    , mainWindowParent(mainWindowParent)

{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &Register::on_pushButton_register_clicked);
    connect(ui->pushButton_login, &QPushButton::clicked, this, &Register::on_pushButton_login_clicked);
    ui->password->setEchoMode(QLineEdit::Password);
    ui->password_3->setEchoMode(QLineEdit::Password);


}

Register::~Register()
{
    delete ui;
}




void Register::on_pushButton_register_clicked()
{
    QString username = ui->username->text().trimmed();
    QString password = ui->password->text().trimmed();
    QString repassword = ui->password_3->text().trimmed(); // Corrected variable name for re-entered password

    if (username.isEmpty() || password.isEmpty() || repassword.isEmpty()) {
        QMessageBox::warning(this, "Registration Failed", "Please fill in all fields.");
        return;
    }

    if (password != repassword) {
        QMessageBox::warning(this, "Registration Failed", "Passwords do not match.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // Hash the password

    if (!query.exec()) {
        QMessageBox::critical(this, "Registration Error", query.lastError().text());
    } else {
        QMessageBox::information(this, "Registration Successful", "User registered successfully.");

        if (mainWindowParent) mainWindowParent->show();
        this->hide();

    }

}

void Register::on_pushButton_login_clicked()
{
    if (mainWindowParent) mainWindowParent->show();
    this->hide();

}

