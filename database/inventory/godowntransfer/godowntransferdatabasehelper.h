#ifndef GODOWNTRANSFERDATABASEHELPER_H
#define GODOWNTRANSFERDATABASEHELPER_H

#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include "database/Notifications/sendchanneldatabasehelper.h"
#include <QMap>
#include <QDebug>


class GodownTransferDatabaseHelper
{
public:
    GodownTransferDatabaseHelper();
    QString Godown_Transfer_Main_TableName = "Godown_Tranfer_Main";

    QString Godown_Transfer_Main_id = "_id";
    QString Godown_Transfer_Main_Voucher_No = "Voucher_No";
    QString Godown_Transfer_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Godown_Transfer_Main_Voucher_Date = "Voucher_Date";
    QString Godown_Transfer_Main_Date_Created = "Date_Created";
    QString Godown_Transfer_Main_TimeStamp = "TimeStamp";
    QString Godown_Transfer_Main_Last_Updated = "Last_Updated";
    QString Godown_Transfer_Main_Ledger_ID = "Ledger_ID";
    QString Godown_Transfer_Main_Billing_Name = "Billing_Name";
    QString Godown_Transfer_Main_Total = "Total";
    QString Godown_Transfer_Main_Narration = "Narration";
    QString Godown_Transfer_Main_Added_By = "Added_By";
    QString Godown_Transfer_Main_Delivery_Date = "Delivery_Date";
    QString Godown_Transfer_Main_Completion_Probability = "Completion_Probability";
    QString Godown_Transfer_Main_Credit_Period = "Credit_Period";
    QString Godown_Transfer_Main_POC_Name = "POC_Name";
    QString Godown_Transfer_Main_POC_Phone = "POC_Phone";
    QString Godown_Transfer_Main_POC_Email = "POC_Email";
    QString Godown_Transfer_Main_Project_ID = "Project_ID";
    QString Godown_Transfer_Main_Revision_Number = "Revision_Number";
    QString Godown_Transfer_Main_Salesman_ID = "Salesman_ID";
    QString Godown_Transfer_Main_FromLocation = "From_Location";
    QString Godown_Transfer_Main_ToLocation = "To_Location";
    QString Godown_Transfer_Main_VoucherStatus = "VoucherStatus";


    QString Godown_Transfer_Details_TableName = "Godown_Transfer_Details";
    QString Godown_Transfer_Details_id = "_id";
    QString Godown_Transfer_Details_Voucher_No = "Voucher_No";
    QString Godown_Transfer_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Godown_Transfer_Details_Voucher_Date = "Voucher_Date";
    QString Godown_Transfer_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Godown_Transfer_Details_Quantity = "Quantity";
    QString Godown_Transfer_Details_Difference = "Adj_Value";
    QString Godown_Transfer_Details_Price = "Price";
    QString Godown_Transfer_Details_PriceLevel = "PriceLevel";
    QString Godown_Transfer_Details_isCompoundItem = "isCompoundItem";
    QString Godown_Transfer_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Godown_Transfer_Details_Discount_Percent = "Discount_Percent";
    QString Godown_Transfer_Details_Discount_Amount = "Discount_Amount";
    QString Godown_Transfer_Details_Subtotal = "Subtotal";
    QString Godown_Transfer_Details_Project_ID = "Project_ID";
    QString Godown_Transfer_Details_Item_Narration = "Item_Narration";
    QString Godown_Transfer_Details_Item_Description = "Item_Description";
    QString Godown_Transfer_Details_Salesman_ID = "Salesman_ID";
    QString Godown_Transfer_Details_Uom = "UOM_Id";
    QString Godown_Transfer_Details_itemVoucherStatus = "itemVoucherStatus";
    QString Godown_Transfer_Details_itemProductionStatus = "itemProductionStatus";
    QString Godown_Transfer_Details_Time = "CurrTime";
    QString Godown_Transfer_Details_Requirement_ItemID = "Requirement_ItemID";

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


    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::GodownTransfer); // "GODOWNTRANSFER";
    bool flag = true;
    DatabaseHelper *db;
    ItemTransactionDatabaseHelper *itemTransaction;
    ItemTransactionDataModel *ItemModel;
    TransactionHelper *ledTransaction;

    int startTransaction();

    int insertGodownTransfer(GeneralVoucherDataObject *obj);

    int updateGodownTransfer(GeneralVoucherDataObject *obj, QString oldID);

    int deleteGodownTransfer(GeneralVoucherDataObject *obj);

    int commit();

    int getMaxID();

    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom,QDate dateTo);

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix = LoginValues::voucherPrefix);

    QString getNextVoucherNo(QString voucherPrefix);
};

#endif // GODOWNTRANSFERDATABASEHELPER_H
