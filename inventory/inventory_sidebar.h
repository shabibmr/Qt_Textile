#ifndef INVENTORY_SIDEBAR_H
#define INVENTORY_SIDEBAR_H

#include <QWidget>
#include "login/loginvalues.h"
#include <customwidgets/section.h>
#include "customwidgets/defaultdashboard.h"
#include "inventory/masters/sales_inventory_items/inventoryitem.h"
#include "inventory/masters/sales_inventory_groups/inventorygroup.h"
#include "inventory/masters/godowns/godown.h"
#include "inventory/masters/properties/propertylist.h"
#include "inventory/masters/uom/uom.h"
#include "inventory/pricelist/pricelist.h"
#include "inventory/masters/brands/brandslist.h"
#include "projects/WorkOrder/workorderlist.h"

#include "inventory/transactions/stockentry/stockentry.h"
#include "inventory/transactions/stockjournal/stockjournals.h"
#include "inventory/transactions/deliverynote/deliverynotes.h"
#include "inventory/transactions/receiptnotes/receiptnotes.h"
#include "inventory/transactions/materialinout/materialinout.h"
#include "inventory/transactions/StockRequest/stockrequestlist.h"
#include "inventory/transactions/Defective/defectivelist.h"
#include "inventory/reports/GodownwiseReport/godownwisereport.h"
#include "inventory/reports/GodownwiseStock/godownwisestockreport.h"
#include "inventory/reports/StockReport/stockreport.h"
#include "inventory/reports/ItemAnalysis/itemanalysis.h"
#include "inventory/reports/MovementAnalysis/movementanalysis.h"
#include "inventory/reports/PhysicalStockRegister/physicalstockregister.h"
#include "inventory/reports/GodownwiseReport/godownwisereport.h"
#include "inventory/reports/NegativeStock/negativestock.h"
#include "inventory/reports/BelowReorderLevel/belowreorderlevel.h"
#include "inventory/reports/SalesBillsPending/salesbillspending.h"
#include "inventory/reports/PurchaseBillsPending/purchasebillspending.h"
#include "inventory/masters/sales_inventory_items/salesinventoryopening.h"
#include "inventory/reports/GroupwiseReport/groupwisereport.h"
#include "inventory/reports/PurchaseReport/purchasereport.h"
#include "inventory/reports/WorkOrderReport/workorderreport.h"
#include "inventory/reports/ItemwiseReport/itemwisereport.h"
#include "inventory/reports/ItemTransactionSummary/itemtransactionsummaryreportgenerator.h"
#include "inventory/reports/ProductionRate/productionratereport.h"
#include "counter/Reports/modeofservicewisereport.h"
#include "counter/Reports/timebasedsales.h"
#include "inventory/reports/DebitNoteReport/debitnotereport.h"
#include "inventory/reports/CreditNoteReport/creditnotereport.h"
#include "inventory/reports/WastageReport/wastagereport.h"
#include "inventory/transactions/ItemwiseSalesOrder/iteemwisesalesorderlist.h"
#include "inventory/transactions/ItemwiseSalesOrder/itemwisesodatewisereport.h"
#include "inventory/reports/MovementAnalysis/ledgerwiseitemmovement.h"
#include "inventory/reports/MovementAnalysis/groupwiseitemmovement.h"
#include "inventory/reports/MovementAnalysis/godownwiseitemmovement.h"
#include "ecommerce/itemselectorwidget.h"
#include "inventory/reports/GodownwiseReport/godownwisereport.h"
#include "inventory/reports/DailyCosting/dailycostingreport.h"

#include "counter/Reports/Restaurant/restaurantclosingstock.h"

#include "customwidgets/Misc/sidebarbutton.h"

//#include ""

#include <QDebug>

namespace Ui {
class Inventory_SideBar;
}

class Inventory_SideBar : public QWidget
{
    Q_OBJECT

public:

    QString fontColor = "feffef";

    inventoryItem* invItemWidget;
    inventorygroup* invGroupWidget;
    ItemSelectorWidget *itemSel;
    godown* godownwidget;
    UOM* uomWidget;
    PropertyList* propertyListWidget;
    priceList* priceListWidget;
    BrandsList* BrandsListWidget;

    StockEntry* StockEntryWidget;
    stockJournals* stockJournalsWidget;
    deliveryNotes* deliveryNotesWidget;
    WorkOrderList* WordOrderListWidget;
    DefectiveList* DefectiveWidget;

    ReceiptNotes* ReceiptNotesWidget;
    materialInOut* materialInWidget;
    materialInOut* materialOutWidget;
    StockRequestList* StockRequestWidget;

    RestaurantClosingStock* StockReportWidget;
    ItemAnalysis* ItemAnalysisWidget;
    GroupwiseReport* GroupwiseItemsWidget;
    PurchaseReport* PurchaseReportWidget;
    WorkOrderReport *WorkOrderReportWidget;
    DebitNoteReport* DebitNoteReportWidget;
    CreditNoteReport* CreditNoteReportWidget;
    WastageReport* WastageReportWidget;
    MovementAnalysis* MovementAnalysisWidget;
    StockEntry* PhysicalStockRegisterWidget;
    GodownwiseReport* GodownwiseReportWidget;
    NegativeStock* NegativeStockWidget;
    ReOrderLevel* BelowReorderLevelWidget;
    SalesBillsPending* SalesBillsPendingWidget;
    PurchaseBillsPending* PurchaseBillsPendingWidget;
    ItemwiseReport* itemwiseReport;
    LedgerwiseItemMovement* ledgerwiseItemsWidget;
    ItemwiseSODatewiseReport *itemRep1;

    ModeOfServiceWiseReport *serviceSalesReport;
    TimeBasedSales *timeSalesReport;

    GodownwiseStockReport *godownStockReport;

    ItemTransactionSummaryReportGenerator* summaryWidget;
    ProductionRateReport* productionRateWidget;


    SalesInventoryOpening* SalesInventoryOpeningWidget;
    IteemwiseSalesOrderList* itemwiseSOListWidget;



    explicit Inventory_SideBar(QWidget *parent = 0);
    ~Inventory_SideBar();

    DefaultDashBoard *dash;
    Section* sectionMaster;
    Section* sectionTransaction ;
    Section* sectionReports ;
    Section* sectionItems;

    SideBarButton *ItemMaster;
    SideBarButton *ItemGroup;
    SideBarButton *itemSelect;
    SideBarButton *StockEntryButton;
    SideBarButton *StockJournal;
    SideBarButton *MaterialIn;
    SideBarButton *MaterialOut;
    SideBarButton *DeliveryNote;
    SideBarButton *ReceiptNote;
    SideBarButton *Property;
    SideBarButton *PriceList;
    SideBarButton *UOMButton;
    SideBarButton *Godown;
    SideBarButton *brandsToolButt;
    SideBarButton *InventoryOpeningEntry;
    SideBarButton *StockRequestButton;
    SideBarButton *WastageButton;


    SideBarButton *TransSummary;
    SideBarButton *ProductionRate;

    SideBarButton *StockButton;
    SideBarButton *ItemWiseSales;
    SideBarButton *itemRepB1;

    SideBarButton *workOrderButton;
    SideBarButton *MovementAnalysisButton;
    SideBarButton *ItemAnalysisButton;
    SideBarButton *GroupwiseSales;
    SideBarButton *GroupwisePurchase;
    SideBarButton *DebitNoteReportButton;
    SideBarButton *CreditNoteReportButton;
    SideBarButton *WastageReportButton;
    SideBarButton *WOReportButton;
    SideBarButton *PhysicalStockRegistersButton;
    SideBarButton *GodownWiseReportButton;
    SideBarButton *SalesBillsPendingButton;
    SideBarButton *PurchaseBillsPendingButton;
    SideBarButton *NegativeStockButton;
    SideBarButton *ReorderLevelButton;
    SideBarButton *ItemeiseSOButton;
    SideBarButton *ItemeisePOButton;

    SideBarButton *TimeButton;
    SideBarButton *modeButton;
    SideBarButton *godownwiseStockButton;
    SideBarButton *godownwiseMovementButton;
    SideBarButton *ledgerwiseItemsButton;
    SideBarButton *dailyCostingButton;

    SideBarButton *POSButton;

    QFrame *getLine();

private:
    Ui::Inventory_SideBar *ui;

public slots:
    void searchText(QString text);
    void resetAllColors();
    void GodownwiseClicked();

    void ItemMasterClicked();
    void ItemGroupClicked();
    void ItemSelectClicked();
    void StockEntryClicked();
    void WorkOrderClicked();
    void StockRequestClicked();
    void StockJournalClicked();
    void MaterialInClicked();
    void MaterialOutClicked();
    void DeliveryNoteClicked();
    void ReceiptNoteClicked();
    void WastageClicked();
    void PropertyClicked();
    void PriceListClicked();
    void UOMClicked();
    void GodownClicked();
    void BrandsClicked();
    void StockClicked();
    void ProductionRateClicked();
    void MovementAnalysisClicked();
    void ItemAnalysisClicked();
    void ItemwiseSalesClicked();
    void itemRep1Clicked();
    void GroupwiseClicked();
    void PhysicalStockRegistersClicked();
    void GodownWiseReportClicked();
    void SalesBillsPendingClicked();
    void PurchaseBillsPendingClicked();
    void NegativeStockClicked();
    void ReorderLevelClicked();
    void itemwiseSOClicked();

    void itemwisePOClicked();

    void ModeOfServiceClicked();
    void timeReportClicked();

    void GodownStockClicked();
    void GodownMovementClicked();

    void transactionReportClicked();


    void OpeingClicked();

    void clearLayout();

    void setUAC();

    void PurchaseReportClicked();
    void WorkOrderReportClicked();
    void DebitNoteReportClicked();
    void CreditNoteReportClicked();
    void WastageReportClicked();
    void LedgerWiseItemsClicked();

    void POSClicked();

signals:
    void searchTextchanged(QString);

private slots:
    void DailyCostingClicked();
    void GodownWiseItemsClicked();
    void GroupWiseItemsClicked();
};


#endif // INVENTORY_SIDEBAR_H
