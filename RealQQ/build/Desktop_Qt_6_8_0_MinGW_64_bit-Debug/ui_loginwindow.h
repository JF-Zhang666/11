/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginwindow
{
public:
    QPushButton *loginButton;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *close;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *loginwindow)
    {
        if (loginwindow->objectName().isEmpty())
            loginwindow->setObjectName("loginwindow");
        loginwindow->resize(407, 436);
        loginButton = new QPushButton(loginwindow);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(80, 360, 261, 51));
        layoutWidget = new QWidget(loginwindow);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(80, 190, 301, 171));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        usernameLineEdit = new QLineEdit(layoutWidget);
        usernameLineEdit->setObjectName("usernameLineEdit");
        usernameLineEdit->setMinimumSize(QSize(0, 50));

        verticalLayout->addWidget(usernameLineEdit);

        passwordLineEdit = new QLineEdit(layoutWidget);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setMinimumSize(QSize(0, 50));

        verticalLayout->addWidget(passwordLineEdit);

        close = new QPushButton(loginwindow);
        close->setObjectName("close");
        close->setGeometry(QRect(360, 10, 41, 41));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/C:/Users/24332/Desktop/MyQQ/nc.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        close->setIcon(icon);
        close->setIconSize(QSize(60, 60));
        pushButton = new QPushButton(loginwindow);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 210, 61, 51));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/C:/Users/24332/Desktop/MyQQ/groups.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(50, 50));
        pushButton_2 = new QPushButton(loginwindow);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(10, 290, 61, 51));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/C:/Users/24332/Desktop/MyQQ/lock.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_2->setIcon(icon2);
        pushButton_2->setIconSize(QSize(55, 45));

        retranslateUi(loginwindow);

        QMetaObject::connectSlotsByName(loginwindow);
    } // setupUi

    void retranslateUi(QWidget *loginwindow)
    {
        loginwindow->setWindowTitle(QCoreApplication::translate("loginwindow", "Form", nullptr));
        loginButton->setText(QCoreApplication::translate("loginwindow", "\347\231\273             \345\275\225", nullptr));
        close->setText(QString());
        pushButton->setText(QString());
        pushButton_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class loginwindow: public Ui_loginwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
