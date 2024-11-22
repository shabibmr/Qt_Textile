#ifndef WaiterwiseServicewiseReport_H
#define WaiterwiseServicewiseReport_H

#include <QWidget>
#include "database/Reports/ledgerhelper.h"
#include <datamodels/Counter/countersettingsdatamodel.h>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "datamodels/Counter/servicemode.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "customwidgets/Charts/SinglesSet/singlesetbarcharthelper.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/appbarwidget.h"
#include "customwidgets/filterwidget.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

class WaiterwiseServicewiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit WaiterwiseServicewiseReport(QWidget *parent = 0);
    ~WaiterwiseServicewiseReport();

    QDateTime dfrom;

    FilterWidget *filterWid;
    ReportsTableView *tableView;

    QSqlQueryModel *model;
    LedgerHelper dbHelper;

    CounterPrinterHelper *printerHelper;

//    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;
    AppBarWidget *appBar;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void setView();
    void on_backButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);

};

#endif // WaiterwiseServicewiseReport_H
