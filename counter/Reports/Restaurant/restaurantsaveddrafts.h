#ifndef RESTAURANTSAVEDDRAFTS_H
#define RESTAURANTSAVEDDRAFTS_H

#include <QWidget>

#include "counter/Reports/Restaurant/restaurantsalesordersreport.h"
#include "counter/Reports/Restaurant/restaurantdeliveryreport.h"
#include "counter/Reports/Restaurant/restaurantpendingdelivery.h"
//#include "counter/Reports/Restaurant/dineinreport.h"
//#include "counter/Reports/Restaurant/takeawayreport.h"

namespace Ui {
class RestaurantSavedDrafts;
}

class RestaurantSavedDrafts : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantSavedDrafts(salesVoucherDeletedDatabaseHelper  *deletedSalesHelper,
                                   SalesOrderDatabaseHelper *soHelper,
                                   AccountGroupMasterDatabaseHelper *accHelper,
                                   LedgerMasterDatabaseHelper *ledHelper,
                                   salesVoucherDatabaseHelper *salesHelper,
                                   QWidget *parent = Q_NULLPTR);
    ~RestaurantSavedDrafts();

    RestaurantSalesOrdersReport *SalesOrdersReportWidget;
    salesVoucherDeletedDatabaseHelper  *deletedSalesHelper;
    SalesOrderDatabaseHelper *soHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    salesVoucherDatabaseHelper *salesHelper;
    UiAccessControlDataModel *permissions;

    void setPermissions(UiAccessControlDataModel *value);

public slots:
    void setTabWidget();
    void closeWidget();

    void VoucherSelected(GeneralVoucherDataObject *obj);

    void checkoutDirect();
    void setTab(int tab=0,int plist=-1);


signals:

    void selectedVoucher(GeneralVoucherDataObject *obj);

    void directCheckOutSignal();

    void VoucherModeSelected(GeneralVoucherDataObject *obj);




private slots:
    void on_pushButton_clicked();

private:
    Ui::RestaurantSavedDrafts *ui;
};

#endif // RESTAURANTSAVEDDRAFTS_H
