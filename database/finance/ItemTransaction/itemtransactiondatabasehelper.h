#ifndef ITEMTRANSACTIONDATABASEHELPER_H
#define ITEMTRANSACTIONDATABASEHELPER_H

#include <QtMath>

#include "database/databasehelper/databasehelper.h"
#include "database/databasehelper/bulkloaddatabasehelper.h"
#include "datamodels/transaction/itemtransactiondatamodel.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/Counter/billofmaterialsingredientsdatabasehelper.h"
#include "datamodels/transaction/itemfifodatamodel.h"
#include "database/finance/ItemTransaction/transactionfifodatabasehelper.h"

class ItemTransactionDatabaseHelper
{
public:
    ItemTransactionDatabaseHelper(DatabaseHelper* db = new DatabaseHelper);

    DatabaseHelper* db;
    TransactionFifoDatabaseHelper *fifoHelper;

    bool flag = true;
    bool useFifo = true;

    QString Transaction_Item_Helper_TableName = "Transaction_Item_Helper";
    QString Transaction_Bom_Helper_TableName = "Transaction_Bom_Helper";
    QString Transaction_Item_Helper_id = "_id";
    QString Transaction_Item_Helper_ItemID = "ItemID";
    QString Transaction_Item_Helper_From_Ledger = "From_Ledger";
    QString Transaction_Item_Helper_To_Ledger = "To_Ledger";
    QString Transaction_Item_Helper_VoucherID = "VoucherID";
    QString Transaction_Item_Helper_VoucherPrefix = "VoucherPrefix";
    QString Transaction_Item_Helper_VoucherDate = "VoucherDate";
    QString Transaction_Item_Helper_VoucherType = "VoucherType";
    QString Transaction_Item_Helper_From_Location = "From_Location";
    QString Transaction_Item_Helper_To_Location = "To_Location";
    QString Transaction_Item_Helper_Cr_Qty = "Cr_Qty";
    QString Transaction_Item_Helper_Dr_Qty = "Dr_Qty";
    QString Transaction_Item_Helper_Cr_Amount = "Cr_Amount";
    QString Transaction_Item_Helper_Dr_Amount = "Dr_Amount";
    QString Transaction_Item_Helper_Narration = "Narration";
    QString Transaction_Item_Helper_Time = "Time";
    QString Transaction_Item_Helper_ItemReqId = "Requirement_ItemID";
    QString Transaction_Item_Helper_Price = "Price";
    QString Transaction_Item_Helper_Consumed = "Consumed";
    QString Transaction_Item_Helper_ConsumedReqId = "Consumed_ReqId";


    QString Sales_Inventory_Table_Name = "Sales_Inventory_Items";
    QString Sales_Inventory_Id ="_Id";
    QString Sales_Inventory_ItemId ="Item_Id";
    QString Sales_Inventory_isCustomItem = "isCustomItem";
    QString Sales_Inventory_isPurchaseItem = "isPurchaseItem";
    QString Sales_Inventory_isSalesItem = "isSalesItem";



    int startTransaction();

    int insertItemTransaction(ItemTransactionDataModel *obj, QList<GodownTransactionDataModel> godownList);

    int updateItemTransaction(ItemTransactionDataModel *obj,GeneralVoucherDataObject *old);

    int deleteItemTransaction(GeneralVoucherDataObject *old);

    int commit();

    bool isSalesItemByItemId(QString itemID);

    GeneralVoucherDataObject *getVoucherToExport(int sourceVoucherType, int destinationVoucherType, QString ItemReqId );

    QList<ItemFifoDataModel *> getItemPriceForQtyByFifoWithUpdate(QString itemID, float quantity, QDate date);
    float getItemPriceToDisplayByFifo(QString itemID);
    int updateConsumedQty(QList<ItemFifoDataModel *> fifoList);
    void updateConsumedQtyForDelete(GeneralVoucherDataObject *voucher);
    float getConsumedQtyByVoucher(QString itemID, QString voucherNo, QString voucherType, QString voucherPrefix, QString ItemReqUuid);
    bool isPurchaseItemByItemId(QString itemID);

    bool hasRecordsForVoucherTypeByItemReqId(QString itemReqId, QString voucherType);

    QSqlQueryModel *getItemQtyListFifo(QString itemID, QDate date, QString godownID="%");

    QList<ItemFifoDataModel *> getFifoListByVoucherItem(QString voucherNo, QString voucherType, QString voucherPrefix, QString itemID);
    int insertItemTransactionRecalc(ItemTransactionDataModel *obj, QList<GodownTransactionDataModel> godownList, BulkLoadDatabaseHelper *db);
    int updateConsumedQtyForVoucher(GeneralVoucherDataObject *voucher);

    QList<GodownTransactionDataModel> getGodownAllocationByVoucherAndItem(QString voucherNo, QString voucherType, QString voucherPrefix,QString itemReqId );
};

#endif // ITEMTRANSACTIONDATABASEHELPER_H
