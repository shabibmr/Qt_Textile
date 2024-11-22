#ifndef LEDGERREPORT_H
#define LEDGERREPORT_H

#include <QWidget>

#include <database/Reports/ledgerhelper.h>
#include <database/finance/ledgermaster/ledgermasterdatabasehelper.h>

#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "customwidgets/gmledgersearch.h"
#include <QToolButton>

#include <finance/transactions/purchases/purchasevouchereditor.h>
#include <finance/transactions/sales/salesvouchereditor.h>
#include <finance/transactions/payments/paymentvouchereditor.h>
#include <finance/transactions/receipts/receiptvouchereditor.h>
#include <finance/transactions/journals/journalvouchereditor.h>
#include <finance/transactions/creditnote/creditnotevouchereditor.h>
#include <finance/transactions/debitnote/debitnotevouchereditor.h>


#include "counter/PrinterHelper/counterprinterhelper.h"

#include "finance/transactions/BillwiseRecords/addbillwiserecord.h"
#include "sharetools/excel/excelreporthelper.h"

#include "sharetools/sharetools.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

#include "voucherOperations/vouchereditorselector.h"


namespace Ui {
class LedgerReport;
}

class LedgerReport : public QWidget
{
    Q_OBJECT

public:
    explicit LedgerReport(QWidget *parent = 0);
    ~LedgerReport();

    QList<GeneralVoucherDataObject> voucherList;

    float openingBalance = 0;
    float crTotal = 0;
    float drTotal = 0;
    float dr =0;
    float cr =0;
    float closingBalance =0;


    LedgerMasterDataModel currentLedger;

    LedgerMasterDatabaseHelper *ledHelper;
    QSqlQueryModel *model;
    QSqlQueryModel *ledgerListModel;
    QList<QSharedPointer<LedgerMasterDataModel>> allLedgersPtr ;
    LedgerHelper *ledgerReportDbHelper;


    salesVoucherDatabaseHelper* salesHelper;
    PurchaseVoucherDatabaseHelper* purchaseHelper;
    PaymentVoucherDatabaseHelper* paymentHelper;
    ReceiptVoucherDatabaseHelper* receiptHelper;
    JournalVoucherDatabaseHelper* journalHelper;
    CreditNoteVoucherDatabaseHelper* creditHelper;
    DebitNoteVoucherDatabaseHelper* debitHelper;

    AddBillwiseRecord* billwiseWidget;


    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

    ReportsTableView *tableView;

public slots:
    void setBillwiseWidget();
private:
    Ui::LedgerReport *ui;

private slots:

    void setTableView();

    void setLedgerName(LedgerMasterDataModel ledger);

    void on_OKButton_clicked();
    void on_excelButton_clicked();
    void on_printButton_clicked();
    void on_backButton_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tabWidget_tabBarClicked(int index);
    void showShareOptions();

    void printReport(QPrinter *printer);
};

#endif // LEDGERREPORT_H
