#ifndef VOUCHEREDITORSELECTOR_H
#define VOUCHEREDITORSELECTOR_H

#include <QString>
#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"

#include <finance/transactions/purchases/purchasevouchereditor.h>
#include <finance/transactions/sales/salesvouchereditor.h>
#include <finance/transactions/payments/paymentvouchereditor.h>
#include <finance/transactions/receipts/receiptvouchereditor.h>
#include <finance/transactions/journals/journalvouchereditor.h>
#include <finance/transactions/creditnote/creditnotevouchereditor.h>
#include <finance/transactions/debitnote/debitnotevouchereditor.h>
#include <finance/transactions/Proforma/proformainvoiceeditor.h>
#include <finance/transactions/contra/contravouchereditor.h>

#include "counter/TableManagement/tablereservationeditor.h"

#include <inventory/transactions/Defective/wastageentryeditor.h>
#include <inventory/transactions/StockRequest/stockrequesteditor.h>
#include <inventory/transactions/deliverynote/deliverynoteeditor.h>
#include <inventory/transactions/materialinout/materialinouteditor.h>
#include <inventory/transactions/purchaseorders/purchaseordervouchereditor.h>
#include <inventory/transactions/salesorders/salesordervouchereditor.h>
#include <inventory/transactions/receiptnotes/receiptnotevouchereditor.h>
#include <inventory/transactions/stockentry/stockentryeditor.h>
#include <inventory/transactions/stockjournal/stockjournaleditor.h>
#include <warehouse/godowntransfer/godowntransfereditor.h>
#include "inventory/transactions/ItemwiseSalesOrder/additemwisesalesorder.h"
#include "projects/WorkOrder/workordereditor.h"

class VoucherEditorSelector : public QWidget
{
    Q_OBJECT

public:
    VoucherEditorSelector(UiAccessControlDataModel *permissions = nullptr, QWidget *parent=nullptr);

   void openEditor(QString voucherNO,QString voucherPref,QString voucherType);

   salesVoucherDatabaseHelper *salesHelper;
   PurchaseVoucherDatabaseHelper *purchaseHelper ;
   PaymentVoucherDatabaseHelper *paymentHelper;
   ReceiptVoucherDatabaseHelper *receiptHelper;
   JournalVoucherDatabaseHelper *journalHelper;
   CreditNoteVoucherDatabaseHelper *creditHelper ;
   DebitNoteVoucherDatabaseHelper *debitHelper;
   ProformaDatabaseHelper *proformaHelper;
   ContraVoucherDatabaseHelper *contraHelper;
   DeliveryNoteDatabaseHelper *deliveryHelper;
   ReceiptNoteDatabaseHelper *receiptNoteHelper;
   PurchaseOrderDatabaseHelper *POHelper;
   SalesOrderDatabaseHelper *SOHelper;
   WorkOrderDatabaseHelper *WOHelper;

   StockJournalDatabaseHelper *stockJournalHelper;
   StockEntryDatabaseHelper *stockEntryHelper;
   materialinoutDatabaseHelper *materialHelper;
   DefectiveDatabaseHelper *wastageHelper;
   StockRequestDatabaseHelper *stockRequestHelper;
   GodownTransferDatabaseHelper *godownTransferHelper;
   TableReservationDatabaseHelper *tableReservationHelper;


   VoucherEditorBase *voucherWidget;

   UiAccessControlDataModel *permissions;
   UserGroupDatabaseHelper *userGroupHelper;


   void createNewVoucher(QString voucherType);
   void setPermissions(UiAccessControlDataModel *value);
   void exportVoucher(GeneralVoucherDataObject *srcVoucher, QString tgtVoucherType);
   void openExternalVoucher(GeneralVoucherDataObject *voucher);

signals:
   void refresh();
   void saveCompleted();
   void NotificationAccepted();
   void NotificationRejected(QString reason);
private:

private slots:
   void refreshList();
   void emitSaveCompleted();
   void emitNotificationAccepted();
   void emitNotificationRejected(QString reason);


};

#endif // VOUCHEREDITORSELECTOR_H
