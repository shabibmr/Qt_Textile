#ifndef SALESORDERSREPORT_H
#define SALESORDERSREPORT_H

#include <QWidget>
#include <QMessageBox>
#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/hr/userprofiledatabasehelper.h"

#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/assignsalesman.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "finance/transactions/calculator/transactioncalculator.h"

namespace Ui {
class SalesOrdersReport;
}

class SalesOrdersReport : public QWidget
{
    Q_OBJECT

public:
    explicit SalesOrdersReport(QWidget *parent = 0);
    ~SalesOrdersReport();

    SalesOrdersReport* SalesOrdersReportWidget;
    SalesOrderDatabaseHelper* salesOrderHelper;
    GeneralVoucherDataObject *voucher;
    QList<GeneralVoucherDataObject> voucherList;
    ReportsDatabaseHelper *dbhelper;
    UserProfileDatabaseHelper *userHelper;
    QMap<int, QString> employees;
    QStringList voucherNos;

public slots:
    void showDateWidget(bool v);

    void setTable();

    void setTableHeading();

    void SwitchSalesMan(int empid);

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_backButton_clicked();

    void on_copyButton_clicked();

    void on_editButton_clicked();

    void on_cancelButton_clicked();

    void on_OKButton_clicked();

    void setBillPreview(GeneralVoucherDataObject *voucher);

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_switchButton_clicked();

    void on_printButton_clicked();

    void printVoucher(GeneralVoucherDataObject *voucher);

    void on_voucherNoLineEdit_returnPressed();

    void on_filterComboBox_currentIndexChanged(int index);

    void calcTotals();

signals:
    void closeWindow();

    void selectedVoucher(GeneralVoucherDataObject *obj);

private:
    Ui::SalesOrdersReport *ui;
};

#endif // SALESORDERSREPORT_H
