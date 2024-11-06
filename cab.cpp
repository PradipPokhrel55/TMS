#include "home.h"
#include "ui_cab.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QImage>
#include <QByteArray>
#include <QPixmap>
#include "cab.h"
#include "guide.h"
#include "hotel.h"
#include "mainwindow.h"
#include "register.h"
#include "profile.h"

Cab::Cab(QWidget *parent) : QDialog(parent), ui(new Ui::Cab)
{
    ui->setupUi(this);
    displayData();

    connect(ui->pushButton_6, &QPushButton::clicked, this, &Cab::on_pushButton_login_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Cab::openHomeDialog);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Cab::openHotelDialog);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Cab::openGuideDialog);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Cab::openCabDialog);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &Cab::openRegisterDialog);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &Cab::openProfileDialog);

}

Cab::~Cab()
{
    delete ui;
}

void Cab::displayData()
{
    QSqlQuery query("SELECT driver_name, contact_number, vehicle_type, capacity, fare_per_km, image FROM cabs");
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *containerWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(containerWidget);

    while (query.next()) {
        QString driverName = query.value(0).toString();
        QString contactNumber = query.value(2).toString();
        QString vehicleType = query.value(1).toString();
        QString capacity = query.value(4).toString();
        QString farePerKm = query.value(3).toString();
        QByteArray imageBlob = query.value(5).toByteArray();


        QImage image = QImage::fromData(imageBlob);
        QPixmap pixmap = QPixmap::fromImage(image);
        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio));
        imageLabel->setAlignment(Qt::AlignCenter);


        QVBoxLayout *infoLayout = new QVBoxLayout;
        QLabel *driverNameLabel = new QLabel("<b>Driver:</b> " + driverName);
        QLabel *contactLabel = new QLabel("<b>Contact:</b> " + contactNumber);
        QLabel *vehicleTypeLabel = new QLabel("<b>Vehicle Type:</b> " + vehicleType);
        QLabel *capacityLabel = new QLabel("<b>Capacity:</b> " + capacity);
        QLabel *fareLabel = new QLabel("<b>Fare per km:</b> " + farePerKm);

        QPushButton *purchaseButton = new QPushButton("Book Cab");
        purchaseButton->setProperty("driverName", driverName);
        purchaseButton->setProperty("vehicle", vehicleType);
        purchaseButton->setProperty("price", farePerKm);

        connect(purchaseButton, &QPushButton::clicked, this, &Cab::onPurchaseButtonClicked);

        purchaseButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #f57224;"
            "   color: white;"
            "   padding: 5px 10px;"
            "   border-radius: 10px;"
            "   font-weight: bold;"
            "   font-size: 14px;"
            "   border: none;"
            "}"
            );


        infoLayout->addWidget(driverNameLabel);
        infoLayout->addWidget(contactLabel);
        infoLayout->addWidget(vehicleTypeLabel);
        infoLayout->addWidget(capacityLabel);
        infoLayout->addWidget(fareLabel);
        infoLayout->addWidget(purchaseButton);

        QHBoxLayout *cabLayout = new QHBoxLayout;
        cabLayout->addWidget(imageLabel);
        cabLayout->addLayout(infoLayout);
        cabLayout->setContentsMargins(5, 5, 5, 5);
        cabLayout->setSpacing(15);

        mainLayout->addLayout(cabLayout);
    }

    containerWidget->setLayout(mainLayout);
    scrollArea->setWidget(containerWidget);

    QVBoxLayout *outerLayout = new QVBoxLayout(ui->groupBox);
    outerLayout->addWidget(scrollArea);
    ui->groupBox->setLayout(outerLayout);
}

void Cab::openHomeDialog()
{
    Home *homeDialog = new Home(this);
    this->hide();
    homeDialog->show();
}

void Cab::openHotelDialog()
{
    Hotel *hotelDialog = new Hotel(this);
    this->hide();
    hotelDialog->show();
}

void Cab::openGuideDialog()
{
    Guide *guideDialog = new Guide(this);
    this->hide();
    guideDialog->show();
}

void Cab::openCabDialog()
{
    Cab *cabDialog = new Cab(this);
    this->hide();
    cabDialog->show();
}

void Cab::openRegisterDialog()
{
    Register *registerDialog = new Register(this);
    registerDialog->setAttribute(Qt::WA_DeleteOnClose);
    this->hide();
    registerDialog->show();
}




void Cab::onPurchaseButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString driverName = button->property("driverName").toString();

        QString vehicleType = button->property("vehicle").toString();
        QString fare_per_km = button->property("price").toString();
        QString username = MainWindow::username;


        QMessageBox::information(this, "Booking Confirmation",
                                 "You have booked " + vehicleType + driverName + " at " + fare_per_km + " per km.");


        QSqlQuery query;
        query.prepare("UPDATE users SET cab_drivername = :driver_name, "
                      "cab_vehicletype = :vehicle_type, cab_price = :fare_per_km "
                      "WHERE username = :username");
        query.bindValue(":driver_name", driverName);
        query.bindValue(":vehicle_type", vehicleType);
        query.bindValue(":fare_per_km", fare_per_km);
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Booking Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Booking Successful",
                                     driverName + " has been successfully booked.");
        }
    }
}


void Cab::on_pushButton_login_clicked()
{
    MainWindow *mainwindowDialog = new MainWindow(this);
    this->hide();
    mainwindowDialog->show();
}

void Cab:: openProfileDialog(){
    Profile *profileDialog = new Profile(this);
    profileDialog->setAttribute(Qt::WA_DeleteOnClose);
    this->hide();
    profileDialog->show();
}
