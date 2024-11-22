#ifndef ATTENDANCEEDITOR_H
#define ATTENDANCEEDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QToolButton>
#include <QTableWidget>
#include <QPushButton>

#include "customwidgets/TableViews/employeetableview.h"
#include "customwidgets/Models/gmemployeemodel.h"
//#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
//#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchwidget.h"
#include "customwidgets/Delegates/Attendance/gmattendancedelegate.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
//#include "customwidgets/Material/components/qtmaterialsnackbar.h"
//#include "customwidgets/Material/components/qtmaterialdrawer.h"
//#include "customwidgets/Material/components/qtmaterialiconbutton.h"
//#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "database/hr/attendancedatabasehelper.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"

#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"



namespace Ui {
class AttendanceEditor;
}

class AttendanceEditor : public QWidget
{
    Q_OBJECT

    QString windowTitle="Attendance";
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;

    AttendanceDataModel  *obj;
   AttendanceListDataModel *attendanceList;
    AttendanceDatabaseHelper *dbHelper;
    UserProfileDatabaseHelper *userHelper;

    EmployeeTableView *DataTableWidget;                 //X

    GMEmployeeSearchWidget *employeeSearchBox;

//    QToolButton *saveButton;
    QToolButton *rejectButton;
    QToolButton *backButton;
    QToolButton *shareButton;
    QToolButton *printButton;
    QVBoxLayout *appBar;
    QToolButton *importButton;
    QtMaterialFloatingActionButton *saveButton;
//    QtMaterialSnackbar *snackBar;
//    QtMaterialDrawer *drawer;


    QPointer<GMDateEdit> voucherDateWidget;
//    QPointer<GMLineEdit> voucherNumberWidget;
    QPointer<GMPlainTextEdit> voucherNarrationWidget;
    QPointer<GMEmployeeModel> model;
    QPointer<QTableWidget> tableWidget;

    PrinterHelper *printHelper;

    QStringList lis;


public:
    explicit AttendanceEditor(QWidget *parent = nullptr);
    ~AttendanceEditor();


    void initExtraActions();

    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

    void setWidgets();

    void resetWidgets();

public slots :
    void showVoucher(AttendanceListDataModel *obj);
    void closeEditor();
    void saveVoucher();

signals:
    void closing();

private slots:
//    void on_toolButton_clicked();

private:
    Ui::AttendanceEditor *ui;
};

#endif // ATTENDANCEEDITOR_H
