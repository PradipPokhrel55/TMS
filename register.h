#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QMainWindow>
#include <QSqlDatabase>

class MainWindow;

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);


    ~Register();

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_login_clicked();

private:
    MainWindow *mainWindowParent;
    Ui::Register *ui;
    QSqlDatabase mydb;

};

#endif
