#ifndef PENDINGORDERS_H
#define PENDINGORDERS_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "inventory/transactions/salesorders/salesordervouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

#include "deliveryordersmodel.h"

#include <QComboBox>
#include "finance/transactions/sales/salesvouchereditor.h"

namespace Ui {
class PendingOrders;
}

class PendingOrders : public QWidget
{
    Q_OBJECT

public:
    explicit PendingOrders(QWidget *parent = 0);
    ~PendingOrders();
    SalesOrderVoucherEditor* AddSalesOrderWidget;
    QList<GeneralVoucherDataObject> voucherList;

    UserProfileDatabaseHelper *userHelper;
    SalesOrderDatabaseHelper *dbhelper;
    PricelistDatabaseHelper *pHelper;
    AddressBookDatabaseHelper *addHelper;
    salesVoucherDatabaseHelper *salesHelper;
    GodownDatabaseHelper *godownHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    UserProfileDatabaseHelper *empHelper;

    QSqlQueryModel *model;
    DeliveryOrdersModel *model2;
    QSqlQueryModel *itemwiseModel;
    GMItemSortFilterProxyModel *sort;
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

    QTableView *tableView;

    QLineEdit *totalLine;



    QComboBox *empCombo;
    QComboBox *timeCombo;
    QComboBox *serviceCombo;
    QComboBox *routeCombo;
    QComboBox *statusCombo;


    GeneralVoucherDataObject *voucher = nullptr;

    QString prevVoucher,prevPrefix;

    CounterPrinterHelper *printer;

     void setTableParams();

     void showShareOptions2();
public slots:

    void setDBHelpers();
    void setHeading(QString label);


    void setTableView();

    void showBill();
    void on_excelButton_clicked();
    void printReportSlot(QPrinter *printer);

private slots:
    void on_okPushButton_clicked();
    void delButtonClicked(bool);
    void on_tableView_doubleClicked(const QModelIndex &index);

    void setFilters();
    void askForPrint();
    void printReport(QPrinter *printer);
    void showShareOptions();

    void on_printPushButton_clicked();

    void printKOT(QString voucherNo,QString vPref);

    void saveSalesVoucher(QString voucherNo,QString vPref);

    void printSalesOrder();

    void on_tableView_clicked(const QModelIndex &index);

    void on_printSOButton_clicked();

    void on_printDetailed_clicked();

    void on_editButton_clicked();
signals:
    void selectedVoucher(GeneralVoucherDataObject *obj);
private:
    Ui::PendingOrders *ui;
};

#endif // PENDINGORDERS_H
