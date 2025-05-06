#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QThread>
#include "chatwindow.h"
#include "loginwindow.h"
#include "server.h"
#include <QApplication>

void initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("chatapp");
    db.setUserName("root");
    db.setPort(3306);    // 替换为您的 MySQL 用户名
    db.setPassword("123456"); // 替换为您的 MySQL 密码

    if (!db.open()) {
        qDebug() << "Error: Unable to open database" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS files ("
                    "id INT AUTO_INCREMENT PRIMARY KEY,"
                    "filepath VARCHAR(255) NOT NULL,"
                    "received VARCHAR(10) NOT NULL,"
                    "username VARCHAR(255) NOT NULL)")) {
        qDebug() << "Error creating table:" << query.lastError().text();
    } else {
        qDebug() << "Table 'files' created successfully.";
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS usernames ("
                    "id INT AUTO_INCREMENT PRIMARY KEY,"
                    "username VARCHAR(255) NOT NULL,"
                    "password VARCHAR(255) NOT NULL,"
                    "avatar_path VARCHAR(255) NOT NULL)")) {
        qDebug() << "Error creating table:" << query.lastError().text();
    } else {
        qDebug() << "Table 'usernames' created successfully.";
    }

    QStringList usernames = {"张三", "李四", "王五"};
    QStringList avatarPaths = {"1.png", "2.png", "3.png"};
    for (int i = 0; i < usernames.size(); ++i) {
        QString username = usernames[i];
        QString avatarPath = avatarPaths[i];
        QSqlQuery query;
        query.prepare("INSERT INTO usernames (username, password, avatar_path) VALUES (:username, :password, :avatar_path)");
        query.bindValue(":username", username);
        query.bindValue(":password", "123");
        query.bindValue(":avatar_path", avatarPath);
        if (!query.exec()) {
            qDebug() << "Error inserting data:" << query.lastError().text();
        } else {
            qDebug() << "Inserted data for username:" << username << "with avatar path:" << avatarPath;
        }
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS chat_history ("
                    "id INT AUTO_INCREMENT PRIMARY KEY,"
                    "sender VARCHAR(255),"
                    "receiver VARCHAR(255),"
                    "content TEXT,"
                    "contentType VARCHAR(10))")) {
        qDebug() << "Error creating table:" << query.lastError().text();
    } else {
        qDebug() << "Table 'chat_history' created successfully.";
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initializeDatabase();
    QFont font("Arial", 12, QFont::Bold); // 设置字体

    Server server1;
    server1.start();

    loginwindow *loginWindow = new loginwindow();
    chatwindow *chatWindow = new chatwindow();

    QObject::connect(loginWindow, &loginwindow::loginSuccessful, chatWindow, [chatWindow](const QString &username){

        chatWindow->setCurrentUsername(username);
        chatWindow->initializeContactList();
        chatWindow->setWindowFlags(Qt::FramelessWindowHint);
        chatWindow->setMouseTracking(true);
        chatWindow->show();
    });
    loginWindow->setWindowFlags(Qt::FramelessWindowHint);
    loginWindow->setMouseTracking(true);
    loginWindow->show();

    return a.exec();
}
