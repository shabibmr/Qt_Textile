#ifndef TIMEBASEDSALES_H
#define TIMEBASEDSALES_H

#include <QWidget>
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "database/Reports/ledgerhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Charts/SinglesSet/singlesetbarcharthelper.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"


namespace Ui {
class TimeBasedSales;
}

class TimeBasedSales : public QWidget
{
    Q_OBJECT


public:
    explicit TimeBasedSales(QWidget *parent = 0);
    ~TimeBasedSales();

    QSqlQueryModel *model;
    LedgerHelper *ledHelper;

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
    Ui::TimeBasedSales *ui;
};

#endif // TIMEBASEDSALES_H
