#ifndef RESTAURANTCLOSINGSTOCK_H
#define RESTAURANTCLOSINGSTOCK_H

#include <QWidget>
#include "database/Reports/inventoryhelper.h"
#include "database/Reports/closingstockhelper.h"
#include "database/inventory/godown/godowndatabasehelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/closingstockretdelegate.h"

#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownsearchlite.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include <QPushButton>
#include "sharetools/sharetools.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Delegates/closingstockretdelegate.h"

namespace Ui {
class RestaurantClosingStock;
}

class RestaurantClosingStock : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantClosingStock(QWidget *parent = 0);
    ~RestaurantClosingStock();
    ClosingStockHelper* dbHelper;
    InventoryHelper *invHelper;
    SalesInventoryGroupsDatabaseHelper* invGrpHelper;
    QSqlQueryModel *grpModel;
    QSqlQueryModel *model;

    QtMaterialIconButton *backButton;
    QtMaterialIconButton *itemWise;
    QtMaterialIconButton *grouped;
    QtMaterialIconButton *printButton;
    QtMaterialIconButton *shareButton;

    ReportsTableView *tableView;


    QtMaterialAppBar *appBar;

    QStringList selectedGroups;

    QString reportType="Items";

    GMItemSortFilterProxyModel *sort;
    QPointer<GMItemSearchLite> itemLineLite;


    GodownDatabaseHelper* godownHelper;
    QList<GodownDataObject> godowns;

    QString godownId = "%";
    bool showVariance = false;

    QString groupID;

    bool show0stock = false;
    ShareTools *shareOptionsWidget;

    void exportToExcel();
private slots:
    void on_OKButton_clicked();

    void setTable();

    void searchList();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_searchLine_textChanged(const QString &arg1);

    void getItemDetails(QString item);
    void resetID();
    void on_tableView_doubleClicked(const QModelIndex &index);

    void setGroupedReport();
    void setItemwiseReport();
    void backButtonPressed();

    void getGodownDetails(GodownDataObject obj);
    void on_show0RadioButton_clicked(bool checked);

    void on_show0CheckBox_clicked(bool checked);

    void on_show0CheckBox_stateChanged(int arg1);

    void printReport(QPrinter *printer);
    void showShareOptions();

    void askForPrint();
private:
    Ui::RestaurantClosingStock *ui;
};

#endif // RESTAURANTCLOSINGSTOCK_H
