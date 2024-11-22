#ifndef RESTAURANTPENDINGDELIVERY_H
#define RESTAURANTPENDINGDELIVERY_H

#include <QWidget>
#include "datamodels/transaction/generalvoucherdataobject.h"

#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "datamodels/Counter/servicemode.h"

#include "counter/MainScreen/assignsalesman.h"

#include "counter/PrinterHelper/counterprinterhelper.h"

namespace Ui {
class RestaurantPendingDelivery;
}

class RestaurantPendingDelivery : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantPendingDelivery(QWidget *parent = 0);
    ~RestaurantPendingDelivery();

    SalesOrderDatabaseHelper* salesOrderHelper;
    GeneralVoucherDataObject *voucher;
    QList<GeneralVoucherDataObject> voucherList;
    QMap<int, QString> employees;
    QStringList voucherNos;
    QString route;

    QSqlQueryModel *model;

public slots:
    void showDateWidget(bool v);

    void setTable();

//    void setTableHeading();

    void calcTotals();

private slots:

    void on_backButton_clicked();

    void on_copyButton_clicked();

    void on_editButton_clicked();

    void on_assignButton_clicked();

    void on_OKButton_clicked();

    void setBillPreview(GeneralVoucherDataObject *voucher);

    void SwitchSalesMan(int empid);

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_printButton_clicked();

    void printVoucher(GeneralVoucherDataObject *voucher);

    void on_voucherNoLineEdit_returnPressed();

    void on_routeComboBox_currentIndexChanged(const QString &arg1);

    void on_tableView_clicked(const QModelIndex &index);

signals:
    void closeWindow();

    void selectedVoucher(GeneralVoucherDataObject *obj);

private:
    Ui::RestaurantPendingDelivery *ui;

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // RESTAURANTPENDINGDELIVERY_H
