#include "transactionhelper.h"

TransactionHelper::TransactionHelper(DatabaseHelper *db)
{
    this->db = db;
}

int TransactionHelper::startTransaction()
{
    db->startTransaction();
}

int TransactionHelper::insertTransaction(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO;
    QMap<QString,QVariant> map;
    LedgerMasterDatabaseHelper* ledHelper = new LedgerMasterDatabaseHelper;
    AccountGroupMasterDatabaseHelper* accHelper = new AccountGroupMasterDatabaseHelper;

    for(LedgerMasterDataModel ledger:voucher->ledgersList){
//        qDebug()<<"Credit : "<<ledger.crAmount;
        qDebug()<<"insert ledger entry"<<ledger.LedgerName;

        map.insert(Transaction_Helper_VoucherType,voucher->voucherType);
        map.insert(Transaction_Helper_VoucherPrefix,voucher->VoucherPrefix);
        map.insert(Transaction_Helper_VoucherNo,voucher->voucherNumber);
        map.insert(Transaction_Helper_VoucherDate,voucher->VoucherDate);
        map.insert(Transaction_Helper_ProjectID,voucher->ProjectId);
        map.insert(Transaction_Helper_Narration,voucher->narration);
        map.insert(Transaction_Helper_SalesManID,voucher->SalesmanID);
        map.insert(Transaction_Helper_isAdditional,!(ledger.isInvoiceItem));
        map.insert(Transaction_Helper_TransactionAmount,ledger.amount);
        map.insert(Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        map.insert(Transaction_Helper_DebitLedgerID,ledger.AgainstLedger);
        map.insert(Transaction_Helper_Debit_Amount,ledger.drAmount);
        map.insert(Transaction_Helper_Credit_Amount,ledger.crAmount);
        map.insert(Transaction_Helper_LedgerID,ledger.LedgerID);
        map.insert(Transaction_Helper_AgainstLedger,ledger.AgainstLedger);
        map.insert(Transaction_Helper_TimeStamp, voucher->timestamp);

        db->insertRecordsPlus(map,Transaction_Helper_TableName);

        LedgerMasterDataModel ledger1 = ledHelper->getLedgerObjectByID(ledger.LedgerID);
        qDebug()<<"ledger group for billwise"<<ledger1.LedgerGroupId<<ledHelper->getLedgerIDByName("Sundry Debtors")<<ledHelper->getLedgerIDByName("Sundry Creditors");
        if( ledger1.LedgerGroupId == accHelper->getGroupIDByName("Sundry Debtors")
                || ledger1.LedgerGroupId == accHelper->getGroupIDByName("Sundry Creditors") ){

            qDebug()<<"Insert billwise";

            BillwiseRecordsDatabaseHelper* billHelper = new BillwiseRecordsDatabaseHelper;
            billHelper->deleteBillwiseRecordsByVoucher(voucher->voucherNumber, voucher->voucherType, voucher->VoucherPrefix);
            if(ledger.mapList.size() > 0)
            {
                for(BillwiseRecordDataModel* a:ledger.mapList){
                    if(a->VoucherNo.length()==0 && a->VoucherType == "OPENING")
                        a->VoucherNo = "OPENING";
                    else{
                        a->VoucherNo=voucher->voucherNumber;
                        a->VoucherDate=voucher->VoucherDate;
                        a->VoucherType=voucher->voucherType;
                        a->VoucherPrefix=voucher->VoucherPrefix;

                    }

                    if(a->RefVoucherNo.length()==0){
                        a->RefVoucherNo=voucher->voucherNumber;
                        a->RefType=voucher->voucherType;
                        a->RefPrefix=voucher->VoucherPrefix;

                    }
                    billHelper->insertBillwiseRecords(a);
                }
            }

        }


    }
    qDebug()<<"check if cheque exists";

    if( ( GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::PaymentVoucher)
          || GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::ReceiptVoucher))
            && voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts")
            && voucher->chequeEntry != nullptr){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"inserting cheque";
        ChequeDetailsDatabaseHelper* chqHelper = new ChequeDetailsDatabaseHelper();
        chqHelper->deleteChequeDetailsByVoucher(voucher->chequeEntry);
        chqHelper->insertChequeDetails(voucher->chequeEntry);
    }

}

int TransactionHelper::updateTransaction(GeneralVoucherDataObject *voucher, QString oldID)
{

//    this->startTransaction();
    QMap<QString,QVariant> map;
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper;
    AccountGroupMasterDatabaseHelper* accHelper = new AccountGroupMasterDatabaseHelper;

    map.insert(Transaction_Helper_VoucherType,voucher->voucherType);
    map.insert(Transaction_Helper_VoucherPrefix,voucher->VoucherPrefix);
    map.insert(Transaction_Helper_VoucherNo,voucher->voucherNumber);
    map.insert(Transaction_Helper_VoucherDate,voucher->VoucherDate);
    map.insert(Transaction_Helper_ProjectID,voucher->ProjectId);
    map.insert(Transaction_Helper_TransactionAmount,voucher->ledgerObject.amount);
    map.insert(Transaction_Helper_CreditLedgerID,voucher->ledgerObject.LedgerID);
    map.insert(Transaction_Helper_DebitLedgerID,voucher->ledgerObject.LedgerID);
    map.insert(Transaction_Helper_Debit_Amount,voucher->ledgerObject.drAmount);
    map.insert(Transaction_Helper_Credit_Amount,voucher->ledgerObject.crAmount);
    map.insert(Transaction_Helper_LedgerID,voucher->ledgerObject.LedgerID);
    map.insert(Transaction_Helper_Narration,voucher->narration);
    map.insert(Transaction_Helper_SalesManID,voucher->SalesmanID);
    map.insert(Transaction_Helper_TimeStamp,voucher->timestamp);

    QMap<QString,QVariant> clause;

    clause.insert(Transaction_Helper_id,oldID);
    db->updateRecordsPlus(map,clause,Transaction_Helper_TableName);

    LedgerMasterDataModel ledger1 = ledHelper->getLedgerObjectByID(voucher->ledgerObject.LedgerID);
    if(ledger1.LedgerGroupId == "0x2x15" || ledger1.LedgerGroupId == "0x5x19"){
        BillwiseRecordsDatabaseHelper* billHelper = new BillwiseRecordsDatabaseHelper;
        billHelper->deleteBillwiseRecordsByVoucher(voucher->voucherNumber, voucher->voucherType, voucher->VoucherPrefix);
        for(BillwiseRecordDataModel* a:voucher->ledgerObject.mapList){
            billHelper->insertBillwiseRecords(a);
        }

    }

    if(voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts") && voucher->chequeEntry != nullptr){
        qDebug()<<Q_FUNC_INFO;
        ChequeDetailsDatabaseHelper* chqHelper = new ChequeDetailsDatabaseHelper();
        chqHelper->deleteChequeDetailsByVoucher(voucher->chequeEntry);
        chqHelper->insertChequeDetails(voucher->chequeEntry);
    }

}

int TransactionHelper::deleteTransaction(GeneralVoucherDataObject *obj)
{

    QMap<QString,QVariant> map;

    map.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
    map.insert(Transaction_Helper_VoucherType,obj->voucherType);
    map.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);

    db->deleteRecordsPlus(map,Transaction_Helper_TableName);

    BillwiseRecordsDatabaseHelper* billHelper = new BillwiseRecordsDatabaseHelper;
    billHelper->deleteBillwiseRecordsByVoucher(obj->voucherNumber, obj->voucherType, obj->VoucherPrefix);

}

int TransactionHelper::commit()
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


int TransactionHelper::insertProxyTransaction(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO;
    QMap<QString,QVariant> map;
    LedgerMasterDatabaseHelper* ledHelper = new LedgerMasterDatabaseHelper;
    AccountGroupMasterDatabaseHelper* accHelper = new AccountGroupMasterDatabaseHelper;

    for(LedgerMasterDataModel ledger:voucher->ledgersList){
//        qDebug()<<"Credit : "<<ledger.crAmount;
        qDebug()<<"insert ledger entry"<<ledger.LedgerName;

        map.insert(Transaction_Helper_VoucherType,voucher->voucherType);
        map.insert(Transaction_Helper_VoucherPrefix,voucher->VoucherPrefix);
        map.insert(Transaction_Helper_VoucherNo,voucher->voucherNumber);
        map.insert(Transaction_Helper_VoucherDate,voucher->VoucherDate);
        map.insert(Transaction_Helper_ProjectID,voucher->ProjectId);
        map.insert(Transaction_Helper_Narration,voucher->narration);
        map.insert(Transaction_Helper_SalesManID,voucher->SalesmanID);
        map.insert(Transaction_Helper_isAdditional,!(ledger.isInvoiceItem));
        map.insert(Transaction_Helper_TransactionAmount,ledger.amount);
        map.insert(Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        map.insert(Transaction_Helper_DebitLedgerID,ledger.AgainstLedger);
        map.insert(Transaction_Helper_Debit_Amount,ledger.drAmount);
        map.insert(Transaction_Helper_Credit_Amount,ledger.crAmount);
        map.insert(Transaction_Helper_LedgerID,ledger.LedgerID);
        map.insert(Transaction_Helper_AgainstLedger,ledger.AgainstLedger);

        db->insertRecordsPlus(map,Proxy_Transaction_Helper_TableName);

//        LedgerMasterDataModel ledger1 = ledHelper->getLedgerObjectByID(ledger.LedgerID);
//        qDebug()<<"ledger group for billwise"<<ledger1.LedgerGroupId<<ledHelper->getLedgerIDByName("Sundry Debtors")<<ledHelper->getLedgerIDByName("Sundry Creditors");
//        if( ledger1.LedgerGroupId == accHelper->getGroupIDByName("Sundry Debtors")
//                || ledger1.LedgerGroupId == accHelper->getGroupIDByName("Sundry Creditors") ){

//            qDebug()<<"Insert billwise";

//            BillwiseRecordsDatabaseHelper* billHelper = new BillwiseRecordsDatabaseHelper;
//            billHelper->deleteBillwiseRecordsByVoucher(voucher->voucherNumber, voucher->voucherType, voucher->VoucherPrefix);
//            if(ledger.mapList.size() > 0)
//            {
//                for(BillwiseRecordDataModel* a:ledger.mapList){
//                    if(a->VoucherNo.length()==0 && a->VoucherType == "OPENING")
//                        a->VoucherNo = "OPENING";
//                    else{
//                        a->VoucherNo=voucher->voucherNumber;
//                        a->VoucherDate=voucher->VoucherDate;
//                        a->VoucherType=voucher->voucherType;
//                        a->VoucherPrefix=voucher->VoucherPrefix;

//                    }

//                    if(a->RefVoucherNo.length()==0){
//                        a->RefVoucherNo=voucher->voucherNumber;
//                        a->RefType=voucher->voucherType;
//                        a->RefPrefix=voucher->VoucherPrefix;

//                    }
//                    billHelper->insertBillwiseRecords(a);
//                }
//            }

//        }


    }
    qDebug()<<"check if cheque exists";

//    if( ( voucher->voucherType == "PAYMENTVOUCHER" || voucher->voucherType == "RECEIPTVOUCHER" )
//            && voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts")
//            && voucher->chequeEntry != nullptr){
//        qDebug()<<"inserting cheque";
//        ChequeDetailsDatabaseHelper* chqHelper = new ChequeDetailsDatabaseHelper();
//        chqHelper->deleteChequeDetailsByVoucher(voucher->chequeEntry);
//        chqHelper->insertChequeDetails(voucher->chequeEntry);
//    }



}


int TransactionHelper::deleteProxyTransaction(GeneralVoucherDataObject *obj)
{

    QMap<QString,QVariant> map;

    map.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
    map.insert(Transaction_Helper_VoucherType,obj->voucherType);
    map.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);

    db->deleteRecordsPlus(map,Proxy_Transaction_Helper_TableName);

//    BillwiseRecordsDatabaseHelper* billHelper = new BillwiseRecordsDatabaseHelper;
//    billHelper->deleteBillwiseRecordsByVoucher(voucher->voucherNumber);

}
