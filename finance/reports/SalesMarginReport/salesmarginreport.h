#ifndef SALESMARGINREPORT_H
#define SALESMARGINREPORT_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include <QFileDialog>
#include <sharetools/excel/excelreporthelper.h>
#include <counter/PrinterHelper/counterprinterhelper.h>
#include <customwidgets/Delegates/reportcommondelegate.h>
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

namespace Ui {
class SalesMarginReport;
}

class SalesMarginReport : public QWidget
{
    Q_OBJECT

public:
    explicit SalesMarginReport(QWidget *parent = nullptr);
    ~SalesMarginReport();
    QSqlQueryModel *model;
    InventoryHelper *dbHelper;

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
    Ui::SalesMarginReport *ui;
};

#endif // SALESMARGINREPORT_H
