#ifndef PASSWORDENTRY_H
#define PASSWORDENTRY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <login/loginvalues.h>

namespace Ui {
class PasswordEntry;
}

class PasswordEntry : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordEntry(QWidget *parent = 0);
    ~PasswordEntry();

    void hideWidgets();
    bool kotDeleted = false;

    void setKotDeleted(bool value);

private slots:
    void on_OKButton_clicked();

    void on_cancelButton_clicked();

    void on_passwordLineEdit_returnPressed();



    void on_itemwiseButton_clicked();

    void on_todaysbillsButton_clicked();

    void on_pushButton_clicked();



signals:
    void loginSuccess();

    void loginSuccessWithComment(QString username,QString remarks, int);

    void loginFail();

    void showtodaysBills();

    void showItemwiseSales();

    void sendEmailReport();

    void loginSuccessWithPrivilege(int);

private:
    Ui::PasswordEntry *ui;
};

#endif // PASSWORDENTRY_H
