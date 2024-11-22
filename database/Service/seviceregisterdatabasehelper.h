#ifndef SEVICEREGISTERDATABASEHELPER_H
#define SEVICEREGISTERDATABASEHELPER_H

#include <database/databasehelper/databasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <datamodels/Services/servicecomplaintregisterdatamodel.h>
#include <datamodels/Services/complaintlogdatamodel.h>

class SeviceRegisterDatabaseHelper
{
public:
    SeviceRegisterDatabaseHelper();


    bool flag = true;
    DatabaseHelper *db;
    int startTransaction();
    int commit();

    QString ServiceComplaintRegister_Main_TableName = "ServiceComplaintRegister_Main";

    QString ServiceComplaintRegister_Main_id = "_id";

    QString ServiceComplaintRegister_Main_Ledger_ID = "Ledger_ID";
    QString ServiceComplaintRegister_Main_Contact_id = "Contact_ID";
    QString ServiceComplaintRegister_Main_VoucherDate = "Voucher_Date";
    QString ServiceComplaintRegister_Main_DateCreated = "Date_Created";
    QString ServiceComplaintRegister_Main_TimeStamp = "TimeStamp";
    QString ServiceComplaintRegister_Main_Location = "Location";
    QString ServiceComplaintRegister_Main_ComplaintStatus = "Complaint_Status";
    QString ServiceComplaintRegister_Main_CompletionDate = "Completion_Date";
    QString ServiceComplaintRegister_Main_Remarks ="Remarks";
    QString ServiceComplaintRegister_Main_NonCompletion_Reason="Non_Completion_Reason";
    QString ServiceComplaintRegister_Main_NonCompletion_Id = "Non_Completion_id";
    QString ServiceComplaintRegister_Main_InspectedBy = "InspectedBy";
    QString ServiceComplaintRegister_Main_InspectionCharge = "InspectionCharge";
    QString ServiceComplaintRegister_Main_InspectedDate = "Inspected_Date";





    QString ServiceComplaintRegister_Details_TableName = "ServiceComplaintRegister_Details";
    QString ServiceComplaintRegister_Details_id = "_id";
    QString ServiceComplaintRegister_Details_Voucher_id = "Voucher_Id";
    QString ServiceComplaintRegister_Details_itemID = "Item_ID";
    QString ServiceComplaintRegister_Details_ItemDescripion = "Item_Description";
    QString ServiceComplaintRegister_Details_ItemComplaintDetail = "Item_Complaint_Detail";
    QString ServiceComplaintRegister_Details_ItemComplaintStatusid = "Item_Complaint_Status_id";
    QString ServiceComplaintRegister_Details_ItemNonCompletionReasonid = "Item_Non_Completion_Reason_ID";
    QString ServiceComplaintRegister_Details_ItemNonCompletionRemarks = "Item_Non_Completion_Remarks";
    QString ServiceComplaintRegister_Details_TechnicianRemarks = "TechnicianRemarks";
    QString ServiceComplaintRegister_Details_TechnicianFixed = "Fixed_Technician";
    QString ServiceComplaintRegister_Details_InspectedBy = "Inspected_By";
    QString ServiceComplaintRegister_Details_FixedDate = "Fixed_Date";
    QString ServiceComplaintRegister_Details_isUnderWarranty = "is_Under_Warranty";


    QString ServiceComplaintRegister_Log_TableName = "ComplaintRegister_Log";
    QString ServiceComplaintRegister_Log_id = "_id";
    QString ServiceComplaintRegister_Log_DetailsId = "Details_ID";
    QString ServiceComplaintRegister_Log_UserId = "User_Id";
    QString ServiceComplaintRegister_Log_TimeStamp = "TimeStamp";
    QString ServiceComplaintRegister_Log_Date = "Date";
    QString ServiceComplaintRegister_Log_Status = "Status";





    int insertServiceComplaint(ServiceComplaintRegisterDataModel *voucher);
    int updateServiceComplaint(ServiceComplaintRegisterDataModel *voucher,QString oldID);
    int deleteServiceComplaint(QString oldID);

    int insertServiceComplaintLog(ServiceComplaintRegisterDataModel *voucher);
    int updateServiceComplaintLog(ServiceComplaintRegisterDataModel *voucher,QString oldID);
    int deleteServiceComplaintLog(QString oldID);



    QList<ServiceComplaintRegisterDataModel> getServicesRegisterdinDate(QDate fromDate,QDate toDate,QString ledgerId ="",
                                                                        QString ContactID ="");

    QList<ComplaintLogDataModel> getLogByID(QString itemDetailID);


};

#endif // SEVICEREGISTERDATABASEHELPER_H
