#ifndef RESTAURANTSALESORDERSREPORT_H
#define RESTAURANTSALESORDERSREPORT_H

#include <QRadioButton>
#include <QWidget>

#include <database/finance/reports/reportsdatabasehelper.h>
#include "database/finance/salesvoucher/salesvoucherdeleteddatabasehelper.h"
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/assignsalesman.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QMessageBox>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "sharetools/sharetools.h"
#include "customwidgets/TableViews/reportstableview.h"

namespace Ui {
class RestaurantSalesOrdersReport;
}

class RestaurantSalesOrdersReport : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantSalesOrdersReport(salesVoucherDeletedDatabaseHelper *deletedSalesHelper,
                                         SalesOrderDatabaseHelper *soHelper,
                                         AccountGroupMasterDatabaseHelper *accHelper,
                                         LedgerMasterDatabaseHelper *ledHelper,
                                         QWidget *parent = Q_NULLPTR);
    ~RestaurantSalesOrdersReport();

    SalesOrderDatabaseHelper *salesOrderHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    salesVoucherDeletedDatabaseHelper *deletedSalesHelper;
    GeneralVoucherDataObject *voucher;
    QList<GeneralVoucherDataObject> voucherList;
    UserProfileDatabaseHelper userHelper;
    PricelistDatabaseHelper pHelper;
    QMap<int, QString> employees;
    QList<PriceListDataObject> prices;
    QStringList voucherNos;
    int plistID=0;

    QList<QRadioButton*> radioList;

    bool multiVoucher = true;

    QDialog *passWordShiftDialog;
    QLabel *textLabel;
    QLineEdit* passwordShiftLine;

    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;
    ShareTools *shareOptionsWidget;

    QString previousTable="";

    QTableView *tableView;


    void MailDeletedList(QString name);


public slots:
    void showDateWidget(bool v);
    void setPlistID(int newPlistID);
    void setTable();

    void SwitchSalesMan(int empid);

    void DeleteSalesOrder();

    void passwordEntered();

    void calcTotals();

    void showShareOptions();
    void on_excelButton_clicked();
    void switchOption();
    void disableEdit();
    void disableDelete();
private slots:

    void printReport(QPrinter *printer);

    void on_backButton_clicked();

    void on_copyButton_clicked();

    void on_editButton_clicked();

    void on_cancelButton_clicked();

    void on_OKButton_clicked();

    void setBillPreview(GeneralVoucherDataObject *voucher);

    void on_switchButton_clicked();

    void on_printButton_clicked();

    void printVoucher(GeneralVoucherDataObject *voucher);

    void on_voucherNoLineEdit_returnPressed();

    void on_filterComboBox_currentIndexChanged(int index);

    void on_printBill_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_modePay_clicked();

    void on_serviceCombo_currentIndexChanged(int index);

    void on_itemwiseButton_clicked();

    void on_routeComboBox_currentIndexChanged(int index);

    void on_switchTableButton_clicked();

signals:
    void closeWindow();

    void selectedVoucher(GeneralVoucherDataObject *obj);

    void directCheckOut();

    void VoucherModeSelected(GeneralVoucherDataObject *obj);

private:
    Ui::RestaurantSalesOrdersReport *ui;

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // RESTAURANTSALESORDERSREPORT_H
