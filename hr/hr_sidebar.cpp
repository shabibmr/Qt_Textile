#include "hr_sidebar.h"
#include "ui_hr_sidebar.h"

HR_SideBar::HR_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HR_SideBar)
{
    ui->setupUi(this);
    dash = new DefaultDashBoard(this);
    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");

    Section* sectionAdmin = new Section("Admin", 300,new QIcon(":/icons/gm.ico"), this);
    Section* sectionReports = new Section("Reports", 300,new QIcon(":/icons/gm.ico"), this);

    UserGroups = new QToolButton(this);
    UserProfiles = new QToolButton(this);
    Attendance = new QToolButton(this);
    PaspportExpiry = new QToolButton(this);
    VisaExpiry = new QToolButton(this);

    AttendanceReportButton = new QToolButton(this);
    PayslipReportButton = new QToolButton(this);

    ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    //    ui->verticalLayout_3->addWidget(UserGroups);
    //    ui->verticalLayout_3->addWidget(UserProfiles);
    //    ui->verticalLayout_3->addWidget(Attendance);
    //    ui->verticalLayout_3->addWidget(sectionReports);

    UserGroups->setText("User Groups");
    UserProfiles->setText("User Profiles");
    Attendance->setText("Attendance");
    AttendanceReportButton->setText("Attendance Report");
    PayslipReportButton->setText("Payslip Report");
    PaspportExpiry->setText("Passport expiry");
    VisaExpiry->setText("Visa expiry");


    QFont font;
    font.setPointSize(10);

    UserGroups->setFont(font);
    UserProfiles->setFont(font);
    Attendance->setFont(font);
    AttendanceReportButton->setFont(font);
    PayslipReportButton->setFont(font);
    PaspportExpiry->setFont(font);
    VisaExpiry->setFont(font);

    PaspportExpiry->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    VisaExpiry->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    UserGroups->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    UserProfiles->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Attendance->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    AttendanceReportButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PayslipReportButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    auto* reportsLayout  = new QVBoxLayout();
    reportsLayout->addWidget(AttendanceReportButton);
    reportsLayout->addWidget(PayslipReportButton);
    reportsLayout->addWidget(PaspportExpiry);
    reportsLayout->addWidget(VisaExpiry);


    auto* adminLayout = new  QVBoxLayout();
    adminLayout->addWidget(UserProfiles);
    adminLayout->addWidget(UserGroups);


    sectionReports->setContentLayout(*reportsLayout);

    sectionAdmin->setContentLayout(*adminLayout);


    ui->verticalLayout_3->addWidget(Attendance);
    ui->verticalLayout_3->addWidget(sectionReports);
    ui->verticalLayout_3->addWidget(sectionAdmin);

    QObject::connect(UserGroups,SIGNAL(clicked(bool)),this,SLOT(UserGroupsClicked()));
    QObject::connect(UserProfiles,SIGNAL(clicked(bool)),this,SLOT(UserProfilesClicked()));
    QObject::connect(Attendance,SIGNAL(clicked(bool)),this,SLOT(attendanceClicked()));
    QObject::connect(AttendanceReportButton,SIGNAL(clicked(bool)),this,SLOT(attendanceReportClicked()));
    QObject::connect(PayslipReportButton,SIGNAL(clicked(bool)),this,SLOT(payslipReportClicked()));
    QObject::connect(PaspportExpiry,SIGNAL(clicked(bool)),this,SLOT(passcli()));
    QObject::connect(VisaExpiry,SIGNAL(clicked(bool)),this,SLOT(visacli()));


    resetColors();
    ui->widgetLayout->addWidget(dash);

}

HR_SideBar::~HR_SideBar()
{
    delete ui;
}

void HR_SideBar::clearLayout()
{
    if (ui->widgetLayout != NULL )
    {

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }
}

void HR_SideBar::resetColors()
{
    UserGroups->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    UserProfiles->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Attendance->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    AttendanceReportButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PayslipReportButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PaspportExpiry->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    VisaExpiry->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");


    clearLayout();
}

void HR_SideBar::UserGroupsClicked()
{
    resetColors();
    UserGroups->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    UserGroupsWidget = new UserGroupsList(this);
    ui->widgetLayout->addWidget(UserGroupsWidget);
}

void HR_SideBar::UserProfilesClicked()
{
    resetColors();
    UserProfiles->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    UserProfilesWidget = new UsersList(this);
    ui->widgetLayout->addWidget(UserProfilesWidget);
}

void HR_SideBar::attendanceClicked()
{
    resetColors();
    Attendance->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    attendanceEditorWidget = new AttendanceEditor(this);
    qDebug()<<"Show Attendance";
    AttendanceDatabaseHelper *atHelper = new AttendanceDatabaseHelper();
    attendanceEditorWidget->showVoucher(atHelper->getAttendanceListByDate(QDate::currentDate()));
    ui->widgetLayout->addWidget(attendanceEditorWidget);
}


void HR_SideBar::attendanceReportClicked()
{
    resetColors();
    AttendanceReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    AttendanceReportWidget = new AttendanceReport(this);
    ui->widgetLayout->addWidget(AttendanceReportWidget);
}

void HR_SideBar::payslipReportClicked()
{
    resetColors();
    PayslipReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PayslipReportWidget = new PayslipReport(this);
    ui->widgetLayout->addWidget(PayslipReportWidget);
}

void HR_SideBar::passcli()
{
    resetColors();
    PaspportExpiry->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PassportExpiryList *p = new PassportExpiryList(this);
    ui->widgetLayout->addWidget(p);
}

void HR_SideBar::visacli()
{
    resetColors();
    VisaExpiry->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    VisaExpiryReport *p = new VisaExpiryReport(this);
    ui->widgetLayout->addWidget(p);
}
