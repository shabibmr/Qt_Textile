#ifndef WAITERWISESALESREPORT_H
#define WAITERWISESALESREPORT_H

#include <QWidget>
#include "database/Reports/ledgerhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "sharetools/excel/excelreporthelper.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"


namespace Ui {
class WaiterWiseSalesReport;
}

class WaiterWiseSalesReport : public QWidget
{
    Q_OBJECT

public:
    explicit WaiterWiseSalesReport(QWidget *parent = 0);
    ~WaiterWiseSalesReport();

    QSqlQueryModel *model;
    QSqlQueryModel *deliveryModel;
    LedgerHelper* ledHelper;
    QDateTime dfrom;

    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void setTable();
    void showShareOptions();

    void printReport(QPrinter *printer);

private:
    Ui::WaiterWiseSalesReport *ui;
};

#endif // WAITERWISESALESREPORT_H
