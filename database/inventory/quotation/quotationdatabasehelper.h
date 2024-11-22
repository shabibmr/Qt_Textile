#ifndef QUOTATIONDATABASEHELPER_H
#define QUOTATIONDATABASEHELPER_H


#include "datamodels/transaction/generalvoucherdataobject.h"
#include "datamodels/projects/Documents/documentsdatamodel.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/addressbook/addressbookdatabasehelper.h"
#include "datamodels/projects/Quotation/documenttype.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

#include <QMap>
#include <QDebug>

class QuotationDatabaseHelper
{
public:
    QuotationDatabaseHelper();
    ~QuotationDatabaseHelper();

    QString Quotation_Main_TableName = "Quotation_Main";

    QString Quotation_Main_id = "_id";
    QString Quotation_Main_Voucher_No = "Voucher_No";
    QString Quotation_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Quotation_Main_Date_Created = "Date_Created";
    QString Quotation_Main_Voucher_Date = "Voucher_Date";
    QString Quotation_Main_TimeStamp = "TimeStamp";
    QString Quotation_Main_TimeStampC = "TimeStamp";

    QString Quotation_Main_Last_Updated = "Last_Updated";
    QString Quotation_Main_Ledger_ID = "Ledger_ID";
    QString Quotation_Main_Billing_Name = "Billing_Name";
    QString Quotation_Main_Total = "Total";
    QString Quotation_Main_Discount_In_Amount ="Discount_in_Amount";
    QString Quotation_Main_Discount_In_Percent = "Discount_in_Percent";
    QString Quotation_Main_Discount_Total="Discount_Total";
    QString Quotation_Main_Narration = "Narration";
    QString Quotation_Main_Added_By = "Added_By";
    QString Quotation_Main_Delivery_Date = "Delivery_Date";
    QString Quotation_Main_Completion_Probability = "Completion_Probability";
    QString Quotation_Main_Credit_Period = "Credit_Period";
    QString Quotation_Main_POC_Name = "POC_Name";
    QString Quotation_Main_POC_Phone = "POC_Phone";
    QString Quotation_Main_POC_Email = "POC_Email";
    QString Quotation_Main_Project_ID = "Project_ID";
    QString Quotation_Main_Revision_Number = "Revision_Number";
    QString Quotation_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Quotation_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Quotation_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Quotation_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Quotation_Main_Salesman_ID = "Salesman_ID";
    QString Quotation_Main_Terms_And_Conditions_ID = "Terms_And_Conditions_ID";
    QString Quotation_Main_Manager_Approval_Status ="ManagerApprovalStatus";
    QString Quotation_Main_Client_Approval_Status ="ClientApprovalStatus";
    QString Quotation_Main_VoucherStatus="VoucherStatus";
    QString Quotation_Main_Contact_ID="ContactID";
    QString Quotation_Main_LPO_Ref="LPO_Ref";

    QString Quotation_Main_VatAmount = "VAT_AMOUNT";
    QString Quotation_Main_Gross_Total="Gross_Total";
    QString Quotation_Main_Location = "Location";

    QString Quotation_Main_Client_Approval_Remarks = "Remarks";
    QString Quotation_Main_ClientApprovedBy ="ClientApprovedBy";
    QString Quotation_Main_Advance = "Payment_Advance";
    QString Quotation_Main_PeymentTerms = "Payment_Terms";
    QString Quotation_Main_Accounts_Remarks = "Accounts_Remarks";


    QString Quotation_Details_TableName = "Quotation_Details";

    QString Quotation_Details_id = "_id";
    QString Quotation_Details_Voucher_No = "Voucher_No";
    QString Quotation_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Quotation_Details_Voucher_Date = "Voucher_Date";
    QString Quotation_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Quotation_Details_Quantity = "Quantity";
    QString Quotation_Details_Price = "Price";
    QString Quotation_Details_PriceLevel = "PriceLevel";
    QString Quotation_Details_isCompoundItem = "isCompoundItem";
    QString Quotation_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Quotation_Details_Discount_Percent = "Discount_Percent";
    QString Quotation_Details_Discount_Amount = "Discount_Amount";
    QString Quotation_Details_Subtotal = "Subtotal";
    QString Quotation_Details_Project_ID = "Project_ID";
    QString Quotation_Details_Item_Narration = "Item_Narration";
    QString Quotation_Details_Item_Description = "Item_Description";
    QString Quotation_Details_Salesman_ID = "Salesman_ID";
    QString Quotation_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Quotation_Details_Length="Length";
    QString Quotation_Details_Dimenstion = "Dimension";
    QString itemVoucherStatus ="ItemVoucherStatus";
    QString itemProductionStatus ="itemProductionStatus";


    QString Proxy_Transaction_Helper_TableName = "Proxy_Transaction_Helper";
    QString Proxy_Transaction_Helper_id = "_id";
    QString Proxy_Transaction_Helper_VoucherType = "Voucher_Type";
    QString Proxy_Transaction_Helper_VoucherPrefix = "Voucher_Prefix";
    QString Proxy_Transaction_Helper_VoucherNo = "Voucher_ID";
    QString Proxy_Transaction_Helper_VoucherDate = "Voucher_Date";
    QString Proxy_Transaction_Helper_ProjectID = "Project_ID";
    QString Proxy_Transaction_Helper_TranactionAmount = "Transaction_Amount";
    QString Proxy_Transaction_Helper_CreditLedgerID = "Credit_Ledger_ID";
    QString Proxy_Transaction_Helper_DebitLedgerID = "Debit_Ledger_ID";
    QString Proxy_Transaction_Helper_Narration = "Narration";
    QString Proxy_Transaction_Helper_SalesManID = "Salesman_ID";
    QString Proxy_Transaction_Helper_TimeStamp = "TIMESTAMP";
    QString Proxy_Transaction_Helper_isAdditional ="isAdditional";


    QString Quotation_Documents_TableName="Quotation_Documents_Table";


    QString Quotation_Documents_id = "_id";
    QString Quotation_Documents_File_Name = "File_Name";
    QString Quotation_Documents_File_Type = "File_Type";
    QString Quotation_Documents_File_Description = "File_Description";
    QString Quotation_Documents_File_Data = "File_Data";
    QString Quotation_Documents_AddedByID = "AddedByID";
    QString Quotation_Documents_ReqId = "ReqId";
    QString Quotation_Documents_ProjectId = "ProjectId";
    QString Quotation_Documents_QuotationNumber = "Quotation_Number";
    QString Quotation_Documents_TimeStamp = "TimeStamp";
    QString Quotation_Documents_TagType = "TagType";
    QString Quotation_Documents_SearchTag = "SearchTag";
    QString Quotation_Documents_ClientApprovalStatus = "ClientApprovalStatus";


    QString ledgerType = "QUOTATION";


    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertQuotation(GeneralVoucherDataObject *obj);

    int updateQuotation(GeneralVoucherDataObject *obj, QString oldID);

    int deleteQuotation(QString oldID);

    int commit(int n=1);

    int getMaxID();

    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);

    GeneralVoucherDataObject *getVoucherByVoucherNoAsPtr(QString vNo);

    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom,QDate dateTo);

    int setClientApproval(QString vid,bool value,QString LPO,QDate deliveryDate,QString notes ="");
    int setManagerApproval(QString vid,bool value);

    bool getClientApprovalStatus(QString vid);
    bool getManagerApprovalStatus(QString vid);

    int updateQuotationStatus(QString quotationVoucherNumber,QString status);

    void setItemVoucherStatus(float qty,QString id);
    void resetItemVoucherStatus(float qty,QString id);

    QMap<QString,float> getAllUnimportedQuotes(QString ledid,QDate date);

    int getFileCount(QString quoteNUmber);

    QByteArray getFilewithId(int id);
    QList<DocumentsDataModel> getALLFilesWithReqID(int reqID ,int TagType=DocumentType::Drafts);

    QList<DocumentsDataModel> getALLFilesWithQuoteNo(QString quoteNo, int tagType =DocumentType::Drafts);



    int insertQImage(DocumentsDataModel item);

    int updateQImage(DocumentsDataModel item ,int id);

    int deleteQImage(int id);

    QString getRevisionNumber(QString vNO);

    QString setQuotationStatus(int status,QString qNo);

    int setImageApprovalStatusByID(int id,int status);

    int getQuotationStatus(QString qNo);


    QString getNextVoucherNo(QString voucherPrefix);
};

#endif // QUOTATIONDATABASEHELPER_H


