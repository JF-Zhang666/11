#ifndef CHATWINDOW_H
#define CHATWINDOW_H
#include"QListWidget"
#include "socketthread.h"
#include <QTcpSocket>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QThread>
#include<QSqlDatabase>

namespace Ui {
class chatwindow;
}

class chatwindow : public QWidget
{
    Q_OBJECT

public:
    explicit chatwindow(QWidget *parent = nullptr);
    ~chatwindow();
    void setCurrentUsername(const QString &username);
    void initializeContactList();  // 初始化联系人列表
    QString currentusername;  // 存储当前登录的用户名

signals:
    void startConnect(quint16, QString);
    void sendMsgSignal(QString);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override ;

private slots:
    void readyRead_slot(QByteArray msg); //处理readyRead信号槽函数
    void on_btn1_clicked();
    void onContactClicked(QListWidgetItem *item);
    void sendPhotoButton_clicked();
    void sendFileButton_clicked();
    void receiveFileButton_clicked();
    void on_close_clicked();

private:
    Ui::chatwindow *ui;
    void loadChatHistory(const QString &username);
    QTcpSocket *socket;
    QWidget* createMessageBubble(const QString &username, const QString &message, bool isSelf);
    QWidget* createImageBubble(const QString &username, const QString &imagePath, bool isSelf);
    QWidget* createFileBubble(const QString &content);
    QString getAvatarPathFromUsername(const QString &username);
    //聊天记录相关
    void saveMessageToHistory(const QString &sender, const QString &receiver, const QString &content, const QString &contentType);
    void loadChatHistory(const QString &username1, const QString &username2);
    SocketThread *socketThread;
    QPoint mousePressPos;  // 鼠标按下时的全局位置
    QPoint windowPos;      // 窗口当前位置
    QPoint diffPos;
    QSqlDatabase db;

};

#endif // CHATWINDOW_H
