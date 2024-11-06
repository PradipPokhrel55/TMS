#include "profile.h"
#include "ui_profile.h"
#include <QScrollArea>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "home.h"
#include "cab.h"
#include "guide.h"
#include "hotel.h"
#include "mainwindow.h"
#include "register.h"

Profile::Profile(QWidget *parent) : QDialog(parent), ui(new Ui::Profile)
{
    ui->setupUi(this);
    displayData();

    // Connect buttons to their respective slots
    connect(ui->pushButton_6, &QPushButton::clicked, this, &Profile::on_pushButton_login_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Profile::openHomeDialog);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Profile::openHotelDialog);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Profile::openGuideDialog);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Profile::openCabDialog);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &Profile::openRegisterDialog);


}

Profile::~Profile()
{
    delete ui;
}



void Profile::displayData()
{
    // Clear existing layout to avoid duplication
    if (QLayout *oldLayout = ui->groupBox->layout()) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QSqlQuery query;
    QString username = MainWindow::username;  // Ensure this is set and accessible

    query.prepare("SELECT * FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *containerWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(containerWidget);

    // Define the stylesheet for delete buttons
    QString deleteButtonStyle = R"(
        QPushButton {
            width: 100px;
            background-color: #d80000;
            color: white;
            padding: 5px 10px;
            border-radius: 4px;
            font-weight: bold;
            border: none;
        }
    )";

    while (query.next()) {
        int id = query.value(0).toInt();

        QString hotelName = query.value(3).toString();
        QString hotelPrice = query.value(4).toString();
        QString hotelLocation = query.value(5).toString();
        QString cabDriverName = query.value(6).toString();
        QString cabVehicleType = query.value(7).toString();
        QString cabPrice = query.value(8).toString();
        QString guideName = query.value(9).toString();
        QString guideNumber = query.value(10).toString();
        QString guidePrice = query.value(11).toString();
        QString packageName = query.value(13).toString();
        QString packagePrice = query.value(14).toString();
        QString packageLocation = query.value(15).toString();


        auto addSection = [&](const QString &labelText, QPushButton *deleteButton = nullptr) {
            QHBoxLayout *sectionLayout = new QHBoxLayout();
            QLabel *label = new QLabel(labelText);
            label->setStyleSheet("font-size: 14px; color: #333;");
            sectionLayout->addWidget(label);
            if (deleteButton) {
                deleteButton->setStyleSheet(deleteButtonStyle);
                sectionLayout->addWidget(deleteButton);
            }
            mainLayout->addLayout(sectionLayout);
        };





        // Username
        addSection(QString("<b>Username:</b> %1").arg(username));

        // Hotel information
        if (!hotelName.isEmpty()) {
            QPushButton *hotelDeleteButton = new QPushButton("Delete");
            hotelDeleteButton->setProperty("entryId", id);
            connect(hotelDeleteButton, &QPushButton::clicked, this, &Profile::onHotelDeleteButtonClicked);
            addSection(QString("<b>Hotel:</b> %1 | <b>Price:</b> %2 | <b>Location:</b> %3")
                           .arg(hotelName, hotelPrice, hotelLocation), hotelDeleteButton);
        }

        // Cab information
        if (!cabDriverName.isEmpty()) {
            QPushButton *cabDeleteButton = new QPushButton("Delete");
            cabDeleteButton->setProperty("entryId", id);
            connect(cabDeleteButton, &QPushButton::clicked, this, &Profile::onCabDeleteButtonClicked);
            addSection(QString("<b>Cab Driver:</b> %1 | <b>Vehicle Type:</b> %2 | <b>Price per Km:</b> %3")
                           .arg(cabDriverName, cabVehicleType, cabPrice), cabDeleteButton);
        }

        // Guide information
        if (!guideName.isEmpty()) {
            QPushButton *guideDeleteButton = new QPushButton("Delete");
            guideDeleteButton->setProperty("entryId", id);
            connect(guideDeleteButton, &QPushButton::clicked, this, &Profile::onGuideDeleteButtonClicked);
            addSection(QString("<b>Guide:</b> %1 | <b>Contact:</b> %2 | <b>Price per day:</b> %3")
                           .arg(guideName, guideNumber, guidePrice), guideDeleteButton);
        }

        // Package information
        if (!packageName.isEmpty()) {
            QPushButton *packageDeleteButton = new QPushButton("Delete");
            packageDeleteButton->setProperty("entryId", id);
            connect(packageDeleteButton, &QPushButton::clicked, this, &Profile::onHomeDeleteButtonClicked);
            addSection(QString("<b>Package:</b> %1 | <b>Price:</b> %2 | <b>Location:</b> %3")
                           .arg(packageName, packagePrice, packageLocation), packageDeleteButton);
        }

        // Space between each record for clarity
        mainLayout->addSpacing(10);
    }

    containerWidget->setLayout(mainLayout);
    scrollArea->setWidget(containerWidget);

    // Apply styles to scroll area and group box
    scrollArea->setStyleSheet("background-color: #f9f9f9; border: none;");
    containerWidget->setStyleSheet("background-color: #f9f9f9;");

    // Add the scroll area to the group box with proper layout
    QVBoxLayout *outerLayout = new QVBoxLayout(ui->groupBox);
    outerLayout->addWidget(scrollArea);
    ui->groupBox->setLayout(outerLayout);
}






// Delete functions

void Profile::onHotelDeleteButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QSqlQuery query;
        QString username = MainWindow::username;

        query.prepare("UPDATE users SET hotel_name=NULL, hotel_price=NULL, hotel_location=NULL WHERE username = :username");
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Delete", "Hotel information deleted successfully.");
            displayData();
        }
    }
}

void Profile::onCabDeleteButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QSqlQuery query;
        QString username = MainWindow::username;

        query.prepare("UPDATE users SET cab_drivername=NULL, cab_price=NULL, cab_vehicletype=NULL WHERE username = :username");
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Delete", "Cab information deleted successfully.");
            displayData();
        }
    }
}

void Profile::onGuideDeleteButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QSqlQuery query;
        QString username = MainWindow::username;

        query.prepare("UPDATE users SET guide_name=NULL, guide_price=NULL, guide_number=NULL WHERE username = :username");
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Delete", "Guide information deleted successfully.");
            displayData();
        }
    }
}

void Profile::onHomeDeleteButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QSqlQuery query;
        QString username = MainWindow::username;

        query.prepare("UPDATE users SET package_name=NULL, package_price=NULL, package_location=NULL WHERE username = :username");
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Delete", "Package information deleted successfully.");
            displayData();
        }
    }
}

// Navigation functions

void Profile::openHomeDialog()
{
    Home *homeDialog = new Home(this);
    this->hide();
    homeDialog->show();
}

void Profile::openHotelDialog()
{
    Hotel *hotelDialog = new Hotel(this);
    this->hide();
    hotelDialog->show();
}

void Profile::openGuideDialog()
{
    Guide *guideDialog = new Guide(this);
    this->hide();
    guideDialog->show();
}

void Profile::openCabDialog()
{
    Cab *cabDialog = new Cab(this);
    this->hide();
    cabDialog->show();
}

void Profile::openRegisterDialog()
{
    Register *registerDialog = new Register(this);
    registerDialog->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    this->hide();
    registerDialog->show();
}

void Profile::on_pushButton_login_clicked()
{
    MainWindow *mainwindowDialog = new MainWindow(this);
    this->hide();
    mainwindowDialog->show();
}
