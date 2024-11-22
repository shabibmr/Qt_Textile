#ifndef DAYBOOK_H
#define DAYBOOK_H

#include <QWidget>

#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/test/ledgerreportdelegate.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"

#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

namespace Ui {
class Daybook;
}

class Daybook : public QWidget
{
    Q_OBJECT

public:
    explicit Daybook(QWidget *parent = 0);
    ~Daybook();
    QList<GeneralVoucherDataObject> voucherList;
    ReportsDatabaseHelper *dbhelper;
    LedgerHelper *ledHelper;
    QSqlQueryModel* model;

    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private:
    Ui::Daybook *ui;

private slots:
    void setTable();

    void on_OKButton_clicked();
    void on_excelButton_clicked();
    void on_printButton_clicked();
    void on_backButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);
};

#endif // DAYBOOK_H
