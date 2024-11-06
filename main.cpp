#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"

bool connectToDatabase(const QString& dbName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Database connection failed: " << db.lastError().text();
        return false;
    }
    qDebug() << "Database connection successful!";
    return true;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;


    QString dbPath = "/Users/pradippokhrel/Desktop/travel/travel.db";
    qDebug() << "Connecting to database at: " << dbPath;

    if (!connectToDatabase(dbPath)) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database.");
        return -1;  // Exit application if database connection fails
    }


    w.show();
    return a.exec();
}
