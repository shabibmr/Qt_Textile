#ifndef SALESMANWISEREPORT_H
#define SALESMANWISEREPORT_H

#include <QWidget>

#include "datamodels/Counter/countersettingsdatamodel.h"
#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/Reports/ledgerhelper.h"

namespace Ui {
class SalesmanwiseReport;
}

class SalesmanwiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit SalesmanwiseReport(QWidget *parent = 0);
    ~SalesmanwiseReport();

    QList<GeneralVoucherDataObject> voucherList;
    LedgerHelper *dbhelper;
    QSqlQueryModel* model;

private:
    Ui::SalesmanwiseReport *ui;

private slots:
    void setTable();

    void on_OKButton_clicked();
    void on_excelButton_clicked();
    void on_printButton_clicked();
    void on_backButton_clicked();
};

#endif // SALESMANWISEREPORT_H
