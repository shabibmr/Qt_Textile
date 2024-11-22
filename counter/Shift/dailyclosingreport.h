#ifndef DAILYCLOSINGREPORT_H
#define DAILYCLOSINGREPORT_H

#include <QWidget>
#include <QVBoxLayout>
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Delegates/dailyclosingdelegate.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "database/finance/Closing/restaurantshiftdatabasehelper.h"
#include "database/finance/Closing/dailyclosingdatabasehelper.h"
#include "customwidgets/filterwidget.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "database/Reports/inventoryhelper.h"
#include "database/Reports/ledgerhelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "database/Reports/closingstockhelper.h"
#include "sharetools/email/emailthread.h"
#include "dailyreportcalculator.h"


class DailyClosingReport : public QWidget
{
public:
    DailyClosingReport(QWidget *parent = nullptr);

    ReportsTableView *shiftTable;
    ReportsTableView *salesTable;
    ReportsTableView *purchaseTable;
    ReportsTableView *receiptsTable;
    ReportsTableView *paymentsTable;
    ReportsTableView *debitNoteTable;
    ReportsTableView *creditNoteTable;

    ShiftDataModel *shift;
    QSqlQueryModel *dailyClosingModel;

    FilterWidget *filterWid;
    bool showReport = false;

    RestaurantShiftDatabaseHelper *shiftHelper;
    DailyClosingDatabaseHelper *dbHelper;
    QtMaterialFloatingActionButton *saveButton;
    QtMaterialFloatingActionButton *printButton;
    QtMaterialFloatingActionButton *mailButton;

    void setWidgets();
    void setShiftTable();
    void setLedgerwiseView();
    void setShift(ShiftDataModel *value);
    void disableSave();
    void setData();

    void setShowReport(bool value);

public slots:
    void sendMail();
    void printReport();
};

#endif // DAILYCLOSINGREPORT_H
