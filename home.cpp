#include "home.h"
#include "ui_home.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "cab.h"
#include "guide.h"
#include "hotel.h"
#include "mainwindow.h"
#include "register.h"
#include "profile.h"

Home::Home(QWidget *parent) : QDialog(parent), ui(new Ui::Home)
{
    ui->setupUi(this);
    displayData();

    connect(ui->pushButton_6, &QPushButton::clicked, this, &Home::on_pushButton_login_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Home::openHomeDialog);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Home::openHotelDialog);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Home::openGuideDialog);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Home::openCabDialog);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &Home::openRegisterDialog);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &Home::openProfileDialog);
}

Home::~Home()
{
    delete ui;
}







void Home::displayData()
{
    QSqlQuery query("SELECT name, location, contact_number, price_per_night, image, facilities FROM packages");
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }


    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *containerWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(containerWidget);

    while (query.next()) {
        QString name = query.value(0).toString();
        QString location = query.value(1).toString();
        QString contact_number = query.value(2).toString();
        QString price_per_night = query.value(3).toString();
        QByteArray imageBlob = query.value(4).toByteArray();
        QString facilities = query.value(5).toString();


        QImage image = QImage::fromData(imageBlob);
        QPixmap pixmap = QPixmap::fromImage(image);
        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio));
        imageLabel->setAlignment(Qt::AlignCenter);


        QWidget *packageWidget = new QWidget();
        QHBoxLayout *packageLayout = new QHBoxLayout(packageWidget);


        QVBoxLayout *infoLayout = new QVBoxLayout();
        QLabel *nameLabel = new QLabel("<b>Name:</b> " + name);
        QLabel *locationLabel = new QLabel("<b>Location:</b> " + location);
        QLabel *contactLabel = new QLabel("<b>Contact:</b> " + contact_number);
        QLabel *priceLabel = new QLabel("<b>Price per night:</b> " + price_per_night);
        QLabel *facilitiesLabel = new QLabel("<b>Facilities:</b> " + facilities);


        QPushButton *purchaseButton = new QPushButton("Purchase");
        purchaseButton->setProperty("Name", name);
        purchaseButton->setProperty("price", price_per_night);
        purchaseButton->setProperty("location", location);

        connect(purchaseButton, &QPushButton::clicked, this, &Home::onPurchaseButtonClicked);
        purchaseButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #f57224;"
            "   color: white;"
            "   padding: 5px 10px;"
            "   border-radius: 10px;"
            "   font-weight: bold;"
            "   font-size: 14px;"
            "   border: none;"
            "   width: 80px;"
            "}"
            );


        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(locationLabel);
        infoLayout->addWidget(contactLabel);
        infoLayout->addWidget(priceLabel);
        infoLayout->addWidget(facilitiesLabel);
        infoLayout->addWidget(purchaseButton);


        packageLayout->addWidget(imageLabel);
        packageLayout->addLayout(infoLayout);


        mainLayout->addWidget(packageWidget);
        mainLayout->addSpacing(15);
    }

    containerWidget->setLayout(mainLayout);
    scrollArea->setWidget(containerWidget);


    QVBoxLayout *outerLayout = new QVBoxLayout(ui->groupBox);
    outerLayout->addWidget(scrollArea);
    ui->groupBox->setLayout(outerLayout);
}










void Home::openHomeDialog()
{
    Home *homeDialog = new Home(this);
    this->hide();
    homeDialog->show();
}

void Home::openHotelDialog()
{
    Hotel *hotelDialog = new Hotel(this);
    this->hide();
    hotelDialog->show();
}

void Home::openGuideDialog()
{
    Guide *guideDialog = new Guide(this);
    this->hide();
    guideDialog->show();
}

void Home::openCabDialog()
{
    Cab *cabDialog = new Cab(this);
    this->hide();
    cabDialog->show();
}


void Home::onPurchaseButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString name = button->property("Name").toString();
        QString price = button->property("price").toString();
        QString location = button->property("location").toString();
        QString username = MainWindow::username;

        // Confirm booking with the user
        QMessageBox::information(this, "Booking Confirmation",
                                 "You have booked " + name + " at " + price + " per km.");

        // Insert booking details into the database
        QSqlQuery query;
        query.prepare("UPDATE users SET package_name = :package_name, "
                      "package_price = :package_price, package_location = :package_location "
                      "WHERE username = :username");
        query.bindValue(":package_name", name);
        query.bindValue(":package_location", location);
        query.bindValue(":package_price", price);
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Booking Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Booking Successful",
                                     name + " has been successfully booked.");
        }
    }
}


void Home::on_pushButton_login_clicked()
{
    MainWindow *mainwindowDialog = new MainWindow(this);
    this->hide();
    mainwindowDialog->show();
}

void Home::openRegisterDialog()
{
    Register *registerDialog = new Register(this);
    registerDialog->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    this->hide();
    registerDialog->show(); // Show the register dialog
}

void Home:: openProfileDialog(){
    Profile *profileDialog = new Profile(this);
    profileDialog->setAttribute(Qt::WA_DeleteOnClose);
    this->hide();
    profileDialog->show();
}
