#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "database/hr/userprofiledatabasehelper.h"
#include "database/addressbook/companyprofiledatabasehelper.h"
#include "database/Counter/countersettingsdatabasehelper.h"
#include "addressbook/companyprofilewidget.h"
#include "loginvalues.h"
#include "mainwindow.h"
#include "counter/MainScreen/restuarantcountermainwindow.h"
#include "CommonUi/uimain.h"
#include <QNetworkInterface>
#include "folder/lineediticon.h"

#include "customwidgets/Material/components/qtmaterialflatbutton.h"


#include "customwidgets/Misc/gmcpbar.h"


namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = 0);
    ~LoginPage();
    UserProfileDatabaseHelper *userHelper;
    QString getMACID();

    CompanySelectorWidget *compSelector;
    QGraphicsDropShadowEffect *usernameEffect;
    QGraphicsDropShadowEffect *passwordEffect;

    QtMaterialFlatButton mLogin;
    QtMaterialFlatButton mCancel;
    QPointer<QLabel> compLable;
    QPointer<QHBoxLayout> lay;



    QPropertyAnimation *animation;




    void setLogoAnimation();
    bool debugMode = false;

    void setCompanyName(QString compName="");

    void setCompSelector(CompanySelectorWidget *value);

private slots:
    void on_username_returnPressed();

    void on_password_returnPressed();

    void on_cancel_clicked();

    void on_loginButton_clicked();

    void on_companyNameLabel_clicked();

signals:

    void cloes();
    void loginSuccess();
    void showUiMain();
    void loginCancelled();

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H

