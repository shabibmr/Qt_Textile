#ifndef KOTREPORT_H
#define KOTREPORT_H

#include <QWidget>
#include "customwidgets/appbarwidget.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "database/inventory/workorder/workorderdatabasehelper.h"
#include "customwidgets/Delegates/WorkOrder/kotreportdelegate.h"

class KOTReport : public QWidget
{
public:
    KOTReport(QWidget *parent = nullptr);

    AppBarWidget *appBar;
    ReportsTableView *tableView;
    QSqlQueryModel *model;

    WorkOrderDatabaseHelper *dbHelper;

private:
    void setWidgets();
    void setTableView();


};

#endif // KOTREPORT_H
