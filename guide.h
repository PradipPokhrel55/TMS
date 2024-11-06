#ifndef GUIDE_H
#define GUIDE_H

#include <QDialog>
#include <QWidget>
#include "ui_home.h"

namespace Ui {
class Guide;
}

class Guide : public QDialog
{
    Q_OBJECT

public:
    explicit Guide(QWidget *parent = nullptr);
    ~Guide();

private:
    Ui::Guide *ui;
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
