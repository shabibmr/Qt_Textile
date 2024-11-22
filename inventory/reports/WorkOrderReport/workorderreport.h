#ifndef WORKORDERREPORT_H
#define WORKORDERREPORT_H

#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "customwidgets/section.h"
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include <QWidget>
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include "sharetools/sharetools.h"

#include <QPushButton>

namespace Ui {
class WorkOrderReport;
}

class WorkOrderReport : public QWidget
{
    Q_OBJECT

public:
    explicit WorkOrderReport(QWidget *parent = 0);
    ~WorkOrderReport();

    GMItemSortFilterProxyModel* sort;
    PurchaseVoucherDatabaseHelper* salesHelper;
    QList<GeneralVoucherDataObject> voucherList;
    SalesInventoryGroupsDatabaseHelper* invGrpHelper;
    QDateTime dfrom;

    QtMaterialIconButton *backButton;
    QtMaterialIconButton *itemWise;
    QtMaterialIconButton *grouped;
    QtMaterialAppBar *appBar;
    QStringList selectedGroups;

    QString reportType="Group";

    InventoryHelper *dbHelper;
    QSqlQueryModel* model;
    QPointer<GMItemSearchLite> itemLineLite;

    bool datewiseFlag=false;

    void setTableHeading();
    void setTable();

    QString groupID;
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void getItemDetails(QString item);

    void resetID();
    void on_datewiseButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);


    void setGroupedReport();
    void setItemwiseReport();
    void backButtonPressed();

    void showShareOptions();

    void printReport(QPrinter *printer);

private:
    Ui::WorkOrderReport *ui;
};

#endif // WORKORDERREPORT_H
