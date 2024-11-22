#ifndef GROUPWISEREPORT_H
#define GROUPWISEREPORT_H

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
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
#include <QPushButton>

#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"

namespace Ui {
class GroupwiseReport;
}

class GroupwiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit GroupwiseReport(QWidget *parent = 0);
    ~GroupwiseReport();

    GMItemSortFilterProxyModel* sort;
    salesVoucherDatabaseHelper* salesHelper;
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
    QPointer<GMItemSearchLite> groupSearch;
    GMGodownLineMaterial *godownSearch;

    QString godownID = "%";


    bool datewiseFlag=false;

    void setTableHeading();
    void setTable();

    QString groupID = "0";

    ReportsTableView *tableView;

    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;


public slots:
private slots:
    void getGodownDetails(GodownDataObject obj);
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();



    void getItemDetails(QString item);

    void resetID();
    void on_datewiseButton_clicked();



    void setGroupedReport();
    void setItemwiseReport();
    void backButtonPressed();
    void on_ModCheckBox_stateChanged(int arg1);

    void on_ModCheckBox_clicked();

    void initWidgets();

    void calculateTotals();
    void showShareOptions();

    void printReport(QPrinter *printer);

    void on_tableView_doubleClicked(const QModelIndex &index);
private:
    Ui::GroupwiseReport *ui;
};

#endif // GROUPWISEREPORT_H
