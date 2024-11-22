#ifndef PAYSLIPGENERATIONEDITOR_H
#define PAYSLIPGENERATIONEDITOR_H

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

#include "database/hr/payslipdatabasehelper.h"
#include "customwidgets/TableViews/paysliptableview.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


namespace Ui {
class PayslipGenerationEditor;
}

class PayslipGenerationEditor : public QWidget
{
    Q_OBJECT

    QString windowTitle="Attendance";
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;

    PayslipDataModel  *obj;
    PayslipDatabaseHelper *dbHelper;

    PayslipTableView *DataTableWidget;                 //X

//    GMEmployeeSearchWidget *employeeSearchBox;

    QToolButton *saveButton;
//    QToolButton *rejectButton;
//    QToolButton *backButton;
//    QToolButton *shareButton;
//    QToolButton *printButton;
////    QVBoxLayout *appBar;
//    QToolButton *importButton;

//    QtMaterialSnackbar *snackBar;
//    QtMaterialDrawer *drawer;


    QPointer<GMDateEdit> voucherDateWidget;
//    QPointer<GMLineEdit> voucherNumberWidget;
//    QPointer<GMPlainTextEdit> voucherNarrationWidget;
//    QPointer<GMEmployeeModel> model;
//    QPointer<QTableWidget> tableWidget;

//    PrinterHelper *printHelper;

    QStringList lis;


public:
    explicit PayslipGenerationEditor(QWidget *parent = nullptr);
    ~PayslipGenerationEditor();
    void initExtraActions();

    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

    void setWidgets();

    void resetWidgets();

public slots :
    void showVoucher(PayslipDataModel *obj);
    void closeEditor();
    void saveVoucher();
    void rejectVoucher();
    void deleteVoucher();
    void askForPrint();
    void printVoucher();

    void setTable();
signals:
    void closing();

private:
    Ui::PayslipGenerationEditor *ui;
};

#endif // PAYSLIPGENERATIONEDITOR_H
