#ifndef COMPLIMENTARYSALESREPORT_H
#define COMPLIMENTARYSALESREPORT_H

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/Reports/ledgerhelper.h"
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "finance/transactions/sales/salesvouchereditor.h"
#include <QSqlRecord>
#include <QWidget>

namespace Ui {
class ComplimentarySalesReport;
}

class ComplimentarySalesReport : public QWidget
{
    Q_OBJECT

public:
    explicit ComplimentarySalesReport(QWidget *parent = 0);
    ~ComplimentarySalesReport();

    salesVoucherDatabaseHelper* salesHelper;
    QList<GeneralVoucherDataObject> voucherList;

    LedgerHelper* ledHelper;

    QSqlQueryModel* model;
    CounterPrinterHelper *printerHelper;
    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

    void setItemTableHeading();
    void setItemTable();
    void setValueTableHeading();
    void setValueTable();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void on_amountWiseTableView_doubleClicked(const QModelIndex &index);

    void on_itemWiseTableView_doubleClicked(const QModelIndex &index);
    void showShareOptions();

    void printReport(QPrinter *printer);

private:
    Ui::ComplimentarySalesReport *ui;
};

#endif // COMPLIMENTARYSALESREPORT_H
