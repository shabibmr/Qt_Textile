#ifndef BILLWISEREPORT_H
#define BILLWISEREPORT_H

#include <QWidget>
#include "database/voucherdbabstract.h"
#include "database/Reports/ledgerhelper.h"
//#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Delegates/billwisereportdelegate.h"
#include <QSqlRecord>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/modeofpay.h"
#include "sharetools/excel/excelreporthelper.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/filterwidget.h"

namespace Ui {
class BillwiseReport;
}

class BillwiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit BillwiseReport(QWidget *parent = 0);
    ~BillwiseReport();

    FilterWidget *filterWid;

    VoucherDBAbstract *salesHelper;
    QList<GeneralVoucherDataObject> voucherList;

    LedgerHelper* ledHelper;

    LedgerMasterDatabaseHelper *ledgerHelper;
    QSqlQueryModel* model;

    bool deleted = false;
    GeneralVoucherDataObject *voucher;

    GMItemSortFilterProxyModel *sort;

    void setValueTable();
    void setBillPreview();

    void switchModeOfPay();

    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;
    ReportsTableView *tableView;
    void setSalesHelper(VoucherDBAbstract *value);

    void setDeleted(bool value);


public slots:
    void calcTotals();
    void disableDelete();
signals:

    void selectedVoucher(GeneralVoucherDataObject *obj);

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void on_amountWiseTableView_clicked(const QModelIndex &index);

    void on_printCopy_clicked();

    void on_modePay_clicked();
    void on_deleteButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);

    void on_amountWiseTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::BillwiseReport *ui;
};

#endif // BILLWISEREPORT_H
