
#include "bundledatabasehelper.h"

BundleDatabaseHelper::BundleDatabaseHelper()
{
    //    this->db = db;
}

void BundleDatabaseHelper::insertMaster(CompoundItemDataObject *item)
{

    QMap<QString,QVariant> map;
    for(int i=0;i<item->subItems.length();i++){
        map.clear();
        map.insert(Bundle_Master_Base_Item,item->BaseItem.ItemID);
        map.insert(Bundle_Master_Free_Item,item->subItems[i].ItemID);
        map.insert(Bundle_Master_Free_Quantity,item->subItems[i].discQuantity);
        DatabaseHelper::insertRecordsPlusStatic(map,Bundle_Master_TableName);
    }
}

void BundleDatabaseHelper::deleteMaster(QString itemID)
{
    QMap<QString,QVariant> map;
    map.insert(Bundle_Master_Base_Item,itemID);
    DatabaseHelper:: deleteRecordsStatic(map,Bundle_Master_TableName);

}

QList<inventoryItemDataModel> BundleDatabaseHelper::getBundledItemsMaster(QString itemID)
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    QList<inventoryItemDataModel> freeList;
    QString qStr = " SELECT ";
    qStr += Bundle_Master_Free_Item +", "+ Bundle_Master_Free_Quantity;
    qStr += " FROM "+ Bundle_Master_TableName;
    qStr += " WHERE "+ Bundle_Master_Base_Item + " = ':itemID'";

    query.prepare(qStr);
    query.bindValue(":itemID",itemID);

    if(query.exec()){
        while(query.next()){
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(0).toString());
            item.discQuantity = query.value(1).toFloat();
            freeList.append(item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();

    }

    return freeList;
}

void BundleDatabaseHelper::insertBundleTransaction(GeneralVoucherDataObject *voucher)
{
    QMap<QString,QVariant> map;
    for(int i=0;i<voucher->InventoryItems.size();i++){

        // The Base Item
        if(voucher->InventoryItems[i].BaseItem.isBundled){
            for(int j=0;j<voucher->InventoryItems[i].subItems.length();j++){
                if(voucher->InventoryItems[i].subItems.length()>0){
                    // Insert SubItems
                    map.clear();
                    map.insert(Bundle_Transaction_VNo,voucher->voucherNumber);
                    map.insert(Bundle_Transaction_VPref,voucher->VoucherPrefix);
                    map.insert(Bundle_Transaction_VType,voucher->voucherType);
                    map.insert(Bundle_Transaction_VDate,voucher->VoucherDate);
                    map.insert(Bundle_Transaction_TransactionID,voucher->TransactionId);
                    if(voucher->ledgerObject.LedgerGroupId=="0x5x19")
                        map.insert(Bundle_Transaction_Ledger_ID,voucher->ledgerObject.LedgerID);
                    map.insert(Bundle_Transaction_Contact_ID,voucher->Contact._id);
                    map.insert(Bundle_Transaction_Item_ID,voucher->InventoryItems[i].subItems[j].ItemID);
                    map.insert(Bundle_Transaction_Free_Quantity,voucher->InventoryItems[i].subItems[j].quantity);
                    map.insert(Bundle_Transaction_Used_Quantity,0);
                    DatabaseHelper::insertRecordsPlusStatic(map,Bundle_Transaction_TableName);


                }

            }
        }
        // Item claimed free.
        else if(voucher->InventoryItems[i].BaseItem.discQuantity){
            map.clear();
            map.insert(Bundle_Transaction_VNo,voucher->voucherNumber);
            map.insert(Bundle_Transaction_VPref,voucher->VoucherPrefix);
            map.insert(Bundle_Transaction_VType,voucher->voucherType);
            map.insert(Bundle_Transaction_VDate,voucher->VoucherDate);
            map.insert(Bundle_Transaction_TransactionID,voucher->TransactionId);
            if(voucher->ledgerObject.LedgerGroupId=="0x5x19")
                map.insert(Bundle_Transaction_Ledger_ID,voucher->ledgerObject.LedgerID);
            map.insert(Bundle_Transaction_Contact_ID,voucher->Contact._id);
            map.insert(Bundle_Transaction_Item_ID,voucher->InventoryItems[i].BaseItem.ItemID);
            map.insert(Bundle_Transaction_Free_Quantity,0);
            map.insert(Bundle_Transaction_Used_Quantity,voucher->InventoryItems[i].BaseItem.discQuantity);
            DatabaseHelper::insertRecordsPlusStatic(map,Bundle_Transaction_TableName);

        }
    }
}

void BundleDatabaseHelper::deleteBundleTransaction(GeneralVoucherDataObject *voucher)
{
    QMap<QString,QVariant> map;
    map.insert(Bundle_Transaction_TransactionID,voucher->TransactionId);
    DatabaseHelper::deleteRecordsStatic(map,Bundle_Transaction_TableName);
}

QSqlQueryModel *BundleDatabaseHelper::getPendingFreeListByContact(QString contactID, QDate asOfDate,QString voucherDate)
{
    QString qStr = "";

    qStr += " SELECT t1."+Bundle_Transaction_Item_ID+",";
    qStr += " (SUM(t2."+Bundle_Transaction_Free_Quantity+") - SUM(t2."+Bundle_Transaction_Used_Quantity+")) AS Balance";
    qStr += " FROM "+Bundle_Transaction_TableName+" AS t1 ";
    qStr += " JOIN "+Bundle_Transaction_TableName+" AS t2 ON t2."+Bundle_Transaction_Contact_ID+" = t1."+Bundle_Transaction_Contact_ID+" ";
    qStr += " AND t2."+Bundle_Transaction_VDate+" <= str_to_date("+DatabaseHelper::getDateStringStatic(asOfDate)+") ";
    qStr += " WHERE t1."+Bundle_Transaction_Contact_ID+" = '"+contactID+"' ";
    qStr += " AND t1."+Bundle_Transaction_VDate+" = str_to_date("+DatabaseHelper::getDateStringStatic(asOfDate)+") ";
    qStr += " GROUP BY t1."+Bundle_Transaction_Item_ID+" ";
    qStr += " HAVING Balance > 0";

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *BundleDatabaseHelper::getPendingFreeListByLedger(QString LedgerID, QDate asOfDate,QString voucherDate)
{
    QString qStr="";
    qStr += " SELECT t1."+Bundle_Transaction_Item_ID+",";
    qStr += " (SUM(t2."+Bundle_Transaction_Free_Quantity+") - SUM(t2."+Bundle_Transaction_Used_Quantity+")) AS Balance";
    qStr += " FROM "+Bundle_Transaction_TableName+" AS t1 ";
    qStr += " JOIN "+Bundle_Transaction_TableName+" AS t2 ON t2."+Bundle_Transaction_Ledger_ID+" = t1."+Bundle_Transaction_Ledger_ID+" ";
    qStr += " AND t2."+Bundle_Transaction_VDate+" <= str_to_date("+DatabaseHelper::getDateStringStatic(asOfDate)+") ";
    qStr += " WHERE t1."+Bundle_Transaction_Ledger_ID+" = '"+LedgerID+"' ";
    qStr += " AND t1."+Bundle_Transaction_VDate+" = str_to_date("+DatabaseHelper::getDateStringStatic(asOfDate)+") ";
    qStr += " GROUP BY t1."+Bundle_Transaction_Item_ID+" ";
    qStr += " HAVING Balance > 0";

    return DatabaseHelper::getQueryModel(qStr);
}

