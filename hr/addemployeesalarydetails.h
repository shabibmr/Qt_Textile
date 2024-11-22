#ifndef ADDEMPLOYEESALARYDETAILS_H
#define ADDEMPLOYEESALARYDETAILS_H

#include "datamodels/hr/employeesalarydatamodel.h"
#include "database/hr/payslipdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include <QWidget>

namespace Ui {
class AddEmployeeSalaryDetails;
}

class AddEmployeeSalaryDetails : public QWidget
{
    Q_OBJECT

public:
    explicit AddEmployeeSalaryDetails(QWidget *parent = nullptr);
    ~AddEmployeeSalaryDetails();

    EmployeeSalaryDataModel *emp;
    PayslipDatabaseHelper *dbHelper;

    GMLedgerSearchLite *salaryLedger;
    GMLedgerSearchLite *advanceLedger;
    LedgerMasterDatabaseHelper *ledgerHelper;

    QSqlQueryModel *ledModel;

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void showData();

signals:
    void closing();

private:
    Ui::AddEmployeeSalaryDetails *ui;
};

#endif // ADDEMPLOYEESALARYDETAILS_H
