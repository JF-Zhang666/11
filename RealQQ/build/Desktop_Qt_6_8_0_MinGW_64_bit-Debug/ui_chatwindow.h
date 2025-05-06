/********************************************************************************
** Form generated from reading UI file 'chatwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_chatwindow
{
public:
    QLabel *chatWindowTitleLabel;
    QPushButton *btn1;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *sendPhotoButton;
    QPushButton *sendFileButton;
    QPushButton *receiveFileButton;
    QPushButton *close;
    QListWidget *chatList;
    QListWidget *ui_mag;
    QTextEdit *idt;

    void setupUi(QWidget *chatwindow)
    {
        if (chatwindow->objectName().isEmpty())
            chatwindow->setObjectName("chatwindow");
        chatwindow->resize(618, 469);
        chatWindowTitleLabel = new QLabel(chatwindow);
        chatWindowTitleLabel->setObjectName("chatWindowTitleLabel");
        chatWindowTitleLabel->setGeometry(QRect(10, 10, 121, 19));
        btn1 = new QPushButton(chatwindow);
        btn1->setObjectName("btn1");
        btn1->setGeometry(QRect(12, 310, 161, 91));
        layoutWidget = new QWidget(chatwindow);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(210, 249, 381, 61));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        sendPhotoButton = new QPushButton(layoutWidget);
        sendPhotoButton->setObjectName("sendPhotoButton");

        horizontalLayout->addWidget(sendPhotoButton);

        sendFileButton = new QPushButton(layoutWidget);
        sendFileButton->setObjectName("sendFileButton");

        horizontalLayout->addWidget(sendFileButton);

        receiveFileButton = new QPushButton(layoutWidget);
        receiveFileButton->setObjectName("receiveFileButton");

        horizontalLayout->addWidget(receiveFileButton);

        close = new QPushButton(chatwindow);
        close->setObjectName("close");
        close->setGeometry(QRect(570, 0, 41, 31));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/C:/Users/24332/Desktop/MyQQ/nc.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        close->setIcon(icon);
        close->setIconSize(QSize(40, 40));
        chatList = new QListWidget(chatwindow);
        chatList->setObjectName("chatList");
        chatList->setGeometry(QRect(10, 41, 181, 211));
        ui_mag = new QListWidget(chatwindow);
        ui_mag->setObjectName("ui_mag");
        ui_mag->setGeometry(QRect(210, 41, 381, 211));
        idt = new QTextEdit(chatwindow);
        idt->setObjectName("idt");
        idt->setGeometry(QRect(210, 310, 381, 91));

        retranslateUi(chatwindow);

        QMetaObject::connectSlotsByName(chatwindow);
    } // setupUi

    void retranslateUi(QWidget *chatwindow)
    {
        chatwindow->setWindowTitle(QCoreApplication::translate("chatwindow", "Form", nullptr));
        chatWindowTitleLabel->setText(QString());
        btn1->setText(QCoreApplication::translate("chatwindow", "\345\217\221       \351\200\201", nullptr));
        sendPhotoButton->setText(QCoreApplication::translate("chatwindow", "\345\217\221\351\200\201\345\233\276\347\211\207", nullptr));
        sendFileButton->setText(QCoreApplication::translate("chatwindow", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
        receiveFileButton->setText(QCoreApplication::translate("chatwindow", "\346\216\245\346\224\266\346\226\207\344\273\266", nullptr));
        close->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class chatwindow: public Ui_chatwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
