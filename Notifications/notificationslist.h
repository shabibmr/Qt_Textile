#ifndef NOTIFICATIONSLIST_H
#define NOTIFICATIONSLIST_H

#include <QWidget>
#include <QSqlRecord>
#include "database/Notifications/receivechanneldatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "finance/transactions/sales/salesvouchereditor.h"
#include "finance/transactions/purchases/purchasevouchereditor.h"
#include "finance/transactions/receipts/receiptvouchereditor.h"
#include "finance/transactions/payments/paymentvouchereditor.h"
#include "finance/transactions/creditnote/creditnotevouchereditor.h"
#include "finance/transactions/debitnote/debitnotevouchereditor.h"
#include "inventory/transactions/deliverynote/deliverynoteeditor.h"
#include "inventory/transactions/receiptnotes/receiptnotevouchereditor.h"
#include "inventory/transactions/purchaseorders/purchaseordervouchereditor.h"
#include "inventory/transactions/salesorders/salesordervouchereditor.h"

#include "inventory/masters/sales_inventory_items/additems.h"
#include "inventory/masters/sales_inventory_groups/additemgroups.h"
#include "inventory/masters/godowns/addgodown.h"
#include "inventory/masters/uom/adduom.h"
#include "inventory/masters/uom/addconversion.h"
#include "datamodels/hr/userprofiledatamodel.h"
#include "inventory/masters/brands/addbrands.h"

#include "finance/masters/accountgroups/addaccountgroup.h"
#include "finance/masters/ledgers/addledger.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "database/finance/debitnote/debitnotevoucherdatabasehelper.h"
#include "database/finance/creditnote/creditnotevoucherdatabasehelper.h"
#include "database/finance/paymentvoucher/paymentvoucherdatabasehelper.h"
#include "database/finance/receiptvoucher/receiptvoucherdatabasehelper.h"
#include "database/inventory/deliverynote/deliverynotedatabasehelper.h"
#include "database/inventory/receiptnote/receiptnotedatabasehelper.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/inventory/purchaseorder/purchaseorderdatabasehelper.h"

#include "customwidgets/TableViews/stockentrytableview.h"
#include "customwidgets/Models/gmnotificationlistmodel.h"
#include "customwidgets/TableViews/reportstableview.h"

#include "voucherOperations/vouchereditorselector.h"

#include "customwidgets/Material/components/qtmaterialflatbutton.h"
#include "notificationshistory.h"

class NotificationsList : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationsList(QWidget *parent = nullptr);
    ~NotificationsList();

    receiveChannelDatabaseHelper *rHelper ;
    LedgerMasterDatabaseHelper *ledHelper;
    NotificationDataModel *notificationObj;
    GeneralVoucherDataObject *voucher;
    inventoryItemDataModel *invItem;
    CompoundItemDataObject compItem;
    InventoryItemGroupDataModel *invGroup;
    LedgerMasterDataModel *ledger;
    AccountGroupDataModel *accGroup;
    GodownDataObject *godown;
    UomDataModel *uom;
    UomConversionDataModel *uomConv;
    UserProfileDataModel *employee;
    BrandsDataModel *brand;

//    GeneralVoucherDatabaseHelper *vHelper;

    QSqlQueryModel* model ;
    GeneralVoucherDataObject* missingItems;
    QPair<QString, QString> pair;
    ReportsTableView *tableView;
    VoucherEditorSelector *vEditor;

    QtMaterialFlatButton *historyButton;

    void ShowMissingDialog();





private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void setTableView();
    void NotificationAccepted();
    void NotificationRejected(QString reason);
    void markRead(int row);

    void ShowSalesOrderVoucher();
    void ShowPurchaseOrderVoucher();
    void ShowSalesInvoiceVoucher();
    void ShowPurchaseInvoiceVoucher();
    void ShowDebitNoteVoucher();
    void ShowCreditNoteVoucher();
    void ShowDeliveryNoteVoucher();
    void ShowReceiptNoteVoucher();
    void ShowReceiptVoucher();
    void ShowPaymentVoucher();
    void resetModel();




private:


signals:
    void notificationCount(int count);


};

#endif // NOTIFICATIONSLIST_H
