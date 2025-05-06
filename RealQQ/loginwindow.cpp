#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include<QEvent>
#include<QMouseEvent>
#include<QPainter>
loginwindow::loginwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginwindow)
{
    ui->setupUi(this);
    QFont font("Arial", 14, QFont::Bold);
    ui->loginButton->setFont(font);

}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap background(":/C:/Users/24332/Desktop/MyQQ/backnew.png");  // 加载背景图片
    if (!background.isNull()) {
        painter.drawPixmap(rect(), background);  // 绘制背景图片
    }
}

void loginwindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标按下时的位置和窗口位置的差值
        mousePressPos = event->globalPosition().toPoint();
        windowPos = this->pos();
        diffPos = mousePressPos - windowPos;
        event->accept();
    }
}

void loginwindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        // 根据鼠标移动更新窗口位置
        QPoint newPos = event->globalPosition().toPoint();
        this->move(newPos - diffPos);
        event->accept();
    }
}

void loginwindow::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    authenticateUser(username, password);
}

void loginwindow::authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM usernames WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (query.exec() && query.next()) {
        emit loginSuccessful(username);
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "username or password is incorrect.");
    }
}


void loginwindow::on_close_clicked()
{
    close();
}

