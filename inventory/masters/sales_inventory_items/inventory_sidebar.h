#ifndef INVENTORY_SIDEBAR_H
#define INVENTORY_SIDEBAR_H

#include <QWidget>
#include <customwidgets/section.h>
#include "customwidgets/defaultdashboard.h"
#include "inventory/masters/sales_inventory_items/inventoryitem.h"
#include "inventory/masters/sales_inventory_groups/inventorygroup.h"
#include "inventory/masters/godowns/godown.h"
#include "inventory/masters/properties/propertylist.h"
#include "inventory/masters/uom/uom.h"
#include "inventory/pricelist/pricelist.h"

#include "inventory/transactions/stockentry/stockentry.h"
#include "inventory/transactions/stockjournal/stockjournals.h"
#include "inventory/transactions/deliverynote/deliverynotes.h"
#include "inventory/transactions/receiptnotes/receiptnotes.h"
#include "inventory/transactions/materialinout/materialinout.h"


#include <QDebug>

namespace Ui {
class Inventory_SideBar;
}

class Inventory_SideBar : public QWidget
{
    Q_OBJECT

public:


    inventoryItem* invItemWidget;
    inventorygroup* invGroupWidget;
    godown* godownwidget;
    UOM* uomWidget;
    PropertyList* propertyListWidget;
    priceList* priceListWidget;

    StockEntry* StockEntryWidget;
    stockJournals* stockJournalsWidget;
    deliveryNotes* deliveryNotesWidget;

    ReceiptNotes* ReceiptNotesWidget;
    materialInOut* materialInWidget;
    materialInOut* materialOutWidget;


    explicit Inventory_SideBar(QWidget *parent = 0);
    ~Inventory_SideBar();

    DefaultDashBoard *dash;
    Section* sectionMaster;
    Section* sectionTransaction ;
    Section* sectionReports ;


    QToolButton *ItemMaster;
    QToolButton *ItemGroup;
    QToolButton *StockEntryButton;
    QToolButton *StockJournal;
    QToolButton *MaterialIn;
    QToolButton *MaterialOut;
    QToolButton *DeliveryNote;
    QToolButton *ReceiptNote;
    QToolButton *Property;
    QToolButton *PriceList;
    QToolButton *UOMButton;
    QToolButton *Godown;
    QToolButton *Stock;
    QToolButton *MovementAnalysis;
    QToolButton *ItemAnalysis;
    QToolButton *PhysicalStockRegisters;
    QToolButton *GodownWiseReport;
    QToolButton *SalesBillsPending;
    QToolButton *PurchaseBillsPending;
    QToolButton *negativeStock;
    QToolButton *reorderItems;



private:
    Ui::Inventory_SideBar *ui;
public slots:

    void resetAllColors();
    void ItemMasterClicked();
    void ItemGroupClicked();
    void StockEntryClicked();
    void StockJournalClicked();
    void MaterialInClicked();
    void MaterialOutClicked();
    void DeliveryNoteClicked();
    void ReceiptNoteClicked();
    void PropertyClicked();
    void PriceListClicked();
    void UOMClicked();
    void GodownClicked();
    void StockClicked();
    void MovementAnalysisClicked();
    void ItemAnalysisClicked();
    void PhysicalStockRegistersClicked();
    void GodownWiseReportClicked();
    void SalesBillsPendingClicked();
    void PurchaseBillsPendingClicked();
    void negativeStockClicked();
    void reorderItemsClicked();

    void clearLayout();

};

#endif // INVENTORY_SIDEBAR_H
