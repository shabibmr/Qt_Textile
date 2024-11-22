#ifndef DISCOUNTSREPORT_H
#define DISCOUNTSREPORT_H

#include <QWidget>
#include <QSqlRecord>
#include "database/Reports/ledgerhelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "finance/transactions/sales/salesvouchereditor.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

namespace Ui {
class DiscountsReport;
}

class DiscountsReport : public QWidget
{
    Q_OBJECT

public:
    explicit DiscountsReport(QWidget *parent = 0);
    ~DiscountsReport();
    QSqlQueryModel *model;
    LedgerHelper *dbHelper;
    salesVoucherDatabaseHelper* salesHelper;

    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:

    void setTable();

    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void on_amountWiseTableView_doubleClicked(const QModelIndex &index);

    void showShareOptions();

    void printReport(QPrinter *printer);

private:
    Ui::DiscountsReport *ui;
};

#endif // DISCOUNTSREPORT_H
