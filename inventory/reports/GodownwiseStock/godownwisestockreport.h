#ifndef GODOWNWISESTOCKREPORT_H
#define GODOWNWISESTOCKREPORT_H

#include <QWidget>
#include "database/inventory/godown/godowndatabasehelper.h"
#include "database/Reports/closingstockhelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownsearchlite.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

namespace Ui {
class GodownwiseStockReport;
}

class GodownwiseStockReport : public QWidget
{
    Q_OBJECT

public:
    explicit GodownwiseStockReport(QWidget *parent = nullptr);
    ~GodownwiseStockReport();

    QSqlQueryModel *model;
    QDate dateTo;
    GMItemSortFilterProxyModel* sort;
    ClosingStockHelper* ReportsHelper;
    GodownDatabaseHelper* godownHelper;
    QList<GodownDataObject> godowns;

    QString godownId;

    void setTable();
    CounterPrinterHelper *printerHelper;
    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void getGodownDetails(GodownDataObject obj);

    void showShareOptions();

    void printReport(QPrinter *printer);
private:
    Ui::GodownwiseStockReport *ui;
};

#endif // GODOWNWISESTOCKREPORT_H
