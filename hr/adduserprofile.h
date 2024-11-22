#ifndef ADDUSERPROFILE_H
#define ADDUSERPROFILE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTextEdit>
#include "datamodels/hr/userprofiledatamodel.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "database/hr/payslipdatabasehelper.h"
#include "database/hr/usergroupdatabasehelper.h"
#include "Notifications/notificationledgerslist.h"
#include "database/hr/attendancedatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

namespace Ui {
class AddUserProfile;
}

class AddUserProfile : public QWidget
{
    Q_OBJECT

public:
    explicit AddUserProfile(QWidget *parent = 0);
    ~AddUserProfile();
    int vNo=0;
    UserProfileDataModel* user;
    EmployeeSalaryDataModel *emp;
    UserProfileDatabaseHelper *dbHelper;
    UserGroupDatabaseHelper *gpHelper;
    QMap<int,QString> userGroupMap;

    PayslipDatabaseHelper *payslipHelper;
    LedgerMasterDatabaseHelper *ledHelper;

    void disableSave();



public slots:
    void saveEmployee(UserProfileDataModel*);
    void showVoucher(UserProfileDataModel voucher);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void sendNotifications(QStringList);
signals:
    void closing();

private:
    Ui::AddUserProfile *ui;
};

#endif // ADDUSERPROFILE_H
