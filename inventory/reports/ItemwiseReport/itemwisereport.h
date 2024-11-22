#ifndef ITEMWISEREPORT_H
#define ITEMWISEREPORT_H

#include "database/Reports/inventoryhelper.h"


#include <QWidget>
#include <QSqlRecord>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

namespace Ui {
class ItemwiseReport;
}

class ItemwiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit ItemwiseReport(QWidget *parent = 0);
    ~ItemwiseReport();

    salesVoucherDatabaseHelper *salesHelper;
    QMap<int, QString> employees;

    InventoryHelper* inHelper;
    QSqlQueryModel* model;
    QList<GeneralVoucherDataObject> voucherList;

    GMItemSortFilterProxyModel* sort;

    QSqlQueryModel* waiterModel;


    void setTable();

//    void setSalesmanTable();

    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;

    QtMaterialIconButton *showChartButton;

    ShareTools *shareOptionsWidget;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void on_salesmanCombo_currentIndexChanged(int index);

//    void on_excelWaiter_clicked();

    void on_ExtractButton_clicked();
    void showShareOptions();

    void showChart();

    void printReport(QPrinter *printer);

private:
    Ui::ItemwiseReport *ui;
};

#endif // ITEMWISEREPORT_H
