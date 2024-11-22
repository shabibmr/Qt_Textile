#ifndef DATEWISESALESREPORT_H
#define DATEWISESALESREPORT_H

#include <QWidget>

#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/test/ledgerreportdelegate.h"
#include "sharetools/excel/excelreporthelper.h"

#include <QSqlRecord>

#include "counter/PrinterHelper/counterprinterhelper.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"


namespace Ui {
class DatewiseSalesReport;
}

class DatewiseSalesReport : public QWidget
{
    Q_OBJECT

public:
    explicit DatewiseSalesReport(QWidget *parent = 0);
    ~DatewiseSalesReport();

    LedgerHelper *ledHelper;
    QSqlQueryModel* model;
    QDateTime dfrom;
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void setTable();
    void showShareOptions();

    void printReport(QPrinter *printer);


private:
    Ui::DatewiseSalesReport *ui;
};

#endif // DATEWISESALESREPORT_H
