#ifndef CAB_H
#define CAB_H

#include <QWidget>
#include <QDialog> // Change this to QDialog if not already
#include "ui_home.h"

namespace Ui {
class Cab;
}

class Cab : public QDialog
{
    Q_OBJECT

public:
    explicit Cab(QWidget *parent = nullptr);
    ~Cab();

private:
    Ui::Cab *ui;
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

#endif // CAB_H
