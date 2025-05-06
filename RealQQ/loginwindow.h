#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class loginwindow;
}

class loginwindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = nullptr);
    ~loginwindow();
signals:
    void loginSuccessful(const QString &username);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override ;

private slots:
    void on_loginButton_clicked();

    void on_close_clicked();

private:
    Ui::loginwindow *ui;
    void authenticateUser(const QString &username, const QString &password);
    QPoint mousePressPos;  // 鼠标按下时的全局位置
    QPoint windowPos;      // 窗口当前位置
    QPoint diffPos;
};

#endif // LOGINWINDOW_H
