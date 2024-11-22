#ifndef SALESVOUCHERDELETEDDATABASEHELPER_H
#define SALESVOUCHERDELETEDDATABASEHELPER_H

//#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"
//#include "database/finance/TransactionHelper/transactionhelper.h"
//#include "datamodels/transaction/generalvoucherdataobject.h"
//#include "database/databasehelper/databasehelper.h"
//#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
//#include "database/hr/userprofiledatabasehelper.h"
//#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

//#include <QMap>
//#include <QDebug>

#include "database/voucherdbabstract.h"

class salesVoucherDeletedDatabaseHelper : public VoucherDBAbstract
{
public:
    salesVoucherDeletedDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~salesVoucherDeletedDatabaseHelper() override;

    static QString Sales_Invoice_Main_TableName ;

    static QString Sales_Invoice_Main_id;
    static QString Sales_Invoice_Main_Voucher_No;
    static QString Sales_Invoice_Main_Voucher_Prefix;
    static QString Sales_Invoice_Main_Date_Created;
    static QString Sales_Invoice_Main_Voucher_Date;
    static QString Sales_Invoice_Main_TimeStamp;
    static QString Sales_Invoice_Main_TimeStampC;

    static QString Sales_Invoice_Main_Last_Updated;
    static QString Sales_Invoice_Main_Ledger_ID;
    static QString Sales_Invoice_Main_Billing_Name;
    static QString Sales_Invoice_Main_Total;
    static QString Sales_Invoice_Main_Narration;
    static QString Sales_Invoice_Main_Added_By;
    static QString Sales_Invoice_Main_Delivery_Date;
    static QString Sales_Invoice_Main_Completion_Probability;
    static QString Sales_Invoice_Main_Credit_Period;
    static QString Sales_Invoice_Main_POC_Name;
    static QString Sales_Invoice_Main_POC_Phone;
    static QString Sales_Invoice_Main_POC_Email;
    static QString Sales_Invoice_Main_Project_ID;
    static QString Sales_Invoice_Main_Revision_Number;
    static QString Sales_Invoice_Main_Converted_To_Sales_Order;
    static QString Sales_Invoice_Main_Quotation_Dropped;
    static QString Sales_Invoice_Main_Quotation_Dropped_Reason;
    static QString Sales_Invoice_Main_Requirement_Voucher_No;
    static QString Sales_Invoice_Main_Salesman_ID;
    static QString Sales_Invoice_Main_LPORef;
    static QString Sales_Invoice_Main_ModeOfService;
    static QString Sales_Invoice_Main_Discount_In_Amount;
    static QString Sales_Invoice_Main_Discount_In_Percent;
    static QString Sales_Invoice_Main_VATAmount;
    static QString Sales_Invoice_Main_VoucherStatus;
    static QString Sales_Invoice_Main_Contact_ID;
    static QString Sales_Invoice_Main_Gross_Total;
    static QString Sales_Invoice_Main_Reference;
    static QString Sales_Invoice_Main_Round_Off;
    static QString Sales_Invoice_Main_Customer_Expecting_Date;
    static QString Sales_Invoice_Main_Currency;
    static QString Sales_Invoice_Main_BoxesCount;
    static QString Sales_Invoice_Main_TotalWeight;
    static QString Sales_Invoice_Main_LRNo;
    static QString Sales_Invoice_Main_VoucherNoDisplay;
    static QString Sales_Invoice_Main_ReqVoucherList;
    static QString Sales_Invoice_Main_Origin;
    static QString Sales_Invoice_Main_GeneratedTransactionId;
    static QString Sales_Invoice_Main_PaymentSplit;


    QString Sales_Invoice_Details_TableName = "Sales_Invoice_Details_Deleted";

    QString Sales_Invoice_Details_id = "_id";
    QString Sales_Invoice_Details_Voucher_No = "Voucher_No";
    QString Sales_Invoice_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Sales_Invoice_Details_Voucher_Date = "Voucher_Date";
    QString Sales_Invoice_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Sales_Invoice_Details_Quantity = "Quantity";
    QString Sales_Invoice_Details_Price = "Price";
    QString Sales_Invoice_Details_PriceLevel = "PriceLevel";
    QString Sales_Invoice_Details_isCompoundItem = "isCompoundItem";
    QString Sales_Invoice_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Sales_Invoice_Details_Discount_Percent = "Discount_Percent";
    QString Sales_Invoice_Details_Discount_Amount = "Discount_Amount";
    QString Sales_Invoice_Details_Subtotal = "Subtotal";
    QString Sales_Invoice_Details_Project_ID = "Project_ID";
    QString Sales_Invoice_Details_Item_Narration = "Item_Narration";
    QString Sales_Invoice_Details_Item_Description = "Item_Description";
    QString Sales_Invoice_Details_Salesman_ID = "Salesman_ID";
    QString Sales_Invoice_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Sales_Invoice_Details_Length ="Length";
    QString Sales_Invoice_Details_Time ="Time";
    QString Sales_Invoice_Details_UomId = "Uom_Id";
    QString Sales_Invoice_Details_Dimension = "Dimension";
    QString Sales_Invoice_Details_itemVoucherStatus = "itemVoucherStatus";
    QString Sales_Invoice_Details_itemProductionStatus = "itemProductionStatus";
    QString Sales_Invoice_Details_TaxRate = "Tax_Rate";
    QString Sales_Invoice_Details_TaxAmount = "Tax_Amount";

    QString Transaction_Helper_TableName = "Transaction_Helper";
    QString Transaction_Helper_id = "_id";
    QString Transaction_Helper_VoucherType = "Voucher_Type";
    QString Transaction_Helper_VoucherPrefix = "Voucher_Prefix";
    QString Transaction_Helper_VoucherNo = "Voucher_ID";
    QString Transaction_Helper_VoucherDate = "Voucher_Date";
    QString Transaction_Helper_ProjectID = "Project_ID";
    QString Transaction_Helper_TranactionAmount = "Transaction_Amount";
    QString Transaction_Helper_CreditLedgerID = "Credit_Ledger_ID";
    QString Transaction_Helper_DebitLedgerID = "Debit_Ledger_ID";
    QString Transaction_Helper_Narration = "Narration";
    QString Transaction_Helper_SalesManID = "Salesman_ID";
    QString Transaction_Helper_TimeStamp = "TIMESTAMP";
    QString Transaction_Helper_isAdditional ="isAdditional";

    QString VoucherType = "DELTEDSALESVOUCHER";

    bool flag = true;
    DatabaseHelper *db;
    ItemTransactionDatabaseHelper *itemTransaction;

    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj) override;



    int deleteSalesVoucher(GeneralVoucherDataObject *obj);

    int commit();

    int getMaxID();

    int getMaxIDwithPrefix(QString prefix);

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;

    QSqlQueryModel *getVoucherListByDateTimeasModel(QDateTime,QDateTime);


    QSqlQueryModel *getDeletedItemsListByDateTime(QDateTime,QDateTime);

    void switchSalesMan(int newEmpID,QString vNO);

    QString getActualVoucherByREFandDate(QString ref, QDate date);

    QString getActualVoucherByREFandDate(QString ref, QDateTime date);

    QString getNextVoucherNo(QString voucherPrefix) override;
    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;

    QList<inventoryItemDataModel> getItemsListForMail(QDateTime fromTime,QDateTime toTime);

    // VoucherDBAbstract interface
public:
    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;
    int deleteVoucher(GeneralVoucherDataObject *obj) override;
    QString getVoucherNumberByLedID(QString LedID, QDate Date) override;
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override;
};

#endif // SALESVOUCHERDELETEDDATABASEHELPER_H
