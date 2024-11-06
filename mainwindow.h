#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include "home.h"
#include "register.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static QString username;

private slots:
    void openRegisterDialog();
    void on_pushButton_login_clicked();

private:
    Ui::MainWindow *ui;
    Home *homeWindow;

    QString hashPassword(const QString &password);
};
#endif
