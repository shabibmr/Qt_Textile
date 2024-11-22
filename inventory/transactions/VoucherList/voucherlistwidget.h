#ifndef VOUCHERLISTWIDGET_H
#define VOUCHERLISTWIDGET_H

#include <QWidget>

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
#include <QLabel>
#include "customwidgets/TableViews/reportstableview.h"

namespace Ui {
class VoucherListWidget;
}

class VoucherListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VoucherListWidget(QWidget *parent = nullptr);
    ~VoucherListWidget();

private:
    Ui::VoucherListWidget *ui;

private slots:
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

    QString reportType="Voucher List";

    InventoryHelper *dbHelper;
    QSqlQueryModel* model;
    QPointer<GMItemSearchLite> itemLineLite;

    bool datewiseFlag=false;

    void setTableHeading();
    void setTable();

    QString groupID;

    ReportsTableView *tableView;

};

#endif // VOUCHERLISTWIDGET_H
