#include "home.h"
#include "ui_guide.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QPixmap>
#include <QByteArray>
#include "cab.h"
#include "guide.h"
#include "hotel.h"
#include "mainwindow.h"
#include "register.h"
#include "profile.h"

Guide::Guide(QWidget *parent) : QDialog(parent), ui(new Ui::Guide)
{
    ui->setupUi(this);
    displayData();

    connect(ui->pushButton_6, &QPushButton::clicked, this, &Guide::on_pushButton_login_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Guide::openHomeDialog);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Guide::openHotelDialog);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Guide::openGuideDialog);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Guide::openCabDialog);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &Guide::openRegisterDialog);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &Guide::openProfileDialog);
}

Guide::~Guide()
{
    delete ui;
}









void Guide::displayData()
{
    QSqlQuery query("SELECT name, languages_spoken, hourly_rate, contact_number, experience_years, image FROM guides");
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
        QString languages_spoken = query.value(1).toString();
        QString hourly_rate = query.value(2).toString();
        QString contact_number = query.value(3).toString();
        QString experience_years = query.value(4).toString();
        QByteArray imageData = query.value(5).toByteArray();

        QLabel *nameLabel = new QLabel("<b>Name:</b> " + name);
        QLabel *languagesLabel = new QLabel("<b>Languages:</b> " + languages_spoken);
        QLabel *rateLabel = new QLabel("<b>Hourly Rate:</b> " + hourly_rate);
        QLabel *contactLabel = new QLabel("<b>Contact:</b> " + contact_number);
        QLabel *experienceLabel = new QLabel("<b>Experience:</b> " + experience_years + " years");


        QLabel *imageLabel = new QLabel();
        if (!imageData.isEmpty()) {
            QPixmap pixmap;
            pixmap.loadFromData(imageData);
            imageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            imageLabel->setText("No Image Available");
        }

        QPushButton *purchaseButton = new QPushButton("Hire Guide");
        purchaseButton->setProperty("guideName", name);
        purchaseButton->setProperty("hourlyRate", hourly_rate);
        purchaseButton->setProperty("guideNumber", contact_number);
        connect(purchaseButton, &QPushButton::clicked, this, &Guide::onPurchaseButtonClicked);

        purchaseButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #f57224;"
            "   color: white;"
            "   padding: 5px 10px;"
            "   border-radius: 10px;"
            "   font-weight: bold;"
            "   font-size: 14px;"
            "   border: none;"
            "   width: 100px;"
            "}"
            );

        QVBoxLayout *infoLayout = new QVBoxLayout;
        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(languagesLabel);
        infoLayout->addWidget(rateLabel);
        infoLayout->addWidget(contactLabel);
        infoLayout->addWidget(experienceLabel);
        infoLayout->addWidget(purchaseButton);

        QHBoxLayout *guideLayout = new QHBoxLayout;
        guideLayout->addWidget(imageLabel);
        guideLayout->addLayout(infoLayout);

        guideLayout->setContentsMargins(5, 5, 5, 5);
        guideLayout->setSpacing(15);

        mainLayout->addLayout(guideLayout);
    }

    containerWidget->setLayout(mainLayout);
    scrollArea->setWidget(containerWidget);

    QVBoxLayout *outerLayout = new QVBoxLayout(ui->groupBox);
    outerLayout->addWidget(scrollArea);
    ui->groupBox->setLayout(outerLayout);
}



void Guide::openHomeDialog()
{
    Home *homeDialog = new Home(this);
    this->hide();
    homeDialog->show();
}

void Guide::openHotelDialog()
{
    Hotel *hotelDialog = new Hotel(this);
    this->hide();
    hotelDialog->show();
}

void Guide::openGuideDialog()
{
    Guide *guideDialog = new Guide(this);
    this->hide();
    guideDialog->show();
}

void Guide::openCabDialog()
{
    Cab *cabDialog = new Cab(this);
    this->hide();
    cabDialog->show();
}



void Guide::onPurchaseButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString guideName = button->property("guideName").toString();
        QString hourlyRate = button->property("hourlyRate").toString();
        QString guideNumber = button->property("guideNumber").toString();
        QString username = MainWindow::username;


        QMessageBox::information(this, "Hire Guide", "Hiring " + guideName + guideNumber +" at " + hourlyRate + " per hour.");


        QSqlQuery query;
        query.prepare("UPDATE users SET guide_name = :guide_name, guide_price = :guide_price, guide_number = :guide_number "
                      "WHERE username = :username");
        query.bindValue(":guide_name", guideName);
        query.bindValue(":guide_price", hourlyRate);
        query.bindValue(":guide_number", guideNumber);
        query.bindValue(":username", username);

        if (!query.exec()) {
            QMessageBox::critical(this, "Hire Guide Error", query.lastError().text());
        } else {
            QMessageBox::information(this, "Guide Hired",
                                     guideName + " has been successfully hired at " + hourlyRate + " per hour.");
        }
    }
}


void Guide::on_pushButton_login_clicked()
{
    MainWindow *mainwindowDialog = new MainWindow(this);
    this->hide();
    mainwindowDialog->show();
}

void Guide::openRegisterDialog()
{
    Register *registerDialog = new Register(this);
    registerDialog->setAttribute(Qt::WA_DeleteOnClose);
    this->hide();
    registerDialog->show();
}

void Guide:: openProfileDialog(){
    Profile *profileDialog = new Profile(this);
    profileDialog->setAttribute(Qt::WA_DeleteOnClose);
    this->hide();
    profileDialog->show();
}

