#ifndef HOTEL_H
#define HOTEL_H

#include <QDialog>
#include <QWidget> // Change this to QDialog if not already
#include "ui_home.h"

namespace Ui {
class Hotel;
}

class Hotel : public QDialog
{
    Q_OBJECT

public:
    explicit Hotel(QWidget *parent = nullptr);
    ~Hotel();

private:
    Ui::Hotel *ui;
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

#endif // HOTEL_H
