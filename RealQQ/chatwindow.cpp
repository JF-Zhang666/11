#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QTextBrowser>
#include <QFile>
#include <QDataStream>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTextStream>
#include <QListWidgetItem>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QTextImageFormat>
#include "socketthread.h"
#include<QEvent>
#include<QPaintEvent>
#include<QPainter>
chatwindow::chatwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chatwindow), db(QSqlDatabase::database("chatapp"))
{
    ui->setupUi(this);
    QFont font("Arial", 14, QFont::Bold);
    ui->btn1->setFont(font);
    QFont font1("Arial",9);
    ui->sendFileButton->setFont(font1);
    ui->receiveFileButton->setFont(font1);
    ui->sendPhotoButton->setFont(font1);
    QString ip = "127.0.0.1";
    quint16 port = 8888;
    QThread *sub = new QThread;
    SocketThread *worker = new SocketThread;
    worker->moveToThread(sub);
    //连接服务器
    connect(this, &chatwindow::startConnect, worker, &SocketThread::connectServer);
    connect(this, &chatwindow::sendMsgSignal, worker, &SocketThread::sendMsgToServer);
    connect(worker, &SocketThread::connectOK, this, [=](){
        qDebug()<<"connection";
    });
    connect(worker, &SocketThread::gameOver, this, [=](){
        sub->quit();
        sub->wait();
        worker->deleteLater();
        sub->deleteLater();  // 线程对象析构
    });
    sub->start();   // 启动子线程
    connect(sub, &QThread::started, this, [=]() {
        emit startConnect(port, ip);
    });
    // 选择联系人
    connect(ui->chatList,&QListWidget::itemClicked,this,&chatwindow::onContactClicked);
    // 发送图片
    connect(ui->sendPhotoButton, &QPushButton::clicked, this, &chatwindow::sendPhotoButton_clicked);
    // 发送文件
    connect(ui->sendFileButton, &QPushButton::clicked, this, &chatwindow::sendFileButton_clicked);
    // 接收文件
    connect(ui->receiveFileButton, &QPushButton::clicked, this, &chatwindow::receiveFileButton_clicked);

}

chatwindow::~chatwindow()
{
    delete ui;
}

void chatwindow::setCurrentUsername(const QString &username)
{
    currentusername = username;
}











void chatwindow::initializeContactList()
{
    QSqlQuery query(db); // 指定数据库连接
    query.prepare("SELECT username, avatar_path FROM usernames");
    if (!query.exec()) { // 执行查询
        qDebug() << "Error loading contact list:" << query.lastError().text();
        return;}
    while (query.next()) {
        QString username = query.value(0).toString();
        QString avatarPath = ":/" + query.value(1).toString();
        if (username == currentusername) {
            continue;
        }

        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->setContentsMargins(10, 10, 10, 10);
        layout->setSpacing(10);

        QLabel *avatarLabel = new QLabel();
        avatarLabel->setFixedSize(30, 30);
        avatarLabel->setStyleSheet(QString("border-image: url(%1); border-radius: 3px;").arg(avatarPath));
        layout->addWidget(avatarLabel);

        QLabel *usernameLabel = new QLabel(username);
        usernameLabel->setObjectName("usernameLabel");
        QFont font = usernameLabel->font();
        font.setPointSize(18);
        usernameLabel->setFont(font);
        usernameLabel->setStyleSheet("color: black; background-color: rgba(0,0,0,0)");
        layout->addWidget(usernameLabel);

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(widget->sizeHint());
        ui->chatList->addItem(item);
        ui->chatList->setItemWidget(item, widget);

        widget->setStyleSheet("QWidget::hover { background-color: #c4c4c4; }"
                              "QWidget::selected { background-color: #c4c4c4; }");
    }

    if (query.lastError().isValid()) {
        qDebug() << "Error loading contact list: " << query.lastError().text();
    } else {
        qDebug() << "Contact list initialized successfully for user:" << currentusername;
    }
}




void chatwindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap background(":/C:/Users/24332/Desktop/MyQQ/backnew.png");  // 加载背景图片
    if (!background.isNull()) {
        painter.drawPixmap(rect(), background);  // 绘制背景图片
    }
}














void chatwindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标按下时的位置和窗口位置的差值
        mousePressPos = event->globalPosition().toPoint();
        windowPos = this->pos();
        diffPos = mousePressPos - windowPos;
        event->accept();
    }
}







void chatwindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        // 根据鼠标移动更新窗口位置
        QPoint newPos = event->globalPosition().toPoint();
        this->move(newPos - diffPos);
        event->accept();
    }
}











void chatwindow::readyRead_slot(QByteArray msg)
{
    QString message = QString::fromLocal8Bit(msg);
    QString username = ui->chatWindowTitleLabel->text();

    QString sender = message.section('#', 1, 1);
    QString receiver = message.section('#', 3, 3);
    QString content = message.section('#', 2, 2);
    QString contentType = message.section('#', 0, 0);
    bool isSelf = (currentusername == sender);
    if(currentusername =="")return;
    if(sender==currentusername)saveMessageToHistory(sender,receiver,content,contentType);
    //0是消息，1是图片，2是文件提示
    if (contentType == "0") {
        if ((currentusername == sender && username == receiver) || (username == sender && currentusername == receiver)) {
            QListWidgetItem *item = new QListWidgetItem();
            QWidget *messageBubble = createMessageBubble(sender, content, isSelf);
            item->setSizeHint(messageBubble->sizeHint());
            ui->ui_mag->addItem(item);
            ui->ui_mag->setItemWidget(item, messageBubble);
        }
    } else if (contentType == "1") {
        if ((username == sender && currentusername == receiver) || (currentusername == sender && username == receiver)) {
            QListWidgetItem *item = new QListWidgetItem();
            QWidget *imageBubble = createImageBubble(sender, content, isSelf);
            item->setSizeHint(imageBubble->sizeHint());
            ui->ui_mag->addItem(item);
            ui->ui_mag->setItemWidget(item, imageBubble);
        }
    } else if (contentType == "2") {
        if(receiver == currentusername)
        {
            QListWidgetItem *item = new QListWidgetItem();
            QWidget *fileBubble = createFileBubble(sender + content);
            item->setSizeHint(fileBubble->sizeHint());
            ui->ui_mag->addItem(item);
            ui->ui_mag->setItemWidget(item, fileBubble);
        }
    }

}









void chatwindow::on_btn1_clicked()
{
    QString sendMsg = ui->idt->toPlainText();
    QString username = ui->chatWindowTitleLabel->text();
    QString content = sendMsg;
    //将数据整改
    sendMsg = "0#" + currentusername + "#" + sendMsg + "#" +username;
    //将消息发送给服务器
    emit sendMsgSignal(sendMsg);
    //将编辑文本内容清空
    ui->idt->clear();
    QListWidgetItem *item = new QListWidgetItem();
    QWidget *messageBubble = createMessageBubble(currentusername, content, true);
    item->setSizeHint(messageBubble->sizeHint());
    ui->ui_mag->addItem(item);
    ui->ui_mag->setItemWidget(item, messageBubble);
    ui->ui_mag->scrollToBottom(); // 滚动到底部
}













void chatwindow::onContactClicked(QListWidgetItem *item)
{
    QWidget *widget = ui->chatList->itemWidget(item);
    QLabel *usernameLabel = widget->findChild<QLabel *>("usernameLabel");
    QString username;
    if (usernameLabel) {
        username = usernameLabel->text();
    } else {
        qDebug() << "Error: Username label not found!";
        return;
    }

    ui->btn1->setEnabled(true);
    ui->sendPhotoButton->setEnabled(true);
    ui->sendFileButton->setEnabled(true);
    ui->receiveFileButton->setEnabled(true);

    ui->chatWindowTitleLabel->setText(username);
    ui->ui_mag->clear();
    loadChatHistory(currentusername, username);

    QSqlQuery query(db);
    QString received;
    QString sender;

    query.prepare("SELECT received, username FROM files WHERE received = '0' AND username = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        qDebug() << "Error executing query:"<< "\n"
                 << "Error text:" << query.lastError().text()<< "\n"
                 << "Query:" << query.lastQuery()<< "\n"
                 << "Database error code:" << query.lastError().nativeErrorCode()<< "\n"
                 << "Username:" << username;
        return;
    }

    if (query.next()) {
        received = query.value(0).toString();
        sender = query.value(1).toString();

        if (received == "0") {
            QListWidgetItem *item = new QListWidgetItem();
            QWidget *fileBubble = createFileBubble(sender + "向你发来了新文件");
            item->setSizeHint(fileBubble->sizeHint());
            ui->ui_mag->addItem(item);
            ui->ui_mag->setItemWidget(item, fileBubble);
        }
    } else {
        qDebug() << "Error retrieving file data from database:"<<"\n"
                 << "Error text:" << query.lastError().text()<< "\n"
                 << "Query:" << query.lastQuery()<< "\n"
                 << "Database error code:" << query.lastError().nativeErrorCode()<< "\n"
                 << "Username:" << username;
    }
}











void chatwindow::sendPhotoButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择文件", "", "Images (*.png *.xpm *.jpg *.jpeg *.bmp)");
    qDebug() << "Sending file:" << filePath;
    // 检查是否选择了文件
    if (!filePath.isEmpty()) {
        // 显示文件预览
        QString username = ui->chatWindowTitleLabel->text();
        QString sendMsg = "1#" + currentusername + "#" + filePath + "#" +username;
        emit sendMsgSignal(sendMsg);}
}












void chatwindow::sendFileButton_clicked()
{
    QString username = ui->chatWindowTitleLabel->text();
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "Files (*.*)");
    qDebug() << "Sending file:" << filePath;
    // 检查是否选择了文件
    if (!filePath.isEmpty()) {
        // 显示文件预览
        QSqlQuery query(db);
        // 删除表中的所有数据
        if (!query.exec("DELETE FROM files")) {
            qDebug() << "Error clearing table:" << query.lastError().text();
        } else {
            qDebug() << "Table data cleared successfully.";
        }
        //插入数据
        query.prepare("INSERT INTO files (filepath, received, username) VALUES (:filepath, :received, :username)");
        query.bindValue(":filepath", filePath);
        query.bindValue(":received", "0");
        query.bindValue(":username", currentusername);
        if (!query.exec()) {
            qDebug() << "Error inserting data:" << query.lastError().text();
        } else {
            qDebug() << "Inserted data for filepath:" << filePath;
        }
        QString sendMsg = "2#" + currentusername + "#向你发来了新文件#" +username;
        emit sendMsgSignal(sendMsg);
    }
}










void chatwindow::receiveFileButton_clicked()
{
    QSqlQuery query(db);
    QString filePath;
    // 查询文件路径
    query.prepare("SELECT filepath FROM files");
    if (query.exec() && query.next()) {
        filePath = query.value(0).toString();
    } else {
        qDebug() << "Error retrieving file path from database:" << query.lastError().text();
    }

    if (filePath.isEmpty()) {
        qDebug() << "File path is empty. Cannot receive file.";
        return;
    }
    //改为已接收
    query.prepare("UPDATE files SET received = '1' WHERE filepath = :filepath");
    query.bindValue(":filepath", filePath);
    if (!query.exec()) {
        qDebug() << "Error updating file status:" << query.lastError().text();
    } else {
        qDebug() << "File status updated successfully.";
    }
    qDebug()<<filePath;
    QString savePath = QFileDialog::getSaveFileName(this, "Save File As", filePath, "All Files (*.*)");
    if (!savePath.isEmpty()) {
        QFile sourceFile(filePath); // 源文件路径
        QFile destFile(savePath);   // 目标文件路径

        // 打开源文件和目标文件
        if (sourceFile.open(QIODevice::ReadOnly) && destFile.open(QIODevice::WriteOnly)) {
            // 读取数据到缓冲区并写入目标文件
            while (!sourceFile.atEnd()) {
                QByteArray buf = sourceFile.read(1024); // 从源文件读取数据
                qint64 bytesWritten = destFile.write(buf); // 写入数据到目标文件
                if (bytesWritten == -1) {
                    qDebug() << "Error writing to file:" << destFile.errorString();
                    break;
                }
            }
            sourceFile.close(); // 关闭源文件
            destFile.close();   // 关闭目标文件
            qDebug() << "File saved successfully to" << savePath;
        } else {
            qDebug() << "Error opening files:" << sourceFile.errorString() << destFile.errorString();
        }
    } else {
        qDebug() << "Save file operation was canceled.";
    }
}









QWidget *chatwindow::createMessageBubble(const QString &username, const QString &message, bool isSelf)
{
    QString avatarPath = getAvatarPathFromUsername(username);



    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    QLabel *avatarLabel = new QLabel();
    avatarLabel->setFixedSize(40, 40); // 头像尺寸
    avatarLabel->setStyleSheet(QString("border-image: url(%1); border-radius: 4px;").arg(avatarPath));
    avatarLabel->setObjectName("avatarLabel");

    QWidget *messageWidget = new QWidget();
    QVBoxLayout *messageLayout = new QVBoxLayout(messageWidget);
    messageLayout->setContentsMargins(0, 0, 0, 0);
    messageLayout->setSpacing(5);

    QLabel *usernameLabel = new QLabel(username);
    usernameLabel->setObjectName("usernameLabel");
    QFont font = usernameLabel->font();
    font.setPointSize(8);
    usernameLabel->setFont(font);
    usernameLabel->setStyleSheet("color: black");
    messageLayout->addWidget(usernameLabel);

    QLabel *messageLabel = new QLabel(message);
    font.setPointSize(12);
    messageLabel->setFont(font);
    messageLabel->setWordWrap(true);
    messageLabel->setStyleSheet(QString("background-color: %1; color: black; border-radius: 10px; padding: 10px;").arg(isSelf ? "#95ec69" : "white"));
    messageLabel->setObjectName("messageLabel");
    messageLayout->addWidget(messageLabel);

    if (isSelf) {
        QHBoxLayout *rightLayout = new QHBoxLayout();
        rightLayout->addStretch();
        rightLayout->addWidget(messageWidget);
        layout->addLayout(rightLayout);
        layout->addWidget(avatarLabel);
    } else {
        layout->addWidget(avatarLabel);
        layout->addWidget(messageWidget);
        layout->addStretch();
    }

    widget->setLayout(layout);
    return widget;
}








QWidget *chatwindow::createImageBubble(const QString &username, const QString &imagePath, bool isSelf)
{
    QString avatarPath = getAvatarPathFromUsername(username);

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    QLabel *avatarLabel = new QLabel();
    avatarLabel->setFixedSize(40, 40); // 头像尺寸
    avatarLabel->setStyleSheet(QString("border-image: url(%1); border-radius: 4px;").arg(avatarPath));
    avatarLabel->setObjectName("avatarLabel");

    QLabel *usernameLabel = new QLabel(username);
    QFont font = usernameLabel->font();
    font.setPointSize(8);
    usernameLabel->setFont(font);
    usernameLabel->setStyleSheet("color: black");

    QLabel *imageLabel = new QLabel();
    QPixmap pixmap(imagePath);
    imageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
    imageLabel->setStyleSheet("border-radius: 10px;overflow: hidden;");
    imageLabel->setObjectName("imageLabel");
    imageLabel->setMinimumSize(150, 150); // 确保有显示空间
    imageLabel->setScaledContents(true);  // 自动填充缩放后的图片

    QVBoxLayout *imageLayout = new QVBoxLayout();
    imageLayout->setContentsMargins(0, 0, 0, 0);
    imageLayout->setSpacing(5);
    imageLayout->addWidget(usernameLabel);
    imageLayout->addWidget(imageLabel);

    if (isSelf) {
        layout->addStretch(1);
        layout->addLayout(imageLayout,0);
        layout->addWidget(avatarLabel);
    } else {
        layout->addWidget(avatarLabel);
        layout->addLayout(imageLayout);
    }

    widget->setLayout(layout);
    return widget;
}








QWidget *chatwindow::createFileBubble(const QString &content)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setAlignment(Qt::AlignCenter);  // 居中对齐

    QLabel *fileLabel = new QLabel(content);
    fileLabel->setStyleSheet("background-color: #dadada; color: #fdfdfd; border-radius: 10px; padding: 10px;");

    layout->addWidget(fileLabel);
    widget->setLayout(layout);

    return widget;
}








QString chatwindow::getAvatarPathFromUsername(const QString &username)
{
    QSqlQuery query(db);
    query.prepare("SELECT avatar_path FROM usernames WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        return ":/" + query.value(0).toString();  // 这里假设头像路径是相对路径
    }
    return ":/C:/Users/24332/Desktop/MyQQ/sanmao.png";  // 默认头像路径
}

void chatwindow::saveMessageToHistory(const QString &sender, const QString &receiver, const QString &content, const QString &contentType)
{
    QSqlQuery query;
    query.prepare("INSERT INTO chat_history (sender, receiver, content, contentType) VALUES (?, ?, ?, ?)");
    query.addBindValue(sender);
    query.addBindValue(receiver);
    query.addBindValue(content);
    query.addBindValue(contentType);
    query.exec();
}








void chatwindow::loadChatHistory(const QString &username1, const QString &username2)
{
    QSqlQuery query(db);
    query.prepare("SELECT sender, receiver, content, contentType FROM chat_history "
                  "WHERE (sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)");
    query.addBindValue(username1);
    query.addBindValue(username2);
    query.addBindValue(username2);
    query.addBindValue(username1);
    query.exec();
    QString username = ui->chatWindowTitleLabel->text();
    while (query.next()) {
        QString sender = query.value(0).toString();
        QString receiver = query.value(1).toString();
        QString content = query.value(2).toString();
        QString contentType = query.value(3).toString();
        bool isSelf = (currentusername == sender);
        // 使用createMessageBubble或createImageBubble函数来显示消息
        if (contentType == "0") {
            if ((currentusername == sender && username == receiver) || (username == sender && currentusername == receiver)) {
                QListWidgetItem *item = new QListWidgetItem();
                QWidget *messageBubble = createMessageBubble(sender, content, isSelf);
                item->setSizeHint(messageBubble->sizeHint());
                ui->ui_mag->addItem(item);
                ui->ui_mag->setItemWidget(item, messageBubble);
            }
        } else if (contentType == "1") {
            if ((username == sender && currentusername == receiver) || (currentusername == sender && username == receiver)) {
                QListWidgetItem *item = new QListWidgetItem();
                QWidget *imageBubble = createImageBubble(sender, content, isSelf);
                item->setSizeHint(imageBubble->sizeHint());
                ui->ui_mag->addItem(item);
                ui->ui_mag->setItemWidget(item, imageBubble);
            }
        }
    }
}








void chatwindow::on_close_clicked()
{
    close();
}

