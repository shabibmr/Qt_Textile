#ifndef RESTAURANTDELIVERYREPORT_H
#define RESTAURANTDELIVERYREPORT_H

#include <QWidget>
#include "datamodels/transaction/generalvoucherdataobject.h"

#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"

#include "finance/transactions/calculator/transactioncalculator.h"

#include "database/hr/userprofiledatabasehelper.h"
#include "counter/MainScreen/assignsalesman.h"
#include <qmath.h>

#include "datamodels/projects/Quotation/quotationstatus.h"
#include "datamodels/Counter/servicemode.h"

#include "counter/PrinterHelper/counterprinterhelper.h"
#include <QCheckBox>
#include <QMessageBox>

namespace Ui {
class RestaurantDeliveryReport;
}

class RestaurantDeliveryReport : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantDeliveryReport(QWidget *parent = 0);
    ~RestaurantDeliveryReport();

    //    RestaurantDeliveryReport* DeliveryReportWidget;
    SalesOrderDatabaseHelper* salesOrderHelper;
    salesVoucherDatabaseHelper salesVoucherHelper;
    GeneralVoucherDataObject *voucher;
    QList<GeneralVoucherDataObject> voucherList;
    QSqlQueryModel *qModel;
    GMSQLQUeryModel *model;
    //    ReportsDatabaseHelper *dbhelper;
    LedgerMasterDatabaseHelper ledHelper;
    UserProfileDatabaseHelper userHelper;
    QMap<int, QString> employees;
    QStringList voucherNos;
    QString checkStyle;

    QList<int> selectedRows;

public slots:
    void showDateWidget(bool v);

    void setTable();



    void SwitchSalesMan(int empid);

    void calcTotals(GeneralVoucherDataObject *obj);

private slots:


    void on_backButton_clicked();

    void on_copyButton_clicked();

    void on_editButton_clicked();

    void on_cancelButton_clicked();

    void on_OKButton_clicked();

    void setBillPreview(GeneralVoucherDataObject *voucher);

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_switchButton_clicked();

    void on_settleButton_clicked();

    void on_printButton_clicked();

    void printVoucher(GeneralVoucherDataObject *voucher);

    void on_voucherNoLineEdit_returnPressed();

    void on_filterComboBox_currentIndexChanged(int index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_printBill_clicked();

    void checked(int row, bool checkState);

    void on_selectALL_clicked();

signals:
    void closeWindow();

    void selectedVoucher(GeneralVoucherDataObject *obj);

    void directCheckOut();

private:
    Ui::RestaurantDeliveryReport *ui;

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // RESTAURANTDELIVERYREPORT_H
