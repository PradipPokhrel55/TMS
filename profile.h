#ifndef PROFILE_H
#define PROFILE_H

#include <QDialog>

namespace Ui {
class Profile;
}

class Profile : public QDialog
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

private:
    Ui::Profile *ui;
    void displayData();
    QString username;


private slots:
    void on_pushButton_login_clicked();
    void openHomeDialog();
    void openHotelDialog();
    void openGuideDialog();
    void openCabDialog();
    void openRegisterDialog();
    void onHotelDeleteButtonClicked();
    void onHomeDeleteButtonClicked();
    void onGuideDeleteButtonClicked();
    void onCabDeleteButtonClicked();
};

#endif // PROFILE_H
