#ifndef DAILYREPORTCALCULATOR_H
#define DAILYREPORTCALCULATOR_H

#include "datamodels/Counter/shiftdatamodel.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
#include "database/finance/Closing/restaurantshiftdatabasehelper.h"
#include "database/Reports/ledgerhelper.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "database/finance/Closing/dailyclosingdatabasehelper.h"
#include <sharetools/email/emailthread.h>
#include "database/Reports/inventoryhelper.h"
#include <QSqlRecord>
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"


class DailyReportCalculator : public QObject
{
    Q_OBJECT

public:

    QString voucherPrefix;
    ShiftDataModel *shift;
    explicit DailyReportCalculator(QObject *parent = Q_NULLPTR);
    ~DailyReportCalculator();

    RestaurantShiftDatabaseHelper *dbHelper;

    QString html = "";
    QString closingStock="";

    GeneralVoucherDataObject PaymentDetails;

    salesVoucherDatabaseHelper *salesHelper;

    InventoryHelper *invHelper;

    QDateTime fromDate;

    QDateTime toDate;

    LedgerHelper *ledHelper;




    void setDate(QDateTime fromDate,QDateTime toDate);
    void CalculateValues();
    void sendMail();

    QString getShiftasHTML();
    void printReport();


    void setShift(ShiftDataModel *value);
};

#endif // DAILYREPORTCALCULATOR_H
