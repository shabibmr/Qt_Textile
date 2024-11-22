#ifndef POITEMWISEREPORT_H
#define POITEMWISEREPORT_H

#include <QWidget>
#include <database/inventory/purchaseorder/purchaseorderdatabasehelper.h>
#include <customwidgets/Delegates/reportcommondelegate.h>
#include <customwidgets/test/gmitemsortfilterproxymodel.h>
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"
#include "counter/PrinterHelper/counterprinterhelper.h"


namespace Ui {
class POItemwiseReport;
}

class POItemwiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit POItemwiseReport(QWidget *parent = nullptr);
    ~POItemwiseReport();

    QSqlQueryModel *model;
    PurchaseOrderDatabaseHelper *inHelper;
    GMItemSortFilterProxyModel* sort;

    void setTableView();
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void on_okButton_clicked();

    void on_printButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);


    void on_searchLine_textChanged(const QString &arg1);

private:
    Ui::POItemwiseReport *ui;
};
#endif // POITEMWISEREPORT_H
