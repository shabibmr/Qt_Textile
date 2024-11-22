#ifndef ITEMWISESODATEWISEREPORT_H
#define ITEMWISESODATEWISEREPORT_H

#include <QWidget>

#include <QDateEdit>
#include <QVBoxLayout>
#include "database/inventory/salesorder/itemwisesalesorderdatabasehelper.h"
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Basic/gmdateedit.h"


#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "sharetools/print/printdotmatrix.h"

namespace Ui {
class ItemwiseSODatewiseReport;
}

class ItemwiseSODatewiseReport : public QWidget
{
    Q_OBJECT

public:

    explicit ItemwiseSODatewiseReport(QWidget *parent = nullptr);
    ~ItemwiseSODatewiseReport();

    QString backgroundColor = "#424242";

    ItemwiseSalesOrderDatabaseHelper *dbHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;

    inventoryItemDataModel item;
    ReportsTableView *tableView;

    QSqlQueryModel *model;
    QSqlQueryModel *itemPurchaseModel;

    QList<QSqlQueryModel*> itemwiseModels;
    QList<QSqlQueryModel*> itemwisePurchaseModels;



    QPointer<QVBoxLayout> TableLay;

    QDate salesDate;

    GMDateEdit *dateEdit;

    GMInventoryItemSearch *itemLine;

    QToolButton *resetButton;

    PrintDotMatrix printer;

    void initDBHelpers();
    void initGlobalObjects();
    void initWidgets();

    void setWidgets();
       void refreshMainTable();

    void printReportItemsSeperate();
    void printReportALLItems();
    void printReportItem(int index);

    void refreshData();



    void clearTableLayout();

private:
    Ui::ItemwiseSODatewiseReport *ui;
};

#endif // ITEMWISESODATEWISEREPORT_H
