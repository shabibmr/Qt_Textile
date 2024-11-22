#ifndef VATREPORT_H
#define VATREPORT_H

#include <QWidget>
#include "database/Reports/vatreportdatabasehelper.h"
#include "database/Reports/ledgerhelper.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include <QSqlRecord>
#include <customwidgets/Delegates/billwisereportdelegate.h>
#include "database/finance/Closing/shiftdatabasehelper.h"

namespace Ui {
class VATReport;
}

class VATReport : public QWidget
{
    Q_OBJECT

public:
    explicit VATReport(QWidget *parent = 0);
    ~VATReport();

    QSqlQueryModel *salesModel;
    QSqlQueryModel *purchaseModel;
    LedgerHelper *ledHelper;
    GMItemSortFilterProxyModel *sortSales;
    GMItemSortFilterProxyModel *sortPurchase;
    VatReportDatabaseHelper *dbHelper;
public slots:
    void setTable();

    void setTableAll();

    void setTableSales();

    void setTablePurchase();

    void setTableCreditNote();

    void setTableDebitNote();

private slots:
    void on_backButton_clicked();

    void on_OKButton_clicked();

private:
    Ui::VATReport *ui;
};

#endif // VATREPORT_H
