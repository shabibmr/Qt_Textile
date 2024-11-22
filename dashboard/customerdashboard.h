#ifndef CUSTOMERDASHBOARD_H
#define CUSTOMERDASHBOARD_H

#include <QWidget>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>

#include "customwidgets/Basic/gmcardwidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

namespace Ui {
class CustomerDashboard;
}

class CustomerDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerDashboard(LedgerHelper *ledgerHelper = new LedgerHelper,QWidget *parent = nullptr);
    ~CustomerDashboard();

    QHBoxLayout *filterLayout;
    QVBoxLayout *ledgerLayout;

    QVBoxLayout *ledgerInfo1;
    QHBoxLayout *allCustLayout;

    QWidget *ledgerDetails;


    LedgerMasterDatabaseHelper *ledgerHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    LedgerHelper *dbHelper;


    int outStandingBillsCount;
    float overDueAmount;
    QString firstOutsDate;

    LedgerMasterDataModel *ledger;
    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();

    QString bgcolor = "#ffffff";
    QString textColor = "#006064";
    QString iconColor = "#00bcd4";
    QString tableViewSS = "QTableView::item{"
                          "gridline-color : #ffffff;"
                          "border: 0px solid #fffff8;"
                          "}"
                          "QTableView::item:selected{"
                          "color:#00acc1;"
                          "background-color:" + bgcolor + ";"
                          "}"
                          "QTableView{"
                          "background-color: #ffffff;"
                          "color: #006064;"
                          "border : none;"
                          "/*selection-background-color: #999*/}"
                          "QHeaderView::section:horizontal"
                          "{"
                          "background-color: #006064;"
                          "color: white;"
                          "border: 1px solid #ffffff;"
                           "}";

    QTableView *avgsTable;
    QTableView *topItems;
    QTableView *last10;
    GMCardWidget *rankCard;
    GMCardWidget *totalLedSalesCard;
    GMCardWidget *outstandingCard;
    GMCardWidget *outstandingDateCard;
    GMCardWidget *avgCreditPeriodCard;


    void initDbHelpers();
    void setData();
    void setWidgets();
    void setFilterLayout();
    void setLedgerLayout();

    void setAllCustomersLayout();

    void calculateLedgerValues();
    void setLedgerValues();
    void setLedgerMasterInfo();

private slots:
    void setValues();



private:
    Ui::CustomerDashboard *ui;
};

#endif // CUSTOMERDASHBOARD_H
