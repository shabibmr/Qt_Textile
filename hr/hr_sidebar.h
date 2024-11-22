#ifndef HR_SIDEBAR_H
#define HR_SIDEBAR_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include "customwidgets/section.h"
#include <QPushButton>
#include "inventory/masters/sales_inventory_items/additems.h"
#include "hr/attendanceeditor.h"
#include "hr/Reports/attendancereport.h"
#include "hr/Reports/payslipreport.h"
#include "hr/Reports/passportexpirylist.h"
#include "hr/Reports/visaexpiryreport.h"

#include "QToolButton"
#include "customwidgets/defaultdashboard.h"
#include "customwidgets/section.h"
#include <QDebug>
#include "hr/usergroupslist.h"
#include "hr/userslist.h"

namespace Ui {
class HR_SideBar;
}

class HR_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit HR_SideBar(QWidget *parent = 0);
    ~HR_SideBar();
    DefaultDashBoard *dash;

    QToolButton* UserGroups;
    QToolButton* UserProfiles;
    QToolButton* Attendance;


    QToolButton* VisaExpiry;
    QToolButton* PaspportExpiry;

    QToolButton* AttendanceReportButton;
    QToolButton* PayslipReportButton;

    UserGroupsList* UserGroupsWidget;
    UsersList* UserProfilesWidget;

    AttendanceEditor* attendanceEditorWidget;
    AttendanceReport* AttendanceReportWidget;
    PayslipReport* PayslipReportWidget;


public slots:
    void clearLayout();
    void resetColors();
    void UserGroupsClicked();
    void UserProfilesClicked();
    void attendanceClicked();

    void attendanceReportClicked();
    void payslipReportClicked();

    void passcli();
    void visacli();
private:
    Ui::HR_SideBar *ui;
};

#endif // HR_SIDEBAR_H
