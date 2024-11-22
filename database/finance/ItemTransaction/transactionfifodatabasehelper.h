#ifndef TRANSACTIONFIFODATABASEHELPER_H
#define TRANSACTIONFIFODATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/itemtransactiondatamodel.h"
#include "datamodels/transaction/itemfifodatamodel.h"
#include "datamodels/transaction/generalvoucherdataobject.h"

class TransactionFifoDatabaseHelper
{
public:
    TransactionFifoDatabaseHelper(DatabaseHelper *db = new DatabaseHelper);

    QString Transaction_Fifo_Helper_TableName = "Transaction_Fifo_Mapping";
    QString Transaction_Fifo_Helper_id = "_id";
    QString Transaction_Fifo_Helper_TransactionID = "Transaction_ID";
    QString Transaction_Fifo_Helper_VoucherDate = "Voucher_Date";
    QString Transaction_Fifo_Helper_Godown = "Godown";
    QString Transaction_Fifo_Helper_ItemID = "Item_ID";
    QString Transaction_Fifo_Helper_VoucherID = "Voucher_No";
    QString Transaction_Fifo_Helper_VoucherPrefix = "Voucher_Prefix";
    QString Transaction_Fifo_Helper_VoucherType = "Voucher_Type";
    QString Transaction_Fifo_Helper_ItemReqId = "Requirement_ItemID";
    QString Transaction_Fifo_Helper_ConsumedVoucherID = "Consumed_Voucher_No";
    QString Transaction_Fifo_Helper_ConsumedVoucherPrefix = "Consumed_Voucher_Prefix";
    QString Transaction_Fifo_Helper_ConsumedVoucherType = "Consumed_Voucher_Type";
    QString Transaction_Fifo_Helper_ConsumedReqId = "Consumed_ReqId";
    QString Transaction_Fifo_Helper_Cr_Qty = "Cr_Qty";
    QString Transaction_Fifo_Helper_Dr_Qty = "Dr_Qty";
    QString Transaction_Fifo_Helper_Cr_Amount = "Cr_Amount";
    QString Transaction_Fifo_Helper_Dr_Amount = "Dr_Amount";
    QString Transaction_Fifo_Helper_Price = "Price";

    DatabaseHelper *db;
    int insertFifoEntry(ItemTransactionDataModel *obj, QList<ItemFifoDataModel*> fifoList);
    int deleteFifoEntry(GeneralVoucherDataObject *obj);
    QSqlQueryModel *getItemQtyListFifo(GeneralVoucherDataObject *voucher, QString itemID, QDate date, QString godownID="%");
    QList<ItemFifoDataModel*> getFifoListByTransId(int transId);



};

#endif // TRANSACTIONFIFODATABASEHELPER_H
