#include "itemtransactiondatabasehelper.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

ItemTransactionDatabaseHelper::ItemTransactionDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
    fifoHelper = new TransactionFifoDatabaseHelper(db);
}

int ItemTransactionDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int ItemTransactionDatabaseHelper::insertItemTransaction(ItemTransactionDataModel *obj,
                                                         QList<GodownTransactionDataModel> godownList)
{
    qDebug()<<Q_FUNC_INFO;
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> map1;
    QMap<QString, QVariant> map2;

    BillOfMaterialsIngredientsDataBaseHelper *bomHelper = new BillOfMaterialsIngredientsDataBaseHelper();
    QList<RestaurantIngredientsDataModel> bomList;

    qDebug()<<"godownlist size"<<godownList.size();
    if( godownList.size()>1 ){
        for(GodownTransactionDataModel a:godownList){
            qDebug()<<"inserting entry for godown "<<a.fromGodown<<a.toGodown;
            map1.insert(Transaction_Item_Helper_ItemID,obj->ItemID);
            map1.insert(Transaction_Item_Helper_From_Ledger,obj->From_Ledger);
            map1.insert(Transaction_Item_Helper_To_Ledger,obj->To_Ledger);
            map1.insert(Transaction_Item_Helper_VoucherID,obj->VoucherID);
            map1.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
            map1.insert(Transaction_Item_Helper_VoucherDate,obj->VoucherDate);
            map1.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
            map1.insert(Transaction_Item_Helper_From_Location,a.fromGodown);
            map1.insert(Transaction_Item_Helper_To_Location,a.toGodown);
            map1.insert(Transaction_Item_Helper_Cr_Qty,a.crQty);
            map1.insert(Transaction_Item_Helper_Dr_Qty,a.drQty);
            map1.insert(Transaction_Item_Helper_Cr_Amount,a.crQty * obj->Price);
            map1.insert(Transaction_Item_Helper_Dr_Amount,a.drQty * obj->Price);
            map.insert(Transaction_Item_Helper_Price, obj->Price);
            map1.insert(Transaction_Item_Helper_Narration,obj->Narration);
            map1.insert(Transaction_Item_Helper_ItemReqId, obj->itemReqId);
            map1.insert(Transaction_Item_Helper_Time,QDateTime::currentDateTime());
            obj->_id = db->insertRecordsPlus(map1,Transaction_Item_Helper_TableName);

            //            fifoHelper->insertFifoEntry(obj, a.fifoList);

        }
    }
    else{
        qDebug()<<"inserting trans for "<<obj->VoucherID<<obj->VoucherPrefix<<obj->VoucherType;
        map.insert(Transaction_Item_Helper_ItemID,obj->ItemID);
        map.insert(Transaction_Item_Helper_From_Ledger,obj->From_Ledger);
        map.insert(Transaction_Item_Helper_To_Ledger,obj->To_Ledger);
        map.insert(Transaction_Item_Helper_VoucherID,obj->VoucherID);
        map.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
        map.insert(Transaction_Item_Helper_VoucherDate,obj->VoucherDate);
        map.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
        map.insert(Transaction_Item_Helper_From_Location,obj->From_Location);
        map.insert(Transaction_Item_Helper_To_Location,obj->To_Location);
        map.insert(Transaction_Item_Helper_Cr_Qty,obj->crQty);
        map.insert(Transaction_Item_Helper_Dr_Qty,obj->drQty);
        map.insert(Transaction_Item_Helper_Consumed, obj->consumed);
        map.insert(Transaction_Item_Helper_Cr_Amount,obj->crAmount);
        map.insert(Transaction_Item_Helper_Dr_Amount,obj->drAmount);
        map.insert(Transaction_Item_Helper_Price, obj->Price);
        map.insert(Transaction_Item_Helper_Narration,obj->Narration);
        map.insert(Transaction_Item_Helper_ItemReqId, obj->itemReqId);
        map.insert(Transaction_Item_Helper_Time,QDateTime::currentDateTime());


        obj->_id = db->insertRecordsPlus(map,Transaction_Item_Helper_TableName);
    }


    bomList = BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(obj->ItemID) ;

    if(bomList.size()>0
            && QString::compare(obj->VoucherType,
             GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder),
             Qt::CaseInsensitive)){

        qDebug()<<Q_FUNC_INFO<<__LINE__<<"insert in bom";
        for(RestaurantIngredientsDataModel bom:bomList){

            float ratio = bom.PurchaseItemQty/bom.SalesItemQty;
            float crQty = obj->crQty*ratio;
            float drQty = obj->drQty*ratio;
            QString tableName = Transaction_Bom_Helper_TableName;

            if(bom.TakeAway)
                continue;
            //            if(bom.TakeAway){
            //                 ratio=1;
            //                 crQty = obj->crQty;
            //                 drQty = obj->drQty;
            //                 tableName = Transaction_Item_Helper_TableName;
            //            }

            //            QList<ItemFifoDataModel*> fifoList2 = getItemPriceForQtyByFifoWithUpdate(bom.PurchaseItem, obj->crQty*ratio, obj->VoucherDate);
            //            if(obj->crQty > 0 && fifoList2.size()>0){

            //                for(ItemFifoDataModel* a:fifoList2){
            //                    map2.insert(Transaction_Item_Helper_ItemID,bom.PurchaseItem);
            //                    map2.insert(Transaction_Item_Helper_From_Ledger,obj->From_Ledger);
            //                    map2.insert(Transaction_Item_Helper_To_Ledger,obj->To_Ledger);
            //                    map2.insert(Transaction_Item_Helper_VoucherID,obj->VoucherID);
            //                    map2.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
            //                    map2.insert(Transaction_Item_Helper_VoucherDate,obj->VoucherDate);
            //                    map2.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
            //                    map2.insert(Transaction_Item_Helper_From_Location,a->From_Location);
            //                    map2.insert(Transaction_Item_Helper_To_Location,a->To_Location);
            //                    map2.insert(Transaction_Item_Helper_Cr_Qty,a->consumedQty);
            //                    map2.insert(Transaction_Item_Helper_Dr_Qty,0);
            //                    map2.insert(Transaction_Item_Helper_Cr_Amount,a->consumedQty*a->Price);
            //                    map2.insert(Transaction_Item_Helper_Dr_Amount,0);
            //                    map2.insert(Transaction_Item_Helper_Price, a->Price);
            //                    map2.insert(Transaction_Item_Helper_Narration,obj->Narration);
            //                    map2.insert(Transaction_Item_Helper_Time,QDateTime::currentDateTime());

            //                    db->insertRecordsPlus(map2,Transaction_Bom_Helper_TableName);


            //                }
            ////                updateConsumedQty(fifoList2);
            //            }

            //            else{

            map2.insert(Transaction_Item_Helper_ItemID,bom.PurchaseItem);
            map2.insert(Transaction_Item_Helper_From_Ledger,obj->From_Ledger);
            map2.insert(Transaction_Item_Helper_To_Ledger,obj->To_Ledger);
            map2.insert(Transaction_Item_Helper_VoucherID,obj->VoucherID);
            map2.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
            map2.insert(Transaction_Item_Helper_VoucherDate,obj->VoucherDate);
            map2.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
            if(obj->crQty > 0)
                map2.insert(Transaction_Item_Helper_From_Location , bom.purchaseItemSection);
            else
                map2.insert(Transaction_Item_Helper_From_Location,obj->From_Location);
            if(obj->drQty > 0)
                map2.insert(Transaction_Item_Helper_To_Location , bom.purchaseItemSection);
            else
                map2.insert(Transaction_Item_Helper_To_Location,obj->To_Location);

            map2.insert(Transaction_Item_Helper_Cr_Qty,crQty);
            map2.insert(Transaction_Item_Helper_Dr_Qty,drQty);
            map2.insert(Transaction_Item_Helper_Cr_Amount,obj->crAmount*ratio);  //THIS IS WRONG
            map2.insert(Transaction_Item_Helper_Dr_Amount,obj->drAmount*ratio);  //THIS IS WRONG
            map2.insert(Transaction_Item_Helper_Narration,obj->Narration);
            map.insert(Transaction_Item_Helper_ItemReqId, obj->itemReqId);

            map2.insert(Transaction_Item_Helper_Time,QDateTime::currentDateTime());

            db->insertRecordsPlus(map2,Transaction_Bom_Helper_TableName);
            //            }
        }
    }
}

int ItemTransactionDatabaseHelper::updateItemTransaction(ItemTransactionDataModel *obj, GeneralVoucherDataObject *old)
{
    //    this->startTransaction();
    QMap<QString,QVariant> map;

    map.insert(Transaction_Item_Helper_ItemID,obj->ItemID);
    map.insert(Transaction_Item_Helper_From_Ledger,obj->From_Ledger);
    map.insert(Transaction_Item_Helper_To_Ledger,obj->To_Ledger);
    map.insert(Transaction_Item_Helper_VoucherID,obj->VoucherID);
    map.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
    map.insert(Transaction_Item_Helper_VoucherDate,obj->VoucherDate);
    map.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
    map.insert(Transaction_Item_Helper_From_Location,obj->From_Location);
    map.insert(Transaction_Item_Helper_To_Location,obj->To_Location);
    map.insert(Transaction_Item_Helper_Cr_Qty,obj->crQty);
    map.insert(Transaction_Item_Helper_Dr_Qty,obj->drQty);
    map.insert(Transaction_Item_Helper_Cr_Amount,obj->crAmount);
    map.insert(Transaction_Item_Helper_Dr_Amount,obj->drAmount);
    map.insert(Transaction_Item_Helper_Narration,obj->Narration);
    QMap<QString,QVariant> clause;

    clause.insert(Transaction_Item_Helper_VoucherID,old->voucherNumber);
    clause.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
    clause.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
    db->updateRecordsPlus(map,clause,Transaction_Item_Helper_TableName);
    //    return commit();
}

int ItemTransactionDatabaseHelper::deleteItemTransaction(GeneralVoucherDataObject *old)
{
    //    this->startTransaction();

    //    updateConsumedQtyForDelete(old);
    qDebug()<<Q_FUNC_INFO<<old->voucherType<<old->voucherNumber<<old->VoucherPrefix;
    QMap<QString,QVariant> map;


    map.insert(Transaction_Item_Helper_VoucherID,old->voucherNumber);
    map.insert(Transaction_Item_Helper_VoucherPrefix,old->VoucherPrefix);
    map.insert(Transaction_Item_Helper_VoucherType,old->voucherType);
    db->deleteRecordsPlus(map,Transaction_Item_Helper_TableName);
    db->deleteRecordsPlus(map,Transaction_Bom_Helper_TableName);
    //    fifoHelper->deleteFifoEntry(old);

    //    return commit();
}

int ItemTransactionDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        flag=true;
        return 0;
    }
}


bool ItemTransactionDatabaseHelper::isSalesItemByItemId(QString itemID)
{
    bool name = false;
    QString qStr = "SELECT "+Sales_Inventory_isSalesItem + " FROM " + Sales_Inventory_Table_Name
            + " WHERE "+Sales_Inventory_ItemId + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",itemID);

    if(query.exec()){
        while (query.next()) {
            name =  query.value(0).toBool();
        }
    }
    else{
        //qDebug()<<query.lastError();
        //qDebug()<<query.lastQuery(); //qDebug()<<"219";
    }
    return name;
}

bool ItemTransactionDatabaseHelper::isPurchaseItemByItemId(QString itemID)
{
    //    //qDebug()<<"inside function 999"<<itemID<<Sales_Inventory_isPurchaseItem;
    bool name = false;
    QString qStr = "SELECT "+Sales_Inventory_isPurchaseItem + " FROM " + Sales_Inventory_Table_Name;
    //    //qDebug()<<"99991"<<qStr;
    qStr +=  " WHERE "+Sales_Inventory_ItemId + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    //qDebug()<<qStr;
    query.prepare(qStr);
    query.bindValue(":id",itemID);
    //qDebug()<<qStr;
    //qDebug()<<query.boundValues();

    if(query.exec()){
        if (query.first()) {
            name =  query.value(0).toBool();
        }
    }
    else{
        //qDebug()<<query.lastError();
        //qDebug()<<query.lastQuery(); //qDebug()<<"219";
    }
    //qDebug()<<name;
    return name;
}

GeneralVoucherDataObject *ItemTransactionDatabaseHelper::getVoucherToExport(int sourceVoucherType, int destinationVoucherType, QString ItemReqId)
{
    GeneralVoucherDataObject *voucher = new GeneralVoucherDataObject();
    CompoundItemDataObject compItem;
    inventoryItemDataModel item;
    SalesInventoryItemDatabaseHelper shelper;

    float srcQty =0, desQty=0;
    QString ItemID;
    QString qStr = "";

    qStr = " SELECT " + Transaction_Item_Helper_ItemID + ", " + Transaction_Item_Helper_VoucherType + ", ";
    qStr +=  " SUM(" + Transaction_Item_Helper_Cr_Qty + " - " + Transaction_Item_Helper_Dr_Qty + ")";
    qStr += " FROM " + Transaction_Item_Helper_TableName ;
    //    qStrSrc += " WHERE " + Transaction_Item_Helper_VoucherType + " = '" + sourceVoucherType + "'";
    qStr += " WHERE " + Transaction_Item_Helper_ItemReqId + " = '" + ItemReqId + "'";
    qStr += " GROUP BY " + Transaction_Item_Helper_VoucherType;

    //    //qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while(query.next()){
            ItemID = query.value(0).toString();
            if(query.value(1).toString() == sourceVoucherType)
                srcQty = qFabs(query.value(1).toFloat());
            if(query.value(1).toString() == destinationVoucherType)
                desQty = qFabs(query.value(1).toFloat());
        }
    }
    else{
        //qDebug()<<query.lastError()<<" in query ";
        //qDebug()<<query.lastQuery();
    }

    item = shelper.getInventoryItemByID(ItemID);
    item.maxQuantity = srcQty - desQty;
    compItem.BaseItem = item;
    voucher->InventoryItems.append(compItem);

    return voucher;
}

QList<ItemFifoDataModel*> ItemTransactionDatabaseHelper::getItemPriceForQtyByFifoWithUpdate(QString itemID, float quantity, QDate date)
{
    //    //qDebug()<<"fifo1";
    QList<ItemFifoDataModel*> itemList;
    //    //qDebug()<<"fifo1"<<quantity<<itemList.size()<<itemID;

    if(quantity >0 && isPurchaseItemByItemId(itemID)){

        //        //qDebug()<<"fifo2";

        QString qStrFifo = " SELECT " + Transaction_Item_Helper_VoucherID + ", ";
        qStrFifo += Transaction_Item_Helper_VoucherType + ", " + Transaction_Item_Helper_VoucherPrefix + ", ";
        qStrFifo += Transaction_Item_Helper_ItemID + ", " + Transaction_Item_Helper_Dr_Qty ;
        qStrFifo += " - coalesce(" + Transaction_Item_Helper_Consumed + ", 0),  coalesce(" + Transaction_Item_Helper_Consumed + ", 0), ";
        qStrFifo += Transaction_Item_Helper_Price + " from " + Transaction_Item_Helper_TableName;
        qStrFifo += " where " + Transaction_Item_Helper_Dr_Qty + " - " ;
        qStrFifo += " coalesce(" + Transaction_Item_Helper_Consumed + ",0) > 0 ";
        qStrFifo += " AND " + Transaction_Item_Helper_ItemID + " = :item ";
        qStrFifo += " AND " + Transaction_Item_Helper_VoucherDate ; //+ ", " + Transaction_Item_Helper_Time + ") ";
        qStrFifo += " <= " + db->getDateString(date);
        qStrFifo += " ORDER BY " +Transaction_Item_Helper_VoucherDate + " LIMIT 1";

        //        //qDebug()<<"fifo2";

        //    QString qStrUpdate = " UPDATE FIFO_TEST SET CR_QTY = CR_QTY + :crQty WHERE _id = :id ";
        //    QString qStrDelete = " DELETE FROM FIFO_TEST WHERE _id = :id";

        QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
        QSqlQuery query1(QSqlDatabase::database(DatabaseValues::connectionString));
        QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));
        QSqlQuery query3(QSqlDatabase::database(DatabaseValues::connectionString));
        QSqlQuery query4(QSqlDatabase::database(DatabaseValues::connectionString));


        query.prepare(qStrFifo);
        query.bindValue(":item", itemID);
        //qDebug()<<query.lastQuery();
        //        //qDebug()<<"fifo4";

        float balance=quantity;
        while(balance>0){
            //            //qDebug()<<"in loop"<<balance;
            if(!query.exec()){
                //qDebug()<<query.lastError();
                //qDebug()<<query.lastQuery();
            }
            else{
                //                //qDebug()<<"Query returns"<<query.size();
                //qDebug()<<query.lastQuery();
                //qDebug()<<query.boundValues();
                if(query.size()>0){
                    if(query.first()){

                        ItemFifoDataModel* fifoObj = new ItemFifoDataModel();
                        fifoObj->VoucherID = query.value(0).toString();
                        fifoObj->VoucherType = query.value(1).toString();
                        fifoObj->VoucherPrefix = query.value(2).toString();
                        fifoObj->ItemID = query.value(3).toString();

                        //                        //qDebug()<<query.value(4).toFloat()<<balance;

                        if(query.value(4).toFloat() >= balance){
                            fifoObj->consumedQty = balance;
                        }
                        else {
                            fifoObj->consumedQty = query.value(4).toFloat();

                        }

                        //                        //qDebug()<<fifoObj->consumedQty;

                        balance -= fifoObj->consumedQty;
                        fifoObj->drQty = query.value(5).toFloat(); // current consumed value, not actual dr
                        //                        //qDebug()<<"Bal"<<balance<<fifoObj->consumedQty<<quantity<<query.value(4).toFloat();
                        fifoObj->Price = query.value(6).toFloat();
                        fifoObj->crQty = quantity;
                        //                        updateConsumedQty(fifoObj);
                        itemList.append(fifoObj);
                    }
                    else{
                        //                    break;
                    }
                }
                else{
                    balance =0;
                }
            }
        }
    }

    else{
        //qDebug()<<"fifo na";
    }


    return itemList;
}


QSqlQueryModel *ItemTransactionDatabaseHelper::getItemQtyListFifo(QString itemID,  QDate date,  QString godownID)
{
    //    QList<ItemFifoDataModel*> itemList;

    QString qStrFifo = " SELECT " + Transaction_Item_Helper_VoucherID + ", ";
    qStrFifo += Transaction_Item_Helper_VoucherType + ", " + Transaction_Item_Helper_VoucherPrefix + ", ";
    qStrFifo += Transaction_Item_Helper_ItemID + ", " ;
    qStrFifo += Transaction_Item_Helper_ItemReqId + ", ";
    qStrFifo += Transaction_Item_Helper_Dr_Qty + ", ";
    qStrFifo += " coalesce(" + Transaction_Item_Helper_Consumed + ", 0), ";
    qStrFifo += Transaction_Item_Helper_Price + " from " + Transaction_Item_Helper_TableName;
    qStrFifo += " where " + Transaction_Item_Helper_Dr_Qty + " - " ;
    qStrFifo += " coalesce(" + Transaction_Item_Helper_Consumed + ",0) > 0 ";
    qStrFifo += " AND " + Transaction_Item_Helper_ItemID + " = '" + itemID + "'";
    qStrFifo += " AND " + Transaction_Item_Helper_VoucherDate ; //+ ", " + Transaction_Item_Helper_Time + ") ";
    qStrFifo += " <= " + db->getDateString(date);
    qStrFifo += " AND " + Transaction_Item_Helper_To_Location + " like '" + godownID + "'";
    qStrFifo += " ORDER BY " +Transaction_Item_Helper_VoucherDate;


    //        QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //        query.prepare(qStrFifo);
    //        query.bindValue(":item", itemID);

    //            if(!query.exec()){
    //                //qDebug()<<query.lastError();
    //                //qDebug()<<query.lastQuery();
    //            }
    //            else{
    //                    while(query.next()){

    //                        ItemFifoDataModel* fifoObj = new ItemFifoDataModel();
    //                        fifoObj->VoucherID = query.value(0).toString();
    //                        fifoObj->VoucherType = query.value(1).toString();
    //                        fifoObj->VoucherPrefix = query.value(2).toString();
    //                        fifoObj->ItemID = query.value(3).toString();
    //                        fifoObj->drQty = query.value(4).toFloat(); // current consumed value, not actual dr
    //                        fifoObj->consumedQty = query.value(5).toFloat();
    //                        fifoObj->Price = query.value(6).toFloat();
    //                        itemList.append(fifoObj);
    //                    }

    //                }

    //        //qDebug()<<qStrFifo;

    return DatabaseHelper::getQueryModel(qStrFifo);
}

QList<ItemFifoDataModel *> ItemTransactionDatabaseHelper::
getFifoListByVoucherItem(QString voucherNo, QString voucherType, QString voucherPrefix, QString itemID)
{
    QList<ItemFifoDataModel *> fifoList;
    QString qStr = " SELECT " + Transaction_Item_Helper_ItemID + ", ";
    qStr += Transaction_Item_Helper_Cr_Qty + ", " + Transaction_Item_Helper_Dr_Qty + ", ";
    qStr += Transaction_Item_Helper_Consumed + ", " + Transaction_Item_Helper_ConsumedReqId ;
    qStr += ", " + Transaction_Item_Helper_Price + ", ";
    qStr += Transaction_Item_Helper_From_Location + ", " + Transaction_Item_Helper_To_Location;
    qStr += " FROM " + Transaction_Item_Helper_TableName + " WHERE ";
    qStr += Transaction_Item_Helper_VoucherID + " = '" + voucherNo + "'";
    qStr += " AND " + Transaction_Item_Helper_VoucherPrefix + " = '" + voucherPrefix + "'";
    qStr += " AND " + Transaction_Item_Helper_VoucherType + " = '" + voucherType + "'";
    qStr += " AND " + Transaction_Item_Helper_ItemID + " = '" + itemID + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while (query.next()) {

            int i=0;
            ItemFifoDataModel *fifoObj = new ItemFifoDataModel();

            fifoObj->VoucherID = voucherNo;
            fifoObj->VoucherType = voucherType;
            fifoObj->VoucherPrefix = voucherPrefix;
            fifoObj->ItemID = query.value(i++).toString();
            fifoObj->crQty = query.value(i++).toFloat();
            fifoObj->drQty = query.value(i++).toFloat();
            fifoObj->consumedQty = query.value(i++).toFloat();
            if(fifoObj->crQty > 0)
                fifoObj->consumedQty = 0;
            fifoObj->ItemReqId = query.value(i++).toString();
            fifoObj->Price = query.value(i++).toFloat();
            fifoObj->From_Location = query.value(i++).toString();
            fifoObj->To_Location = query.value(i++).toString();

            fifoList.append(fifoObj);


        }
    }
    else {
        //qDebug()<<query.lastError();
        //qDebug()<<query.lastQuery()<<"Error above";
    }

    return fifoList;

}

QList<GodownTransactionDataModel> ItemTransactionDatabaseHelper::getGodownAllocationByVoucherAndItem(QString voucherNo, QString voucherType, QString voucherPrefix, QString itemReqId)
{
    qDebug()<<Q_FUNC_INFO;
    QList<GodownTransactionDataModel> godownList;

    QString qStr = " SELECT " + Transaction_Item_Helper_id + ", ";
    qStr += Transaction_Item_Helper_From_Location + ", ";
    qStr += Transaction_Item_Helper_To_Location + ", ";
    qStr += Transaction_Item_Helper_Cr_Qty + ", ";
    qStr += Transaction_Item_Helper_Dr_Qty + " FROM " + Transaction_Item_Helper_TableName;
    qStr += " WHERE " + Transaction_Item_Helper_VoucherID + " = '" + voucherNo + "' ";
    qStr += " AND " + Transaction_Item_Helper_VoucherType + " = '" + voucherType + "' ";
    qStr += " AND " + Transaction_Item_Helper_VoucherPrefix + " = '" + voucherPrefix + "' ";
    qStr += " AND " + Transaction_Item_Helper_ItemReqId + " = '" + itemReqId + "' ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    qDebug()<<qStr;
    if(query.exec())    {

        while(query.next()) {
            qDebug()<<"534"<<query.at();
            int i=1;
            GodownTransactionDataModel godownEntry ;

            //            godownEntry.fifoList = fifoHelper->getFifoListByTransId(query.value(i++).toInt());
            godownEntry.fromGodown = query.value(i++).toString();
            godownEntry.toGodown = query.value(i++).toString();
            godownEntry.crQty = query.value(i++).toFloat();
            godownEntry.drQty = query.value(i++).toFloat();

            godownList.append(godownEntry);
            qDebug()<<"added "<<godownList.size();

        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    qDebug()<<"godown list : (((((((((((((()))))))))))))))))";
    for(GodownTransactionDataModel entry:godownList)
        qDebug()<<entry.fromGodown<<entry.toGodown<<entry.crQty<<entry.drQty;

    return godownList;
}

bool ItemTransactionDatabaseHelper::hasRecordsForVoucherTypeByItemReqId(QString itemReqId, QString voucherType)
{
    bool data = false;

    //    //qDebug()<<" Check item trans for "<<itemReqId<<voucherType;

    //    //qDebug()<<"start";
    QString qStr = " SELECT _id FROM transaction_item_helper where ";

    qStr += " Requirement_ItemID = '" + itemReqId + "' AND ";

    qStr +=  " VoucherType = '" + voucherType + "' ";

    //qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);


    if(query.exec()){
        if(query.first())
            data = true;
    }
    else{
        //qDebug()<<query.lastError();
        //qDebug()<<query.lastQuery();

    }

    return data;
}

float ItemTransactionDatabaseHelper::getItemPriceToDisplayByFifo(QString itemID)
{
    float price=0;

    QString qStrFifo = " SELECT " ;
    qStrFifo += Transaction_Item_Helper_Price + " from " + Transaction_Item_Helper_TableName;
    qStrFifo += " where " + Transaction_Item_Helper_Dr_Qty + " - " ;
    qStrFifo += " coalesce(" + Transaction_Item_Helper_Consumed + ",0) > 0 ";
    qStrFifo += " AND " + Transaction_Item_Helper_ItemID + " = :item ";
    qStrFifo += " ORDER BY " +Transaction_Item_Helper_VoucherDate + " LIMIT 1";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStrFifo);
    query.bindValue(":item", itemID);

    if(!query.exec()){
        //qDebug()<<query.lastError();
        //qDebug()<<query.lastQuery();
    }

    else{
        if(query.first())    {
            price = query.value(0).toFloat();
        }
    }

    return price;
}

int ItemTransactionDatabaseHelper::updateConsumedQty(QList<ItemFifoDataModel*>  fifoList)
{
    QMap<QString,QVariant> map;
    int c = 0;

    QMap<QString,QVariant> map2;

    float currentConsumed=0;

    QMap<QString, float> consumedMap;

    for(ItemFifoDataModel *fifoRow : fifoList){
        //qDebug()<<"in fifo loop: "<<fifoRow->_id<<fifoRow->ItemReqId<<fifoRow->consumedQty;

        QString qStr = " SELECT " + Transaction_Item_Helper_Consumed + " FROM " + Transaction_Item_Helper_TableName;
        qStr += " WHERE " + Transaction_Item_Helper_ItemReqId + " = '" + fifoRow->ItemReqId + "'";
        qStr += " AND " + Transaction_Item_Helper_VoucherType + " = '" + fifoRow->VoucherType + "'";
        qStr += " AND " + Transaction_Item_Helper_VoucherID + " = '" + fifoRow->VoucherID + "'";
        qStr += " AND " + Transaction_Item_Helper_VoucherPrefix + " = '" + fifoRow->VoucherPrefix + "'";
        //qDebug()<<qStr;

        QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
        query.prepare(qStr);
        if(query.exec()){
            if(query.first()){
                if(!consumedMap.contains(fifoRow->ItemReqId)){
                    consumedMap.insert(fifoRow->ItemReqId, query.value(0).toFloat());
                }
                else{
                    consumedMap[fifoRow->ItemReqId] += query.value(0).toFloat();
                }
            }
        }
        else{
            //qDebug()<<query.lastError();
            //qDebug()<<query.lastQuery();
        }

        //qDebug()<<"Update consumed qty for ";
        //qDebug()<<"voucherDate: "<<fifoRow->VoucherDate<<fifoRow->VoucherID;
        //qDebug()<<"Item:"<<fifoRow->ItemID;
        //qDebug()<<"consumed, dr"<<fifoRow->consumedQty<<fifoRow->drQty<<fifoRow->crQty<<currentConsumed;

        //qDebug()<<"Update to"<<fifoRow->consumedQty + currentConsumed<<fifoRow->ItemReqId <<consumedMap[fifoRow->ItemReqId];

        //    for(ItemFifoDataModel* a:fifoList){
        map.insert(Transaction_Item_Helper_Consumed, fifoRow->consumedQty /*+ consumedMap[fifoRow->ItemReqId]*/);

        map2.insert(Transaction_Item_Helper_ItemReqId, fifoRow->ItemReqId);
        map2.insert(Transaction_Item_Helper_VoucherType, fifoRow->VoucherType);
        map2.insert(Transaction_Item_Helper_VoucherID, fifoRow->VoucherID);
        map2.insert(Transaction_Item_Helper_VoucherPrefix, fifoRow->VoucherPrefix);

        c = db->updateRecordsPlus(map,map2,Transaction_Item_Helper_TableName);
    }
    return c;
}

void ItemTransactionDatabaseHelper::updateConsumedQtyForDelete(GeneralVoucherDataObject *voucher)
{

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));
    QMap<QString, float> itemMap;

    QString qStrFifo = " SELECT " + Transaction_Item_Helper_id + ", ";
    qStrFifo += " coalesce(" + Transaction_Item_Helper_Consumed + ", 0) ";
    qStrFifo += " from " + Transaction_Item_Helper_TableName;
    qStrFifo += " where coalesce(" + Transaction_Item_Helper_Consumed + ",0) > 0 ";
    qStrFifo += " AND " + Transaction_Item_Helper_ItemID + " = :item ";
    qStrFifo += " AND " + Transaction_Item_Helper_VoucherDate ; //+ ", " + Transaction_Item_Helper_Time + ") ";
    qStrFifo += " <= " + db->getDateString(voucher->VoucherDate);
    qStrFifo += " ORDER BY " +Transaction_Item_Helper_VoucherDate + " DESC LIMIT 1";

    QString qStr = " SELECT " + Transaction_Item_Helper_ItemID + ", SUM(" + Transaction_Item_Helper_Cr_Qty + ") from ";
    qStr += Transaction_Item_Helper_TableName + " WHERE " ;
    qStr += Transaction_Item_Helper_VoucherID + " = '" + voucher->voucherNumber + "'";
    qStr += " AND " + Transaction_Item_Helper_VoucherType + " = '" + voucher->voucherType + "'";
    qStr += " AND " + Transaction_Item_Helper_VoucherPrefix + " = '" + voucher->VoucherPrefix + "'";
    qStr += " GROUP BY " + Transaction_Item_Helper_ItemID;

    query.prepare(qStr);
    if(!query.exec()){
        //qDebug()<<query.lastError();
        //qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            if(query.value(1).toFloat() > 0)
                itemMap.insert(query.value(0).toString(), query.value(1).toFloat());
        }
    }
    //    //qDebug()<<"Existing records"<<itemMap;
    QMapIterator<QString, float> iterator(itemMap);
    if(itemMap.size()>0){
        query2.prepare(qStrFifo);
        iterator.toFront();
        while (iterator.hasNext()) {
            //            //qDebug()<<"##############################";
            iterator.next();
            query2.bindValue(":item", iterator.key());
            float balance = iterator.value();
            int _id;
            float consumed;

            while (balance >0) {
                if(!query2.exec()){
                    //qDebug()<<query2.lastError();
                    //qDebug()<<query2.lastQuery();
                }

                else{
                    if(query2.size()>0){
                        if(query2.first()){
                            _id = query2.value(0).toInt();
                            //                            consumed = query2.value(1).toFloat();

                            if(query2.value(1).toFloat() >= balance){
                                consumed = query2.value(1).toFloat() - balance;
                                balance =0; // update to consumed - balance
                            }
                            else {
                                consumed =0;
                                balance -= query2.value(1).toFloat();
                            }

                            //Update

                            //                            //qDebug()<<"Updating id,consumed, bal"<<_id<<consumed<<balance;

                            QMap<QString,QVariant> map;

                            QMap<QString,QVariant> map2;
                            map.insert(Transaction_Item_Helper_Consumed, consumed);
                            map2.insert(Transaction_Item_Helper_id , _id);

                            db->updateRecordsPlus(map,map2,Transaction_Item_Helper_TableName);
                        }
                    }
                    else{
                        balance =0;
                    }

                }
            }
        }
    }

}

float ItemTransactionDatabaseHelper::getConsumedQtyByVoucher(QString itemID, QString voucherNo, QString voucherType, QString voucherPrefix, QString ItemReqUuid)
{

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float consumed=0;
    QString qStr = " SELECT COALESCE(" + Transaction_Item_Helper_Consumed + ",0) FROM " + Transaction_Item_Helper_TableName;
    qStr += " WHERE " + Transaction_Item_Helper_VoucherID + " = '" + voucherNo + "'";
    qStr += " AND " + Transaction_Item_Helper_VoucherPrefix + " = '" + voucherPrefix + "'";
    qStr += " AND " + Transaction_Item_Helper_VoucherType + " = '" + voucherType + "'";
    qStr += " AND " + Transaction_Item_Helper_ItemID + " = '" + itemID + "'";
    qStr += " AND " + Transaction_Item_Helper_ItemReqId + " like '" + ItemReqUuid + "%'" ;

    query.prepare(qStr);
    if(!query.exec()){
        //qDebug()<<query.lastError();
        //qDebug()<<query.lastQuery();
    }
    else{
        if(query.first())
            consumed = query.value(0).toFloat();
    }

    return consumed;

}

int ItemTransactionDatabaseHelper::insertItemTransactionRecalc(ItemTransactionDataModel *obj, QList<GodownTransactionDataModel> godownList,
                                                               BulkLoadDatabaseHelper *db)
{
    qDebug()<<Q_FUNC_INFO;
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> map1;
    QMap<QString, QVariant> map2;

    qDebug()<<"godownlist size"<<godownList.size();
//    if( godownList.size()>0 ){
//        for(GodownTransactionDataModel a:godownList){
//            qDebug()<<"inserting entry for godown "<<a.fromGodown<<a.toGodown;
//            map1.insert(Transaction_Item_Helper_ItemID,obj->ItemID);
//            map1.insert(Transaction_Item_Helper_From_Ledger,obj->From_Ledger);
//            map1.insert(Transaction_Item_Helper_To_Ledger,obj->To_Ledger);
//            map1.insert(Transaction_Item_Helper_VoucherID,obj->VoucherID);
//            map1.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
//            map1.insert(Transaction_Item_Helper_VoucherDate,obj->VoucherDate);
//            map1.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
//            map1.insert(Transaction_Item_Helper_From_Location,a.fromGodown);
//            map1.insert(Transaction_Item_Helper_To_Location,a.toGodown);
//            map1.insert(Transaction_Item_Helper_Cr_Qty,a.crQty);
//            map1.insert(Transaction_Item_Helper_Dr_Qty,a.drQty);
//            map1.insert(Transaction_Item_Helper_Cr_Amount,a.crQty * obj->Price);
//            map1.insert(Transaction_Item_Helper_Dr_Amount,a.drQty * obj->Price);
//            map.insert(Transaction_Item_Helper_Price, obj->Price);
//            map1.insert(Transaction_Item_Helper_Narration,obj->Narration);
//            map1.insert(Transaction_Item_Helper_ItemReqId, obj->itemReqId);
//            map1.insert(Transaction_Item_Helper_Time,QDateTime::currentDateTime());
//            obj->_id = db->createInsertQuery(map1,Transaction_Item_Helper_TableName);

//            //            fifoHelper->insertFifoEntry(obj, a.fifoList);

//        }
//    }
//    else{
        qDebug()<<"inserting trans for "<<obj->VoucherID<<obj->VoucherPrefix<<obj->VoucherType;
        map.insert(Transaction_Item_Helper_ItemID,obj->ItemID);
        map.insert(Transaction_Item_Helper_From_Ledger,obj->From_Ledger);
        map.insert(Transaction_Item_Helper_To_Ledger,obj->To_Ledger);
        map.insert(Transaction_Item_Helper_VoucherID,obj->VoucherID);
        map.insert(Transaction_Item_Helper_VoucherPrefix,obj->VoucherPrefix);
        map.insert(Transaction_Item_Helper_VoucherDate,obj->VoucherDate);
        map.insert(Transaction_Item_Helper_VoucherType,obj->VoucherType);
        map.insert(Transaction_Item_Helper_From_Location,obj->From_Location);
        map.insert(Transaction_Item_Helper_To_Location,obj->To_Location);
        map.insert(Transaction_Item_Helper_Cr_Qty,obj->crQty);
        map.insert(Transaction_Item_Helper_Dr_Qty,obj->drQty);
        map.insert(Transaction_Item_Helper_Cr_Amount,obj->crAmount);
        map.insert(Transaction_Item_Helper_Dr_Amount,obj->drAmount);
        map.insert(Transaction_Item_Helper_Price, obj->Price);
        map.insert(Transaction_Item_Helper_Narration,obj->Narration);
        map.insert(Transaction_Item_Helper_ItemReqId, obj->itemReqId);
        map.insert(Transaction_Item_Helper_Time,QDateTime::currentDateTime());


        obj->_id = db->createInsertQuery(map,Transaction_Item_Helper_TableName);
//    }
}

//int ItemTransactionDatabaseHelper::updateConsumedQtyForVoucher(GeneralVoucherDataObject *voucher)
//{
//    //qDebug()<<Q_FUNC_INFO;
//    //create new list with distinct values
//    QList<ItemFifoDataModel*> voucherFifoList;

//    for(CompoundItemDataObject compItem:voucher->InventoryItems)
//    {
////        //qDebug()<<"item "<<compItem.BaseItem.ItemName<<compItem.BaseItem.fifoList.size();
//        for(ItemFifoDataModel *fifoObj: compItem.BaseItem.fifoList){
//            bool flag = true;
//            for(int i=0;i<voucherFifoList.size();i++){
//                if(voucherFifoList[i]->V0oucherID == fifoObj->VoucherID &&
//                        voucherFifoList[i]->VoucherType == fifoObj->VoucherType &&
//                        voucherFifoList[i]->VoucherPrefix == fifoObj->VoucherPrefix &&
//                        voucherFifoList[i]->ItemReqId == fifoObj->ItemReqId){
//                    // entry exists, break
//                    //qDebug()<<voucherFifoList[i]->ItemReqId<<"exists, not adding duplicate";
//                    voucherFifoList[i]->crQty += fifoObj->crQty;
//                    flag = false;
//                    break;
//                }r
//            }
//            if(flag){
//                //qDebug()<<"inserting ";
//                voucherFifoList.append(fifoObj);
//            }
//        }
//    }
//    //qDebug()<<"in voucher fifo size"<<voucherFifoList.size();

//    for(ItemFifoDataModel *fifoObj:voucherFifoList)    {
//        QString qStr = " SELECT SUM(" +Transaction_Item_Helper_Cr_Qty + ") FROM ";
//        qStr += Transaction_Item_Helper_TableName + " WHERE " ;
//        qStr += Transaction_Item_Helper_ConsumedReqId + " LIKE '" + fifoObj->ItemReqId + "'";
//        qStr += " AND NOT (" + Transaction_Item_Helper_VoucherID + " = '" + voucher->voucherNumber + "'";
//        qStr += " AND " + Transaction_Item_Helper_VoucherPrefix + " = '" + voucher->VoucherPrefix + "'";
//        qStr += " AND " + Transaction_Item_Helper_VoucherType + " = '" + voucher->voucherType + "')";

//        //qDebug()<<qStr;

//        float consumedQty = db->getSingletonQueryResult(qStr).toFloat() + fifoObj->crQty;

//        //qDebug()<<"updating cons qty to "<<consumedQty<<fifoObj->crQty;

//        QMap<QString, QVariant> map;
//        QMap<QString, QVariant> clause;
//        map.insert(Transaction_Item_Helper_Consumed, consumedQty );
//        clause.insert(Transaction_Item_Helper_VoucherID, fifoObj->VoucherID);
//        clause.insert(Transaction_Item_Helper_VoucherType, fifoObj->VoucherType);
//        clause.insert(Transaction_Item_Helper_VoucherPrefix, fifoObj->VoucherPrefix);
//        clause.insert(Transaction_Item_Helper_ItemReqId, fifoObj->ItemReqId);

//        db->updateRecordsPlus(map, clause, Transaction_Item_Helper_TableName);

//    }




//}
