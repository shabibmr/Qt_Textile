#ifndef WAREHOUSE_SIDEBAR_H
#define WAREHOUSE_SIDEBAR_H

#include <QWidget>
#include "customwidgets/defaultdashboard.h"
#include "customwidgets/section.h"
#include <QDebug>
#include "inventory/transactions/stockentry/stockentry.h"
#include "inventory/transactions/deliverynote/deliverynotes.h"
#include "inventory/transactions/purchaseorders/purchaseorders.h"
#include "warehouse/godowntransfer/godowntransfer.h"
#include "inventory/reports/StockReport/stockreport.h"
#include "inventory/reports/PhysicalStockRegister/physicalstockregister.h"

namespace Ui {
class Warehouse_SideBar;
}

class Warehouse_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit Warehouse_SideBar(QWidget *parent = 0);
    ~Warehouse_SideBar();

    DefaultDashBoard *dash;
    Section* sectionReports ;

    QToolButton *PhysicalStockEntry;
    QToolButton *DeliveryNotes;
    QToolButton *PurchaseOrders;
    QToolButton *GodownTransfers;
    QToolButton *Stock;
    QToolButton *Registers;

    StockEntry *StockEntryWidget;
    deliveryNotes *deliveryNotesWidget;
    purchaseOrders *purchaseOrdersWidget;
    GodownTransfer *GodownTransferWidget;

    StockReport* StockReportWidget;
private:
    Ui::Warehouse_SideBar *ui;
public slots:
    void clearLayout();
    void resetAllColors();
    void PhysicalStockEntryClicked();
    void DeliveryNotesClicked();
    void PurchaseOrdersClicked();
    void GodownTransfersClicked();
    void StockClicked();
    void RegistersClicked();

};

#endif // WAREHOUSE_SIDEBAR_H
