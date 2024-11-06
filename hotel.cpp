#include "home.h"
#include "ui_hotel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "cab.h"
#include "guide.h"
#include "hotel.h"
#include "mainwindow.h"
#include "register.h"
#include "profile.h"

#include <QScrollArea>
#include <QImage>
#include <QByteArray>
#include <QPixmap>

Hotel::Hotel(QWidget *parent) : QDialog(parent), ui(new Ui::Hotel)
{
    ui->setupUi(this);
    displayData();

    connect(ui->pushButton_6, &QPushButton::clicked, this, &Hotel::on_pushButton_login_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Hotel::openHomeDialog);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Hotel::openHotelDialog);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Hotel::openGuideDialog);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Hotel::openCabDialog);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &Hotel::openRegisterDialog);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &Hotel::openProfileDialog);
}

Hotel::~Hotel()
{
    delete ui;
}

void Hotel::displayData()
{
    QSqlQuery query("SELECT name, location, price_per_night, image, facilities, contact_number FROM hotels");
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
        QString price_per_night = query.value(2).toString();
        QString facilities = query.value(4).toString();
        QString contact_number = query.value(5).toString();
        QByteArray imageBlob = query.value(3).toByteArray();

        // Convert the image blob to a QPixmap
        QImage image = QImage::fromData(imageBlob);
        QPixmap pixmap = QPixmap::fromImage(image);
        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio));  // Larger size, keep aspect ratio
        imageLabel->setAlignment(Qt::AlignCenter);

        // Create a layout to hold hotel information and purchase button
        QVBoxLayout *infoLayout = new QVBoxLayout;

        QLabel *nameLabel = new QLabel("<b>Name:</b> " + name);
        QLabel *locationLabel = new QLabel("<b>Location:</b> " + location);
        QLabel *facilitieslabel = new QLabel("<b>Facilities:</b> " + facilities);
        QLabel *contact_numberlabel = new QLabel("<b>Contact_number:</b> " + contact_number);
        QLabel *priceLabel = new QLabel("<b>Rate per night:</b> " + price_per_night);

        QPushButton *purchaseButton = new QPushButton("Purchase");
        purchaseButton->setProperty("hotelName", name);
        purchaseButton->setProperty("location", location);
        purchaseButton->setProperty("price", price_per_night);

        connect(purchaseButton, &QPushButton::clicked, this, &Hotel::onPurchaseButtonClicked);

        purchaseButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #f57224;"
            "   color: white;"
            "   padding: 5px 10px;"
            "   border-radius: 10px;"
            "   font-weight: bold;"
            "   font-size: 14px;"
            "   border: none;"
            "   width: 80px"
            "}"
            );

        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(locationLabel);
        infoLayout->addWidget(contact_numberlabel);
        infoLayout->addWidget(facilitieslabel);
        infoLayout->addWidget(priceLabel);
        infoLayout->addWidget(purchaseButton);

        // Combine image and information in a horizontal layout for each hotel entry
        QHBoxLayout *hotelLayout = new QHBoxLayout;
        hotelLayout->addWidget(imageLabel);
        hotelLayout->addLayout(infoLayout);

        // Add spacing and borders
        hotelLayout->setContentsMargins(5, 5, 5, 5);
        hotelLayout->setSpacing(15);

        // Add the hotel entry layout to the main layout
        mainLayout->addLayout(hotelLayout);
    }

    containerWidget->setLayout(mainLayout);
    scrollArea->setWidget(containerWidget);

    // Add the scroll area to the main layout of the group box
    QVBoxLayout *outerLayout = new QVBoxLayout(ui->groupBox);
    outerLayout->addWidget(scrollArea);
    ui->groupBox->setLayout(outerLayout);
}

void Hotel::openHotelDialog()
{
    Hotel *hotelDialog = new Hotel(this);
    this->hide();
    hotelDialog->show();
}

void Hotel::openHomeDialog()
{
    Home *homeDialog = new Home(this);
    this->hide();
    homeDialog->show();
}

void Hotel::openGuideDialog()
{
    Guide *guideDialog = new Guide(this);
    this->hide();
    guideDialog->show();
}

void Hotel::openCabDialog()
{
    Cab *cabDialog = new Cab(this);
    this->hide();
    cabDialog->show();
}

void Hotel::onPurchaseButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString hotelName = button->property("hotelName").toString();
        QString hotelLocation = button->property("location").toString();
        QString hotelPrice = button->property("price").toString();
        QString username = MainWindow::username; // Retrieve the logged-in username

        // Confirm hotel purchase with the user
        QMessageBox::information(this, "Purchase", "Purchasing " + hotelName + " located at " + hotelLocation + " at " + hotelPrice);

        // Update hotel booking details for the current user in the database
        QSqlQuery query;
        query.prepare("UPDATE users SET hotel_name = :hotel_name, hotel_price = :hotel_price, hotel_location = :hotel_location "
                      "WHERE username = :username");
        query.bindValue(":hotel_name", hotelName);
        query.bindValue(":hotel_price", hotelPrice);
        query.bindValue(":hotel_location", hotelLocation);
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Purchase Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Hotel Purchased",
                                     hotelName + " has been successfully booked at " + hotelPrice + " per night.");
        }
    }
}

void Hotel::on_pushButton_login_clicked()
{
    MainWindow *mainwindowDialog = new MainWindow(this);
    this->hide();
    mainwindowDialog->show();
}

void Hotel::openRegisterDialog()
{
    Register *registerDialog = new Register(this);
    registerDialog->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    this->hide();
    registerDialog->show(); // Show the register dialog
}

void Hotel::openProfileDialog()
{
    Profile *profileDialog = new Profile(this);
    profileDialog->setAttribute(Qt::WA_DeleteOnClose);
    this->hide();
    profileDialog->show();
}
