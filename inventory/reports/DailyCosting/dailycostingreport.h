#ifndef DAILYCOSTINGREPORT_H
#define DAILYCOSTINGREPORT_H

#include <QWidget>
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"


namespace Ui {
class DailyCostingReport;
}

class DailyCostingReport : public QWidget
{
    Q_OBJECT
    InventoryHelper* inHelper;
    QSqlQueryModel* model;

public:
    explicit DailyCostingReport(QWidget *parent = 0);
    ~DailyCostingReport();

    void setTable();
private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::DailyCostingReport *ui;
};

#endif // DAILYCOSTINGREPORT_H
