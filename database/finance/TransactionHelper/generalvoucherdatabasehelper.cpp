#include "generalvoucherdatabasehelper.h"

GeneralVoucherDatabaseHelper::GeneralVoucherDatabaseHelper()
{
    db = new DatabaseHelper();
}

QString GeneralVoucherDatabaseHelper::Voucher_No_Seq_Tablename = "Voucher_No_Seq";
QString GeneralVoucherDatabaseHelper::Voucher_No_Seq_VoucherType = "Voucher_Type";
QString GeneralVoucherDatabaseHelper::Voucher_No_Seq_VoucherPrefix = "Voucher_Prefix";
QString GeneralVoucherDatabaseHelper::Voucher_No_Seq_VoucherNo = "Voucher_No";
QString GeneralVoucherDatabaseHelper::Voucher_No_seq_Voucher_Date = "Voucher_date";
QString GeneralVoucherDatabaseHelper::Transaction_Id = "TransactionId";
QString GeneralVoucherDatabaseHelper::Requirement_Voucher_No = "Requirement_Voucher_No";


QString GeneralVoucherDatabaseHelper::getNextVoucherNoByType(QString voucherType, QString voucherPrefix, QString tableName)
{

    QString voucherNo;

    QString vNo = "";

    if( voucherType.length() == 0 || voucherPrefix.length() == 0 || tableName.length() == 0){
        qDebug()<<" INCOMPLETE ARGUMENTS AT GENERATE VOUCHER SEQ. VOUCHER TYPE : "<<voucherType;
        return "X";
    }

    QString qStr = " SELECT " + Voucher_No_Seq_VoucherNo + " + 1 FROM " + Voucher_No_Seq_Tablename;
    qStr += " WHERE " + Voucher_No_Seq_VoucherPrefix + " = '" + voucherPrefix + "' ";
    qStr += " AND " + Voucher_No_Seq_VoucherType + " = '" + voucherType + "' ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query3(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query1(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{

        if(query.first()){
            voucherNo = query.value(0).toString();

        }

        if(query.size() == 0){
            QString qStr2 = " SELECT coalesce(max(cast(voucher_no as int)),0) FROM " + tableName +
                    " Where "+ Voucher_No_Seq_VoucherPrefix + " = '" + voucherPrefix + "'";

            query2.prepare(qStr2);

            if(query2.exec()){
                if(query2.first()){
                    vNo = query2.value(0).toString();
                }

                QString qStr1 = " INSERT INTO " + Voucher_No_Seq_Tablename + "(" ;
                qStr1 += Voucher_No_Seq_VoucherNo + ", " + Voucher_No_Seq_VoucherPrefix + ", " + Voucher_No_Seq_VoucherType;
                qStr1 += ") values ('" + vNo +"', '" +  voucherPrefix +"', '"+voucherType +"')";

                query1.prepare(qStr1);

                if(!query1.exec()){
                    qDebug()<<query1.lastQuery()<<" ERROR IN QUERY1 ";
                    qDebug()<<query1.lastError();
                }
                else{
                    query.prepare(qStr);
                    if(!query3.exec()){
                        qDebug()<<query3.lastQuery();
                        qDebug()<<query3.lastError();
                    }
                    else{
                        if(query3.first()){
                            voucherNo = query.value(0).toString();
                        }
                    }
                }

            }

            else{
                qDebug()<<query2.lastQuery()<<" ERROR IN QUERY2 ";
                qDebug()<<query2.lastError();
            }

        }
    }


    return voucherNo;
}

QPair<QString, QString> GeneralVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId, QString tableName)
{
    QPair<QString,QString> pair;
    QString voucherNo;
    QString vNo = "";


    QString qStr = " SELECT " + Voucher_No_Seq_VoucherNo + ", " + Voucher_No_Seq_VoucherPrefix ;
    qStr += "  FROM " + tableName;
    qStr += " WHERE " + Transaction_Id + " = '" + transId + "' ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{
        if(query.first()){
            pair.first = query.value(0).toString();
            pair.second = query.value(1).toString();
        }

        else{
            pair.first = "-1";
            pair.second = "-1";
        }

    }

    return pair;
}


QPair<QString, QString> GeneralVoucherDatabaseHelper::getVoucherNoByReqVoucherNo(QString transId, QString tableName)
{
    QPair<QString,QString> pair;
    QString voucherNo;
    QString vNo = "";


    QString qStr = " SELECT " + Voucher_No_Seq_VoucherNo + ", " + Voucher_No_Seq_VoucherPrefix ;
    qStr += "  FROM " + tableName;
    qStr += " WHERE " + Requirement_Voucher_No + " = '" + transId + "' ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{
        if(query.first()){
            pair.first = query.value(0).toString();
            pair.second = query.value(1).toString();
        }

        else{
            pair.first = "-1";
            pair.second = "-1";
        }

    }

    return pair;
}


QString GeneralVoucherDatabaseHelper::setNextVoucherNoByType(QString voucherType, QString voucherPrefix, QString tableName)
{
    qDebug()<<Q_FUNC_INFO;

    QString voucherNo;
    QString vNo;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery queryCheck(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query1(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query3(QSqlDatabase::database(DatabaseValues::connectionString));

    QString checkProc = "show PROCEDURE status where Name = 'UpdateVoucherNo' and Db = '" + DatabaseValues::dbName + "'";
    query.exec(checkProc);
    if(query.size() == 0){
        qDebug()<<"PROCEDURE DOESNOT EXIST************************";
        DatabaseHelper *db = new DatabaseHelper();
        if(db->createVoucherNoProcedure())
        {}

    }

    QString qStr = " CALL UpdateVoucherNo('" + voucherType + "', '" + voucherPrefix + "', " + "@voucherNo" + ")";
    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;
    QString qStr2 = " SELECT coalesce(max(cast(voucher_no as int)),0) FROM " + tableName +
            " Where "+ Voucher_No_Seq_VoucherPrefix + " = '" + voucherPrefix + "'";

    QString qStrCheck = " SELECT count(*) FROM " + Voucher_No_Seq_Tablename;
    qStrCheck += " WHERE " + Voucher_No_Seq_VoucherPrefix + " = '" + voucherPrefix + "' ";
    qStrCheck += " AND " + Voucher_No_Seq_VoucherType + " = '" + voucherType + "' ";



    //    qDebug()<<qStr;

    queryCheck.prepare(qStrCheck);
    if(!queryCheck.exec()){
        qDebug()<<queryCheck.lastQuery();
        qDebug()<<queryCheck.lastError();
    }
    else{
        if(queryCheck.first()){

            if(queryCheck.value(0).toInt() == 0){

                query2.prepare(qStr2);
                if(query2.exec()){

                    if(query2.first()){
                        vNo = query2.value(0).toString();
                        QString qStrInsert = " INSERT INTO " + Voucher_No_Seq_Tablename + "(" ;
                        qStrInsert += Voucher_No_Seq_VoucherNo + ", " + Voucher_No_Seq_VoucherPrefix + ", " + Voucher_No_Seq_VoucherType;
                        qStrInsert += ") values ('" + vNo +"', '" +  voucherPrefix +"', '"+voucherType +"')";

                        query1.prepare(qStrInsert);

                        if(!query1.exec()){
                            qDebug()<<query1.lastQuery()<<" ERROR IN QUERY1  ";
                            qDebug()<<query1.lastError();
                        }
                    }
                }
                else{
                    qDebug()<<query2.lastQuery()<<" ERROR IN QUERY2 ";
                    qDebug()<<query2.lastError();
                }
            }
        }
    }
    query.prepare(qStr);
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{
        query3.prepare("SELECT @voucherNo");
        if(query3.exec()){
            while(query3.next()){
                voucherNo = query3.value(0).toString();
            }

        }
        else{
            qDebug()<<query3.lastError();
            qDebug()<<query3.lastQuery();
        }
    }
    return voucherNo;
}

int GeneralVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int GeneralVoucherDatabaseHelper::commit()
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
    return 0;

}

int GeneralVoucherDatabaseHelper::getMaxID()
{

}



GeneralVoucherDataObject *GeneralVoucherDatabaseHelper::getVoucherToExportByItem(int sourceVoucherType, int targetVoucherType, QString ItemReqId)
{
    GeneralVoucherDataObject *voucher = new GeneralVoucherDataObject();
    CompoundItemDataObject compItem;
    inventoryItemDataModel item;

    //    float srcQty=0, desQty=0;
    QString ItemID;
    QString qStr = "";

    qStr += " SELECT (";
    qStr += " SELECT " ;
    qStr +=  " SUM(" + General_Voucher_Quantity + ")";
    qStr += " FROM " + GMVoucherTypes::getVoucherDetailsTablename(sourceVoucherType) ;
    qStr += " WHERE " + General_Voucher_ItemReqId + " = '" + ItemReqId + "'";
    //    qStr += " GROUP BY " + Transaction_Item_Helper_VoucherType;
    qStr += ") - (";
    qStr += " SELECT "  ;
    qStr +=  " SUM(" + General_Voucher_Quantity + ")";
    qStr += " FROM " + GMVoucherTypes::getVoucherDetailsTablename(targetVoucherType) ;
    qStr += " WHERE " + General_Voucher_ItemReqId + " = '" + ItemReqId + "'";

    //    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while(query.next()){
            item.maxQuantity = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError()<<" in query ";
        qDebug()<<query.lastQuery();
    }

    //    item = shelper.getInventoryItemByID(ItemID);
    //    item.maxQuantity = srcQty - desQty;
    compItem.BaseItem = item;
    voucher->InventoryItems.append(compItem);

    return voucher;
}

float GeneralVoucherDatabaseHelper::getPendingQtyToExportByItem(int sourceVoucherType, int targetVoucherType, QString ItemReqId)
{
    float pendingQty=0;
    QString qStr = " SELECT (";
    qStr += " SELECT " ;
    qStr +=  " SUM(" + General_Voucher_Quantity + ")";
    qStr += " FROM " + GMVoucherTypes::getVoucherDetailsTablename(sourceVoucherType) ;
    qStr += " WHERE " + General_Voucher_ItemReqId + " = '" + ItemReqId + "'";
    //    qStr += " GROUP BY " + Transaction_Item_Helper_VoucherType;
    qStr += ") - (";
    qStr += " SELECT "  ;
    qStr +=  " SUM(" + General_Voucher_Quantity + ")";
    qStr += " FROM " + GMVoucherTypes::getVoucherDetailsTablename(targetVoucherType) ;
    qStr += " WHERE " + General_Voucher_ItemReqId + " = '" + ItemReqId + "')";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while(query.next()){
            pendingQty = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError()<<" in query ";
        qDebug()<<query.lastQuery();
    }

    return pendingQty;
}

QList<GeneralVoucherDataObject *> GeneralVoucherDatabaseHelper::getVoucherListToexportByType(int sourceVoucherType, int targetVoucherType,  QMap<QString,QVariant> filterMap)
{
    qDebug()<<Q_FUNC_INFO;
    QList<GeneralVoucherDataObject *> voucherList;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();

    QString section = nullptr;
    QString ledgerId = nullptr;

    QMapIterator<QString, QVariant> i(filterMap);

    while(i.hasNext()){
        i.next();

        if(i.key() == "section")
            section = i.value().toString();
        if(i.key() == "ledger")
            ledgerId = i.value().toString();


    }

    QString qStr = "";


    qStr += " SELECT src." + Voucher_No_Seq_VoucherNo + ", src." + Voucher_No_Seq_VoucherPrefix ;
    qStr += " , src." + General_Voucher_ItemID + " as ItemID, src." + General_Voucher_ItemReqId + ", ";
    qStr += " ROUND(COALESCE(SUM(src." + General_Voucher_Quantity + "),0),3) - ROUND(COALESCE(SUM(tgt." + General_Voucher_Quantity + "),0),3) as MaxQty, ";
    qStr += " ( SELECT " + General_Voucher_LedgerId + " FROM " + GMVoucherTypes::getVoucherMainTablename(sourceVoucherType);
    qStr += " WHERE " + Voucher_No_Seq_VoucherNo + " = src." + Voucher_No_Seq_VoucherNo ;
    qStr += " AND " + Voucher_No_Seq_VoucherPrefix + " = src." + Voucher_No_Seq_VoucherPrefix + ") as Ledger FROM ";
    qStr += GMVoucherTypes::getVoucherDetailsTablename(sourceVoucherType) + " src LEFT OUTER JOIN ";
    qStr += GMVoucherTypes::getVoucherDetailsTablename(targetVoucherType) + " tgt ";
    qStr += " ON src." +  General_Voucher_ItemReqId + " = tgt." + General_Voucher_ItemReqId;
    qStr += " WHERE 1=1 ";
    if(targetVoucherType == GMVoucherTypes::WorkOrder)
       qStr += " AND cast(src." + General_Voucher_itemProductionStatus + " as int) < " + QString::number(QuotationStatus::WorkOrderRaised);
    else if(targetVoucherType == GMVoucherTypes::DeliveryNote)
        qStr += " AND cast(src." + General_Voucher_itemProductionStatus + " as int) < " + QString::number(QuotationStatus::DeliveryReady);
    else if(targetVoucherType == GMVoucherTypes::SalesVoucher)
        qStr += " AND cast(src." + General_Voucher_itemProductionStatus + " as int) < " + QString::number(QuotationStatus::SalesInvoiceRaised);
    if(section != nullptr){
        qStr += " AND src." + General_Voucher_ItemID + " IN ( SELECT " + Sales_Inventory_Item_ItemId + " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name;
        qStr += " WHERE " + General_Voucher_Section + " = '" + section + "') ";
    }

    if(ledgerId != nullptr){
        qStr += " AND src." + Voucher_No_Seq_VoucherNo + " IN ( SELECT " + Voucher_No_Seq_VoucherNo + " FROM " + GMVoucherTypes::getVoucherMainTablename(sourceVoucherType);
        qStr += " WHERE " + General_Voucher_LedgerId + " = '" + ledgerId + "') ";
    }
    qStr += " GROUP BY src." + General_Voucher_ItemReqId ;
    qStr += " HAVING ROUND(COALESCE(SUM(src." + General_Voucher_Quantity + "),0),3) - ROUND(COALESCE(SUM(tgt." + General_Voucher_Quantity + "),0),3) > 0";

    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while(query.next()){
            GeneralVoucherDataObject *voucher = new GeneralVoucherDataObject();
            voucher->voucherNumber = query.value(0).toString();
            voucher->VoucherPrefix = query.value(1).toString();
            voucher->voucherType = GMVoucherTypes::getVoucherString(sourceVoucherType);
            voucher->ledgerObject = ledHelper->getLedgerObjectByID(query.value(5).toString());

            //            query.next();
            while(voucher->voucherNumber == query.value(0).toString() && voucher->VoucherPrefix == query.value(1).toString())
            {
                CompoundItemDataObject compItem;
                compItem.BaseItem = itemHelper->getInventoryItemByID(query.value(2).toString());
                compItem.BaseItem.ItemReqUuid = query.value(3).toString();
                compItem.BaseItem.quantity = query.value(4).toFloat();
                compItem.BaseItem.maxQuantity = query.value(4).toFloat();

                if(targetVoucherType == GMVoucherTypes::WorkOrder)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;
                else if(targetVoucherType == GMVoucherTypes::DeliveryNote)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::DeliveryReady;
                else if(targetVoucherType == GMVoucherTypes::SalesVoucher)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::SalesInvoiceRaised;
                else if(targetVoucherType == GMVoucherTypes::ReceiptNote)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::ReceiptReady;
                else if(targetVoucherType == GMVoucherTypes::PurchaseVoucher)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::PurchaseInvoiceRaised;
                if( targetVoucherType == GMVoucherTypes::DeliveryNote || targetVoucherType == GMVoucherTypes::SalesVoucher
                        || targetVoucherType == GMVoucherTypes::ReceiptNote || targetVoucherType == GMVoucherTypes::PurchaseVoucher){
                    if( compItem.BaseItem.ClosingStock >= compItem.BaseItem.maxQuantity  )
                        voucher->InventoryItems.append(compItem);
                }
                else{
                    voucher->InventoryItems.append(compItem);
                }
                if(!query.next())
                    break;
            }
            qDebug()<<"query size - show voucher"<<query.record().count();
            qDebug()<<"Item size"<<voucher->InventoryItems.size();
            query.previous();
            if(voucher->InventoryItems.size() > 0)
            voucherList.append(voucher);
        }
    }

    else{
        qDebug()<<query.lastError()<<" in query ";
        qDebug()<<query.lastQuery();
    }

    return voucherList;
}

QList<GeneralVoucherDataObject *> GeneralVoucherDatabaseHelper::getVoucherListToexportByDate(QDateTime fromDate,QDateTime toDate,int sourceVoucherType, int targetVoucherType, QMap<QString, QVariant> filterMap)
{
    qDebug()<<Q_FUNC_INFO;
    QList<GeneralVoucherDataObject *> voucherList;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();

    QString section = nullptr;
    QString ledgerId = nullptr;

    QMapIterator<QString, QVariant> i(filterMap);

    while(i.hasNext()){
        i.next();

        if(i.key() == "section")
            section = i.value().toString();
        if(i.key() == "ledger")
            ledgerId = i.value().toString();


    }

    QString qStr = "";


    qStr += " SELECT src." + Voucher_No_Seq_VoucherNo + ", src." + Voucher_No_Seq_VoucherPrefix ;
    qStr += " , src." + General_Voucher_ItemID + " as ItemID, src." + General_Voucher_ItemReqId + ", ";
    qStr += " ROUND(COALESCE(SUM(src." + General_Voucher_Quantity + "),0),3) - ROUND(COALESCE(SUM(tgt." + General_Voucher_Quantity + "),0),3) as MaxQty, ";
    qStr += " ( SELECT " + General_Voucher_LedgerId + " FROM " + GMVoucherTypes::getVoucherMainTablename(sourceVoucherType);
    qStr += " WHERE " + Voucher_No_Seq_VoucherNo + " = src." + Voucher_No_Seq_VoucherNo ;
    qStr += " AND " + Voucher_No_Seq_VoucherPrefix + " = src." + Voucher_No_Seq_VoucherPrefix + ") as Ledger FROM ";
    qStr += GMVoucherTypes::getVoucherDetailsTablename(sourceVoucherType) + " src LEFT OUTER JOIN ";
    qStr += GMVoucherTypes::getVoucherDetailsTablename(targetVoucherType) + " tgt ";
    qStr += " ON src." +  General_Voucher_ItemReqId + " = tgt." + General_Voucher_ItemReqId;
    qStr += " WHERE 1=1 ";
    if(targetVoucherType == GMVoucherTypes::WorkOrder)
       qStr += " AND cast(src." + General_Voucher_itemProductionStatus + " as int) < " + QString::number(QuotationStatus::WorkOrderRaised);
    else if(targetVoucherType == GMVoucherTypes::DeliveryNote)
        qStr += " AND cast(src." + General_Voucher_itemProductionStatus + " as int) < " + QString::number(QuotationStatus::DeliveryReady);
    else if(targetVoucherType == GMVoucherTypes::SalesVoucher)
        qStr += " AND cast(src." + General_Voucher_itemProductionStatus + " as int) < " + QString::number(QuotationStatus::SalesInvoiceRaised);
    if(section != nullptr){
        qStr += " AND src." + General_Voucher_ItemID + " IN ( SELECT " + Sales_Inventory_Item_ItemId + " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name;
        qStr += " WHERE " + General_Voucher_Section + " = '" + section + "') ";
    }

    if(ledgerId != nullptr){
        qStr += " AND src." + Voucher_No_Seq_VoucherNo + " IN ( SELECT " + Voucher_No_Seq_VoucherNo + " FROM " + GMVoucherTypes::getVoucherMainTablename(sourceVoucherType);
        qStr += " WHERE " + General_Voucher_LedgerId + " = '" + ledgerId + "') ";
    }

    qStr += " AND " + Voucher_No_seq_Voucher_Date + " <= " + db->getDateTimeString(toDate);
    qStr += " AND ";
    qStr += Voucher_No_seq_Voucher_Date + " >= " + db->getDateTimeString(fromDate);
    qStr += " GROUP BY src." + General_Voucher_ItemReqId ;
    qStr += " HAVING ROUND(COALESCE(SUM(src." + General_Voucher_Quantity + "),0),3) - ROUND(COALESCE(SUM(tgt." + General_Voucher_Quantity + "),0),3) > 0";

    qDebug()<<Q_FUNC_INFO<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while(query.next()){
            GeneralVoucherDataObject *voucher = new GeneralVoucherDataObject();
            voucher->voucherNumber = query.value(0).toString();
            voucher->VoucherPrefix = query.value(1).toString();
            voucher->voucherType = GMVoucherTypes::getVoucherString(sourceVoucherType);
            voucher->ledgerObject = ledHelper->getLedgerObjectByID(query.value(5).toString());

            //            query.next();
            while(voucher->voucherNumber == query.value(0).toString() && voucher->VoucherPrefix == query.value(1).toString())
            {
                CompoundItemDataObject compItem;
                compItem.BaseItem = itemHelper->getInventoryItemByID(query.value(2).toString());
                compItem.BaseItem.ItemReqUuid = query.value(3).toString();
                compItem.BaseItem.quantity = query.value(4).toFloat();
                compItem.BaseItem.maxQuantity = query.value(4).toFloat();

                if(targetVoucherType == GMVoucherTypes::WorkOrder)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;
                else if(targetVoucherType == GMVoucherTypes::DeliveryNote)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::DeliveryReady;
                else if(targetVoucherType == GMVoucherTypes::SalesVoucher)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::SalesInvoiceRaised;
                else if(targetVoucherType == GMVoucherTypes::ReceiptNote)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::ReceiptReady;
                else if(targetVoucherType == GMVoucherTypes::PurchaseVoucher)
                    compItem.BaseItem.itemProductionStatus = QuotationStatus::PurchaseInvoiceRaised;
                if( targetVoucherType == GMVoucherTypes::DeliveryNote || targetVoucherType == GMVoucherTypes::SalesVoucher
                        || targetVoucherType == GMVoucherTypes::ReceiptNote || targetVoucherType == GMVoucherTypes::PurchaseVoucher){
                    if( compItem.BaseItem.ClosingStock >= compItem.BaseItem.maxQuantity  )
                        voucher->InventoryItems.append(compItem);
                }
                else{
                    voucher->InventoryItems.append(compItem);
                }
                if(!query.next())
                    break;
            }
            qDebug()<<"query size - show voucher"<<query.record().count();
            qDebug()<<"Item size"<<voucher->InventoryItems.size();
            query.previous();
            if(voucher->InventoryItems.size() > 0)
            voucherList.append(voucher);
        }
    }

    else{
        qDebug()<<query.lastError()<<" in query ";
        qDebug()<<query.lastQuery();
    }

    return voucherList;
}


QMap<QString, QString> GeneralVoucherDatabaseHelper::getPendingSectionToExportByType(int sourceVoucherType, int targetVoucherType)
{
    QMap<QString, QString> sections;

    QString qStr = "SELECT distinct " + General_Voucher_Section + ", ";
    qStr += " (SELECT Godown_Name from " +GodownDatabaseHelper::Godown_Tablename +" Where Godown_Id = " + General_Voucher_Section + ") FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name;
    qStr += " WHERE " + Sales_Inventory_Item_ItemId + " IN (";
    qStr += " SELECT distinct src." + General_Voucher_ItemID + " as ItemID FROM ";
    qStr += GMVoucherTypes::getVoucherDetailsTablename(sourceVoucherType) + " src LEFT OUTER JOIN ";
    qStr += GMVoucherTypes::getVoucherDetailsTablename(targetVoucherType) + " tgt ";
    qStr += " ON src." +  General_Voucher_ItemReqId + " = tgt." + General_Voucher_ItemReqId;
    qStr += " WHERE src." + General_Voucher_itemProductionStatus + " < " + QString::number(QuotationStatus::WorkOrderRaised);
    qStr += " GROUP BY src." + General_Voucher_ItemReqId ;
    qStr += " HAVING ROUND(COALESCE(SUM(src." + General_Voucher_Quantity + "),0),3) - ROUND(COALESCE(SUM(tgt." + General_Voucher_Quantity + "),0),3) > 0) ";

    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec())    {
        while(query.next()){
            sections.insert(query.value(0).toString(), query.value(1).toString());
        }
    }

    else{
        qDebug()<<query.lastError()<<" in query ";
        qDebug()<<query.lastQuery();
    }

    return sections;

}
