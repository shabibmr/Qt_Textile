#ifndef MODEOFSERVICEWISEREPORT_H
#define MODEOFSERVICEWISEREPORT_H

#include <QWidget>
#include "database/Reports/ledgerhelper.h"
#include <datamodels/Counter/countersettingsdatamodel.h>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "datamodels/Counter/servicemode.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "customwidgets/Charts/SinglesSet/singlesetbarcharthelper.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

namespace Ui {
class ModeOfServiceWiseReport;
}

class ModeOfServiceWiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit ModeOfServiceWiseReport(QWidget *parent = 0);
    ~ModeOfServiceWiseReport();

    QDateTime dfrom;

    QSqlQueryModel *model;
    LedgerHelper dbHelper;

    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void setView();
    void on_backButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);

private:
    Ui::ModeOfServiceWiseReport *ui;
};

#endif // MODEOFSERVICEWISEREPORT_H
