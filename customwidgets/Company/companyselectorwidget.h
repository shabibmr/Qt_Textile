#ifndef COMPANYSELECTORWIDGET_H
#define COMPANYSELECTORWIDGET_H

#include <QWidget>
#include <QPair>
#include <QSettings>
#include <QCheckBox>

#include "database/addressbook/companyprofiledatabasehelper.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include "customwidgets/Material/components/qtmaterialflatbutton.h"
namespace Ui {
class CompanySelectorWidget;
}

class CompanySelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompanySelectorWidget(bool isRestart = false, QWidget *parent = nullptr);
    ~CompanySelectorWidget();


    DatabaseValues dv;
    CompanyProfileDataBaseHelper* dbHelper;
    QList<QStringList> cList ;
    QCheckBox *box;

    void initDBHelpers();
    QMap<QString, QPair<QString,QString>> cMap;

    QString defaultDBKey = "DefaultDBName";
    static QStringList loggedInBranches;
    QMap<QString, int> branchUserMap;
    bool defaultDbFound=false;

    void setView();
    QString getCompanyName(QString dbName);

    QString getDbName(QString dbStr);

    bool selectDefaultBranch();
    void switchDB(QString dbName);
    void setDefaultBranch(QString dbName);
    int getLoggedInUserByBranch(QString dbName);

//    void showLoginPage();
public slots:
    void addLoggedBranch();
private:
    Ui::CompanySelectorWidget *ui;

signals:
    void companySwitched(QString)    ;
    void updateDB();
    void loginRequired(QString);
};

#endif // COMPANYSELECTORWIDGET_H
