#ifndef DEFECTIVEDATABASEHELPER_H
#define DEFECTIVEDATABASEHELPER_H


#include "database/voucherdbabstract.h"


class DefectiveDatabaseHelper : public VoucherDBAbstract
{
public:
    DefectiveDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~DefectiveDatabaseHelper() override;


    QString Wastage_Entry_Main_TableName = "Wastage_Entry_Main";

    QString Wastage_Entry_Main_id = "_id";
    QString Wastage_Entry_Main_Voucher_No = "Voucher_No";
    QString Wastage_Entry_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Wastage_Entry_Main_Date_Created = "Date_Created";
    QString Wastage_Entry_Main_Voucher_Date = "Voucher_Date";
    QString Wastage_Entry_Main_TimeStamp = "`TimeStamp`";
    QString Wastage_Entry_Main_TimeStampC = "TimeStamp";

    QString Wastage_Entry_Main_Last_Updated = "Last_Updated";
    QString Wastage_Entry_Main_Ledger_ID = "Ledger_ID";
    QString Wastage_Entry_Main_Billing_Name = "Billing_Name";
    QString Wastage_Entry_Main_Total = "Total";
    QString Wastage_Entry_Main_Narration = "Narration";
    QString Wastage_Entry_Main_Added_By = "Added_By";
    QString Wastage_Entry_Main_Delivery_Date = "Delivery_Date";
    QString Wastage_Entry_Main_Completion_Probability = "Completion_Probability";
    QString Wastage_Entry_Main_Credit_Period = "Credit_Period";
    QString Wastage_Entry_Main_POC_Name = "POC_Name";
    QString Wastage_Entry_Main_POC_Phone = "POC_Phone";
    QString Wastage_Entry_Main_POC_Email = "POC_Email";
    QString Wastage_Entry_Main_Project_ID = "Project_ID";
    QString Wastage_Entry_Main_Revision_Number = "Revision_Number";
    QString Wastage_Entry_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Wastage_Entry_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Wastage_Entry_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Wastage_Entry_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Wastage_Entry_Main_Salesman_ID = "Salesman_ID";
//    QString Wastage_Entry_Main_LPORef="LPO_Ref";
//    QString Wastage_Entry_Main_ModeOfService = "MODE_OF_SERVICE";
//    QString Wastage_Entry_Main_Discount_In_Amount = "Discount_In_Amount";
//    QString Wastage_Entry_Main_Discount_In_Percent = "Discount_In_Percent";
//    QString Wastage_Entry_Main_VATAmount = "VAT_Amount";
    QString Wastage_Entry_Main_VoucherStatus = "VoucherStatus";
//    QString Wastage_Entry_Main_Contact_ID = "ContactID";
//    QString Wastage_Entry_Main_Gross_Total = "Gross_Total";
//    QString Wastage_Entry_Main_Reference ="Reference";
//    QString Wastage_Entry_Main_Customer_Expecting_Date = "Customer_Expected_Date";
//    QString Wastage_Entry_Main_Currency = "Currency";
//    QString Wastage_Entry_Main_BoxesCount = "Boxes_Count";
//    QString Wastage_Entry_Main_TotalWeight = "Total_Weight";
//    QString Wastage_Entry_Main_LRNo = "LR_No";
//    QString Wastage_Entry_Main_VoucherNoDisplay = "Voucher_No_Display";
//    QString Wastage_Entry_Main_ReqVoucherList = "Requirement_Voucher_List";
//    QString Wastage_Entry_Main_Origin = "Origin";
//    QString Wastage_Entry_Main_Round_Off = "Round_Off";

    QString Wastage_Entry_Details_TableName = "Wastage_Entry_Details";


    QString Wastage_Entry_Details_id = "_id";
    QString Wastage_Entry_Details_Voucher_No = "Voucher_No";
    QString Wastage_Entry_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Wastage_Entry_Details_Voucher_Date = "Voucher_Date";
    QString Wastage_Entry_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Wastage_Entry_Details_Dr_Quantity = "DrQty";
    QString Wastage_Entry_Details_Cr_Quantity = "CrQty";
    QString Wastage_Entry_Details_Quantity = "Quantity";
    QString Wastage_Entry_Details_Price = "Price";
    QString Wastage_Entry_Details_UOM_ID ="UOM_ID" ;
    QString Wastage_Entry_Details_PriceLevel = "PriceLevel";
    QString Wastage_Entry_Details_isCompoundItem = "isCompoundItem";
    QString Wastage_Entry_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Wastage_Entry_Details_Discount_Percent = "Discount_Percent";
    QString Wastage_Entry_Details_Discount_Amount = "Discount_Amount";
    QString Wastage_Entry_Details_Subtotal = "Subtotal";
    QString Wastage_Entry_Details_Project_ID = "Project_ID";
    QString Wastage_Entry_Details_Item_Narration = "Item_Narration";
    QString Wastage_Entry_Details_Item_Description = "Item_Description";
    QString Wastage_Entry_Details_Salesman_ID = "Salesman_ID";
    QString Wastage_Entry_Details_Requirement_ItemID ="Requirement_ItemID";
//    QString Wastage_Entry_Details_itemVoucherStatus ="ItemVoucherStatus";
//    QString Wastage_Entry_Details_itemProductionStatus ="itemProductionStatus";
//    QString Wastage_Entry_Details_Time = "Time";
//    QString Wastage_Entry_Details_Length = "Length";
//    QString Wastage_Entry_Details_Dimension = "Dimension";

    QString VoucherType = "WASTAGEENTRY";


    bool flag = true;
    DatabaseHelper *db;
    ItemTransactionDatabaseHelper *itemTransaction;
    ItemTransactionDataModel *ItemModel;

    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj) override;

    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;

    int deleteVoucher(GeneralVoucherDataObject *obj) override;

    int commit();

    int getMaxID();

    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);

    GeneralVoucherDataObject* getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;

    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom,QDate dateTo);
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom,QDate dateTo) override;

    QString getNextVoucherNo(QString voucherPrefix) override;

    QSqlQueryModel* getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;
    QString getVoucherNumberByLedID(QString LedID,QDate Date) override;
};

#endif // DEFECTIVEDATABASEHELPER_H
