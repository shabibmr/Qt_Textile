#include "seviceregisterdatabasehelper.h"

SeviceRegisterDatabaseHelper::SeviceRegisterDatabaseHelper()
{
    db = new DatabaseHelper;
}

int SeviceRegisterDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int SeviceRegisterDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
    return 1;
}

int SeviceRegisterDatabaseHelper::insertServiceComplaint(ServiceComplaintRegisterDataModel *voucher)
{
    this->startTransaction();

//    QMap<QString,QVariant> map;
//    map.insert(ServiceComplaintRegister_Main_Ledger_ID,voucher->addedByName);
//    map.insert(ServiceComplaintRegister_Main_Contact_id,voucher->addedByID);
//    map.insert(ServiceComplaintRegister_Main_VoucherDate,voucher->date);
//    map.insert(ServiceComplaintRegister_Main_DateCreated,voucher->date);
//    map.insert(ServiceComplaintRegister_Main_TimeStamp,voucher->TimeCreated);
//    map.insert(ServiceComplaintRegister_Main_Location,voucher->location);
//    map.insert(ServiceComplaintRegister_Main_ComplaintStatus,voucher->ComplaintStatus);
//    map.insert(ServiceComplaintRegister_Main_CompletionDate,voucher->date);
//    map.insert(ServiceComplaintRegister_Main_Remarks,voucher->);
//    map.insert(ServiceComplaintRegister_Main_NonCompletion_Reason,voucher->);
//    map.insert(ServiceComplaintRegister_Main_NonCompletion_Id,voucher->);
//    map.insert(ServiceComplaintRegister_Main_InspectedBy,voucher->);
//    map.insert(ServiceComplaintRegister_Main_InspectionCharge,voucher->);
//    map.insert(ServiceComplaintRegister_Main_InspectedDate,voucher->);
//    qint64 vchNo = db->insertRecordsPlus(map,ServiceComplaintRegister_Main_TableName);

//    QMap<QString,QVariant> map1;
//    map1.insert(ServiceComplaintRegister_Details_Voucher_id,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_itemID,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_ItemDescripion,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_ItemComplaintDetail,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_ItemComplaintStatusid,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_ItemNonCompletionReasonid,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_ItemNonCompletionRemarks,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_TechnicianRemarks,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_TechnicianFixed,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_InspectedBy,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_FixedDate,voucher->);
//    map1.insert(ServiceComplaintRegister_Details_isUnderWarranty,voucher->);
//    db->insertRecordsPlus(map,ServiceComplaintRegister_Details_TableName);

//    QMap<QString,QVariant> map2;
//    map2.insert(ServiceComplaintRegister_Log_DetailsId,voucher->);
//    map2.insert(ServiceComplaintRegister_Log_UserId,voucher->);
//    map2.insert(ServiceComplaintRegister_Log_TimeStamp,voucher->);
//    map2.insert(ServiceComplaintRegister_Log_Date,voucher->);
//    map2.insert(ServiceComplaintRegister_Log_Status,voucher->);
//    db->insertRecordsPlus(map2,ServiceComplaintRegister_Log_TableName);

    return commit();
}

int SeviceRegisterDatabaseHelper::updateServiceComplaint(ServiceComplaintRegisterDataModel *voucher, QString oldID)
{
    this->startTransaction();

    return commit();
}

int SeviceRegisterDatabaseHelper::deleteServiceComplaint(QString oldID)
{
    this->startTransaction();

    return commit();
}

QList<ServiceComplaintRegisterDataModel> SeviceRegisterDatabaseHelper::getServicesRegisterdinDate(QDate fromDate,
                                                                                                  QDate toDate,
                                                                                                  QString ledgerId,
                                                                                                  QString ContactID)
{
    QString ledgerClause = "";

    if(ledgerId.length()>0){
        ledgerClause = "";
    }

    QString contactsClause = "";

    if(ContactID.length()>0){
        contactsClause = "";
    }



}

QList<ComplaintLogDataModel> SeviceRegisterDatabaseHelper::getLogByID(QString itemDetailID)
{
    QList<ComplaintLogDataModel> Logs;


    return Logs;
}
