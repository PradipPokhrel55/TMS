#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QDialog>
#include "ui_home.h"

namespace Ui {
class Home;
}

class Home : public QDialog
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

private:
    Ui::Home *ui;
    void displayData();

private slots:
    void on_pushButton_login_clicked();
    void onPurchaseButtonClicked();
    void openHomeDialog();
    void openHotelDialog();
    void openGuideDialog();
    void openCabDialog();
    void openRegisterDialog();
    void openProfileDialog();
};

#endif
