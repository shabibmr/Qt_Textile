#include "transactionfifodatabasehelper.h"

TransactionFifoDatabaseHelper::TransactionFifoDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

int TransactionFifoDatabaseHelper::insertFifoEntry(ItemTransactionDataModel *obj, QList<ItemFifoDataModel *> fifoList)
{
    qDebug()<<Q_FUNC_INFO<<fifoList.size();
    QMap<QString,QVariant> map1;
    for(ItemFifoDataModel* a:fifoList){

        map1.insert(Transaction_Fifo_Helper_TransactionID, obj->_id);
        map1.insert(Transaction_Fifo_Helper_VoucherDate,obj->VoucherDate);
        map1.insert(Transaction_Fifo_Helper_ItemID, obj->ItemID);
        map1.insert(Transaction_Fifo_Helper_VoucherID,obj->VoucherID);
        map1.insert(Transaction_Fifo_Helper_VoucherPrefix,obj->VoucherPrefix);
        map1.insert(Transaction_Fifo_Helper_VoucherType,obj->VoucherType);
        map1.insert(Transaction_Fifo_Helper_ItemReqId, obj->itemReqId);
        map1.insert(Transaction_Fifo_Helper_ConsumedVoucherID,a->VoucherID);
        map1.insert(Transaction_Fifo_Helper_ConsumedVoucherPrefix,a->VoucherPrefix);
        map1.insert(Transaction_Fifo_Helper_ConsumedVoucherType,a->VoucherType);
        if(a->ItemReqId == "X")
            a->ItemReqId = obj->itemReqId;
        map1.insert(Transaction_Fifo_Helper_ConsumedReqId, a->ItemReqId);
        map1.insert(Transaction_Fifo_Helper_Cr_Qty,a->crQty);
        map1.insert(Transaction_Fifo_Helper_Dr_Qty,a->drQty);
        map1.insert(Transaction_Fifo_Helper_Cr_Amount, a->Price *a->crQty);
        map1.insert(Transaction_Fifo_Helper_Dr_Amount, a->Price *a->drQty);
        map1.insert(Transaction_Fifo_Helper_Price, a->Price);

        db->insertRecordsPlus(map1,Transaction_Fifo_Helper_TableName);

    }



}

int TransactionFifoDatabaseHelper::deleteFifoEntry(GeneralVoucherDataObject *obj)
{
    QMap<QString, QVariant> map;

    map.insert(Transaction_Fifo_Helper_VoucherID,obj->voucherNumber);
    map.insert(Transaction_Fifo_Helper_VoucherPrefix,obj->VoucherPrefix);
    map.insert(Transaction_Fifo_Helper_VoucherType,obj->voucherType);

    db->deleteRecordsPlus(map, Transaction_Fifo_Helper_TableName);


}

QSqlQueryModel *TransactionFifoDatabaseHelper::getItemQtyListFifo(GeneralVoucherDataObject *voucher, QString itemID, QDate date, QString godownID)
{
    QString qStrFifo = " SELECT fifo." + Transaction_Fifo_Helper_ConsumedVoucherID + ", ";
    qStrFifo += " fifo." + Transaction_Fifo_Helper_ConsumedVoucherType + ", " ;
    qStrFifo += " fifo." + Transaction_Fifo_Helper_ConsumedVoucherPrefix + ", ";
    qStrFifo += " fifo." + Transaction_Fifo_Helper_ItemID + ", " ;
    qStrFifo += " fifo." + Transaction_Fifo_Helper_ConsumedReqId + ", ";
    qStrFifo += " coalesce(sum(fifo." + Transaction_Fifo_Helper_Dr_Qty + " - fifo." + Transaction_Fifo_Helper_Cr_Qty + "), 0), ";
    qStrFifo += " fifo." + Transaction_Fifo_Helper_Price + " from " ;
    qStrFifo += Transaction_Fifo_Helper_TableName + " fifo JOIN Transaction_Item_Helper trans " ;
    qStrFifo += " ON fifo." + Transaction_Fifo_Helper_TransactionID + " = trans._id";
    qStrFifo += " WHERE fifo." + Transaction_Fifo_Helper_ItemID + " = '" + itemID + "'";
    qStrFifo += " AND fifo." + Transaction_Fifo_Helper_VoucherDate + " <= " + db->getDateString(date);
    qStrFifo += " AND  case when fifo." + Transaction_Fifo_Helper_Dr_Qty + " > 0 then  trans.to_location ";
    qStrFifo += " else trans.from_location end like '" + godownID + "'";
//    qStrFifo += " AND NOT (fifo." + Transaction_Fifo_Helper_VoucherID + " = '" +voucher->voucherNumber +"' ";
//    qStrFifo += " AND fifo." + Transaction_Fifo_Helper_VoucherType + " = '" +voucher->voucherType +"' ";
//    qStrFifo += " AND fifo." + Transaction_Fifo_Helper_VoucherPrefix + " = '" +voucher->VoucherPrefix +"') ";
    qStrFifo += " GROUP BY fifo." + Transaction_Fifo_Helper_ConsumedVoucherID + ", ";
    qStrFifo += " fifo." + Transaction_Fifo_Helper_ConsumedVoucherType + ", ";
    qStrFifo += " fifo." + Transaction_Fifo_Helper_ConsumedVoucherPrefix + ", ";
    qStrFifo += " fifo." + Transaction_Fifo_Helper_ConsumedReqId;
    qStrFifo += " having coalesce(sum(fifo." + Transaction_Fifo_Helper_Dr_Qty +
            " - fifo." + Transaction_Fifo_Helper_Cr_Qty + "), 0) >0 ";
    qStrFifo += " ORDER BY fifo." +Transaction_Fifo_Helper_VoucherDate + ", trans.time" ;

    qDebug()<<qStrFifo;

    return db->getQueryModel(qStrFifo);
}

QList<ItemFifoDataModel *> TransactionFifoDatabaseHelper::getFifoListByTransId(int transId)
{
    qDebug()<<Q_FUNC_INFO;
    QList<ItemFifoDataModel *> fifoList;

    QString qStr = " SELECT fifo." + Transaction_Fifo_Helper_ConsumedVoucherID + ", ";
    qStr += " fifo." + Transaction_Fifo_Helper_ConsumedVoucherType + ", " ;
    qStr += " fifo." + Transaction_Fifo_Helper_ConsumedVoucherPrefix + ", ";
    qStr += " fifo." + Transaction_Fifo_Helper_ItemID + ", " ;
    qStr += " fifo." + Transaction_Fifo_Helper_ConsumedReqId + ", ";
    qStr += " fifo." + Transaction_Fifo_Helper_Dr_Qty + " , " ;
    qStr += " fifo." + Transaction_Fifo_Helper_Cr_Qty + ", ";
    qStr += " fifo." + Transaction_Fifo_Helper_Price + " from " ;
    qStr += Transaction_Fifo_Helper_TableName + " fifo ";
    qStr += " WHERE " + Transaction_Fifo_Helper_TransactionID + " = " + QString::number(transId);

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while (query.next()) {

            int i=0;
            ItemFifoDataModel *fifoObj = new ItemFifoDataModel;

            fifoObj->VoucherID = query.value(i++).toString();
            fifoObj->VoucherType = query.value(i++).toString();
            fifoObj->VoucherPrefix = query.value(i++).toString();
            fifoObj->ItemID = query.value(i++).toString();
            fifoObj->ItemReqId = query.value(i++).toString();
            fifoObj->drQty = query.value(i++).toFloat();
            fifoObj->crQty = query.value(i++).toFloat();
            fifoObj->Price = query.value(i++).toFloat();

            fifoList.append(fifoObj);
        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return fifoList;
}
