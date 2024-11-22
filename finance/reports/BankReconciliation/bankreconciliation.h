#ifndef BANKRECONCILIATION_H
#define BANKRECONCILIATION_H

#include <QWidget>
#include <QLabel>

#include "datamodels/Counter/countersettingsdatamodel.h"

#include "customwidgets/Delegates/reportcommondelegate.h"
#include <QSqlRecord>
//#include "customwidgets/test/gmitemsortfilterproxymodel.h"
//#include "customwidgets/gmquerysortfilterproxymodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "sharetools/sharetools.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"

#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialtabs.h"

#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "database/finance/banking/chequedetailsdatabasehelper.h"
#include "customwidgets/Delegates/Reconciliation/gmbankreconciliationdelegate.h"
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/Models/gmsqlquerymodel.h"

namespace Ui {
class BankReconciliation;
}

class BankReconciliation : public QWidget
{
    Q_OBJECT

public:
    explicit BankReconciliation(QWidget *parent = nullptr);
    ~BankReconciliation();

    QString reportName = "Bank Reconciliation";
    QLabel *windowLabel;
    QString bgcolor = "#ffffff";
    QString textColor = "#006064";
    QString selectionColor = "#e0f7fa";
    QString highlightedColor = "#00acc1";
    QString headerBgColor= "#00bcd4"; //#0082c8
    QString iconColor = "#00bcd4";
    QString titleColor = "#00bcd4";
    QString appBarColor = "white";
    QSize iconSize = QSize(18,18);
    QFont normalFont = QFont("Calibri", 9, QFont::Normal);
    QFont boldFont = QFont("Calibri", 10, QFont::Bold);
    QFont titleFont = QFont("Calibri", 18, QFont::Normal);

    QtMaterialAppBar *appBar;
    QtMaterialIconButton *shareButton;
    QtMaterialIconButton *okButton;
    QPointer<GMDateEdit> fromDateEdit;
    QPointer<GMDateEdit> toDateEdit;

    QString ledgerId;
    GMSQLQUeryModel* model;
    QSqlQueryModel *qModel;
    ReportsTableView *tableView;
//    GMQuerySortFilterProxyModel* sort;
    GMLedgerSearchLite *ledSearch;
    QLineEdit *filterLineEdit;
    QCheckBox *showReconciled;
    QCheckBox *showAll;
    QCheckBox *showUnReconciled;

    CounterPrinterHelper *printerHelper;
    ShareTools *shareOptionsWidget;

    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();

    QWidget *fromDateWidget;
    QWidget *toDateWidget;

    ChequeDetailsDatabaseHelper *chqHelper;

    LedgerHelper *dbHelper;
    LedgerMasterDatabaseHelper *ledHelper ;

private slots:
    void initDbHelpers();
    void initGlobalObjects();
    void setWidgets();
    void setTableView();
    void closeWindow();
    void showShareOptions();
    void printReport(QPrinter *printer);
    void exportToExcel();

    void updateStatus(int row, bool Stat);
private:
    Ui::BankReconciliation *ui;
};

#endif // BANKRECONCILIATION_H
