#ifndef WORKORDERDATABASEHELPER_H
#define WORKORDERDATABASEHELPER_H

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/Notifications/sendchanneldatabasehelper.h"
#include "database/inventory/StockRequest/stockrequestdatabasehelper.h"
#include "finance/transactions/calculator/fifocalculator.h"
#include "customwidgets/Models/gmsqlquerymodel.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QMap>
#include <QDebug>

#include "database/voucherdbabstract.h"


class WorkOrderDatabaseHelper : public VoucherDBAbstract
{
public:
    WorkOrderDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~WorkOrderDatabaseHelper() override;
    static QString Work_Order_Main_TableName;

    static QString Work_Order_Main_id;
    static QString Work_Order_Main_Voucher_No;
    static QString Work_Order_Main_Voucher_Prefix ;
    static QString Work_Order_Main_Date_Created ;
    static QString Work_Order_Main_Voucher_Date;
    static QString Work_Order_Main_TimeStamp;
    static QString Work_Order_Main_TimeStampC;

    static QString Work_Order_Main_Last_Updated;
    static QString Work_Order_Main_Ledger_ID;
    static QString Work_Order_Main_Billing_Name;
    static QString Work_Order_Main_Total ;
    static QString Work_Order_Main_Narration;
    static QString Work_Order_Main_Added_By ;
    static QString Work_Order_Main_Delivery_Date ;
    static QString Work_Order_Main_Completion_Probability;
    static QString Work_Order_Main_Credit_Period;
    static QString Work_Order_Main_POC_Name;
    static QString Work_Order_Main_POC_Phone;
    static QString Work_Order_Main_POC_Email ;
    static QString Work_Order_Main_Project_ID ;
    static QString Work_Order_Main_Revision_Number ;
    static QString Work_Order_Main_Converted_To_Sales_Order ;
    static QString Work_Order_Main_Quotation_Dropped;
    static QString Work_Order_Main_Quotation_Dropped_Reason;
    static QString Work_Order_Main_Requirement_Voucher_No;
    static QString Work_Order_Main_Salesman_ID;
    static QString Work_Order_Main_LPO ;
    static QString Work_Order_Main_VoucherStatus ;
    static QString Work_Order_Main_ModeOfService ;
    static QString Work_Order_Main_Discount_In_Amount;
    static QString Work_Order_Main_Discount_In_Percent;
    static QString Work_Order_Main_VATAmount ;
    static QString Work_Order_Main_Contact_ID;
    static QString Work_Order_Main_Gross_Total ;
    static QString Work_Order_Main_Reference ;
    static QString Work_Order_Main_Customer_Expecting_Date;
    static QString Work_Order_Main_Currency;
    static QString Work_Order_Main_BoxesCount;
    static QString Work_Order_Main_TotalWeight;
    static QString Work_Order_Main_LRNo;
    static QString Work_Order_Main_VoucherNoDisplay;
    static QString Work_Order_Main_ReqVoucherList;
    static QString Work_Order_Main_Origin;
    static QString Work_Order_Main_RoundOff;
    static QString Work_Order_Main_ToLocation;
    static QString Work_Order_Main_TransactionId;
    static QString Work_Order_Main_Completed_TimeStamp;


    static QString WorkOrder_tableName ;
    static QString WorkOrder_id ;
    static QString WorkOrder_SalesOrder_Id ;
    static QString WorkOrder_requirement_no;
    static QString WorkOrder_WorkOrder_id;
    static QString WorkOrder_qty;
    static QString WorkOrder_Price;
    static QString WorkOrder_itemID;
    static QString WorkOrder_SerialNo;
    static QString WorkOrder_status;
    static QString WorkOrder_TechnicanID;
    static QString WorkOrder_CompletedDate;
    static QString WorkOrder_TimeStamp;
    static QString WorkOrder_DN_id;
    static QString WorkOrder_Expected_Date;
    static QString WorkOrder_VoucherPrefix;
    static QString WorkOrder_StartTimestamp;
    static QString WorkOrder_Section;
    static QString WorkOrder_CompletionTimestamp;
    static QString WorkOrder_itemVoucherStatus ;
    static QString WorkOrder_itemProductionStatus;
    static QString WorkOrder_Project_ID;
    static QString WorkOrder_Salesman_ID;
    static QString WorkOrder_Requirement_ItemID;
    static QString WorkOrder_UOM_ID ;


    static QString VoucherType;
    bool flag = true;
    DatabaseHelper *db;

    QString getNextVoucherNo(QString voucherPrefix) override;
    GeneralVoucherDataObject getWorkOrderBySalesOrder(QString SalesOrderID);

    int startTransaction();
    int insertVoucher(GeneralVoucherDataObject *obj) override;
    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;
    int deleteVoucher(GeneralVoucherDataObject *obj) override;
    void updateSalesOrderStatus(QString itemReqId, int status);
    int updateWOStatusByReqVoucherNo(QString reqVoucherNo, int status);
    int commit();
    int getMaxID();

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo,
                                                       QString voucherPrefix = LoginValues::voucherPrefix)override;
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override;

    QSqlQueryModel* getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;
    QString getVoucherNumberByLedID(QString LedID,QDate Date) override;

    QSqlQueryModel *getWorkOrdersByReqVoucherNo();

};

#endif // WORKORDERDATABASEHELPER_H
