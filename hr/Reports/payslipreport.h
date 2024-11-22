#ifndef PAYSLIPREPORT_H
#define PAYSLIPREPORT_H

#include <QWidget>
#include "database/hr/attendancedatabasehelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "finance/transactions/payments/paymentvouchereditor.h"

namespace Ui {
class PayslipReport;
}

class PayslipReport : public QWidget
{
    Q_OBJECT

public:
    explicit PayslipReport(QWidget *parent = nullptr);
    ~PayslipReport();

    AttendanceDatabaseHelper *dbHelper;
    PayslipDatabaseHelper *payslipHelper;
    QSqlQueryModel* model;
    GMItemSortFilterProxyModel* sort;

    ReportsTableView *tableView;

    void setTable();

private slots:
    void setPaymentVoucher(QModelIndex index)    ;

private:
    Ui::PayslipReport *ui;
};

#endif // PAYSLIPREPORT_H
