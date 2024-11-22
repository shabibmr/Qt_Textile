#ifndef GSTREPORT_H
#define GSTREPORT_H

#include <QWidget>
#include "database/Reports/vatreportdatabasehelper.h"
#include "database/Reports/ledgerhelper.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include <QSqlRecord>
#include <customwidgets/Delegates/reportcommondelegate.h>
#include "hsnreportdelegate.h"

#include "database/finance/Closing/shiftdatabasehelper.h"
#include "customwidgets/TableViews/reportstableview.h"

namespace Ui {
class GSTReport;
}

class GSTReport : public QWidget
{
    Q_OBJECT

public:
    explicit GSTReport(QWidget *parent = 0);
    ~GSTReport();

    QSqlQueryModel *salesModel;
    QSqlQueryModel *b2cModel;
    QSqlQueryModel *purchaseModel;
    QSqlQueryModel *debitNoteModel;
    QSqlQueryModel *creditNoteModel;
    QSqlQueryModel *hsnModel;
    LedgerHelper *ledHelper;
    GMItemSortFilterProxyModel *sortSales;
    GMItemSortFilterProxyModel *sortB2CSales;
    GMItemSortFilterProxyModel *sortPurchase;
    GMItemSortFilterProxyModel *sortDebit;
    GMItemSortFilterProxyModel *sortCredit;
    GMItemSortFilterProxyModel *sortHSN;
    VatReportDatabaseHelper *dbHelper;

    QMap<QString, float> ledMap;

    ReportsTableView *salesTableView;
    ReportsTableView *b2csalesTableView;
    ReportsTableView *purchaseTableView;
    ReportsTableView *creditNoteTableView;
    ReportsTableView *debitNoteTableView;
    ReportsTableView *hsnTableView;
    ReportsTableView *cessTableView;

    bool loadedAllGst = false;
    bool loadedSales = false;
    bool loadedb2cSales = false;
    bool loadedPurchase = false;
    bool loadedCreditNotes = false;
    bool loadedDebitNotes = false;
    bool loadedHsn = false;
    bool loadedCess = false;

public slots:
    void setTable();
    void setTableb2bSales();
    void setTableb2cSales();

    void setTableAll();

    void setTableSales();

    void setTablePurchase();

    void setTableCreditNote();

    void setTableDebitNote();

    void setHsnTable();

    void setCessReport();

private slots:
    void on_backButton_clicked();

    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::GSTReport *ui;
};

#endif // GSTREPORT_H
