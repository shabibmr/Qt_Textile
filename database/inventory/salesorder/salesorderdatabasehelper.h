#ifndef SALESORDERDATABASEHELPER_H
#define SALESORDERDATABASEHELPER_H

#include "database/inventory/quotation/quotationdatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include "database/Notifications/sendchanneldatabasehelper.h"
#include "database/inventory/workorder/workorderdatabasehelper.h"

#include "database/voucherdbabstract.h"


class SalesOrderDatabaseHelper : public VoucherDBAbstract
{
public:
    SalesOrderDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~SalesOrderDatabaseHelper();

    static QString Sales_Order_Main_TableName;

    static QString Sales_Order_Main_id;
    static QString Sales_Order_Main_Voucher_No;
    static QString Sales_Order_Main_Voucher_Prefix ;
    static QString Sales_Order_Main_Date_Created ;
    static QString Sales_Order_Main_Voucher_Date;
    static QString Sales_Order_Main_TimeStamp;
    static QString Sales_Order_Main_TimeStampC;

    static QString Sales_Order_Main_Last_Updated;

    static QString Sales_Order_Main_Ledger_ID;
    static QString Sales_Order_Main_Billing_Name;
    static QString Sales_Order_Main_Total ;
    static QString Sales_Order_Main_Narration;
    static QString Sales_Order_Main_Added_By ;
    static QString Sales_Order_Main_Delivery_Date ;
    static QString Sales_Order_Main_Assigned_Time ;

    static QString Sales_Order_Main_Completion_Probability;
    static QString Sales_Order_Main_Credit_Period;
    static QString Sales_Order_Main_POC_Name;
    static QString Sales_Order_Main_POC_Phone;
    static QString Sales_Order_Main_POC_Email ;
    static QString Sales_Order_Main_Project_ID ;
    static QString Sales_Order_Main_Revision_Number ;
    static QString Sales_Order_Main_Converted_To_Sales_Order ;
    static QString Sales_Order_Main_Quotation_Dropped;
    static QString Sales_Order_Main_Quotation_Dropped_Reason;
    static QString Sales_Order_Main_Requirement_Voucher_No;
    static QString Sales_Order_Main_Salesman_ID;
    static QString Sales_Order_Main_LPO ;
    static QString Sales_Order_Main_VoucherStatus ;
    static QString Sales_Order_Main_ModeOfService ;
    static QString Sales_Order_Main_Discount_In_Amount;
    static QString Sales_Order_Main_Discount_In_Percent;
    static QString Sales_Order_Main_VATAmount ;
    static QString Sales_Order_Main_Contact_ID;
    static QString Sales_Order_Main_Gross_Total ;
    static QString Sales_Order_Main_Reference ;
    static QString Sales_Order_Main_Customer_Expecting_Date;
    static QString Sales_Order_Main_Currency;
    static QString Sales_Order_Main_BoxesCount;
    static QString Sales_Order_Main_TotalWeight;
    static QString Sales_Order_Main_LRNo;
    static QString Sales_Order_Main_VoucherNoDisplay;
    static QString Sales_Order_Main_ReqVoucherList;
    static QString Sales_Order_Main_Origin;
    static QString Sales_Order_Main_RoundOff;
    static QString Sales_Order_Main_Generated_TransactionId;
    static QString Sales_Order_Main_Location;

    static QString Sales_Order_Details_TableName ;

    static QString Sales_Order_Details_id;
    static QString Sales_Order_Details_Voucher_No ;
    static QString Sales_Order_Details_Voucher_Prefix ;
    static QString Sales_Order_Details_Voucher_Date ;
    static QString Sales_Order_Details_Inventory_Item_ID;
    static QString Sales_Order_Details_Quantity ;
    static QString Sales_Order_Details_QuantityFull;
    static QString Sales_Order_Details_QuantityDisc;
    static QString Sales_Order_Details_Price ;
    static QString Sales_Order_Details_UOM_ID ;
    static QString Sales_Order_Details_PriceLevel ;
    static QString Sales_Order_Details_isCompoundItem ;
    static QString Sales_Order_Details_subItemOff_list_ID ;
    static QString Sales_Order_Details_Discount_Percent;
    static QString Sales_Order_Details_Discount_Amount ;
    static QString Sales_Order_Details_Subtotal ;
    static QString Sales_Order_Details_Project_ID ;
    static QString Sales_Order_Details_Item_Narration ;
    static QString Sales_Order_Details_Item_Description ;
    static QString Sales_Order_Details_Salesman_ID ;
    static QString Sales_Order_Details_Requirement_ItemID ;
    static QString Sales_Order_Details_Length;
    static QString Sales_Order_Details_Dimension;
    static QString Sales_Order_Details_itemVoucherStatus ;
    static QString Sales_Order_Details_itemProductionStatus;
    static QString Sales_Order_Details_TimeDate;
    static QString Sales_Order_Details_TaxRate;
    static QString Sales_Order_Details_TaxAmount;
    static QString Sales_Order_Details_ContainerCount;

    QString Transaction_Helper_TableName = "Proxy_Transaction_Helper";
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
    QString Transaction_Helper_Debit_Amount = "Debit_Amount";
    QString Transaction_Helper_Credit_Amount = "Credit_Amount";
    QString Transaction_Helper_LedgerID = "Ledger_ID";
    QString Transaction_Helper_AgainstLedger = "Against_Ledger";


    static QString WorkOrder_tableName ;
    static QString WorkOrder_id ;
    static QString WorkOrder_SalesOrder_Id ;
    static QString WorkOrder_requirement_no;

    static QString WorkOrder_WorkOrder_id;
    static QString WorkOrder_qty_No;
    static QString WorkOrder_itemID;
    static QString WorkOrder_SerialNo;
    static QString WorkOrder_status;
    static QString WorkOrder_TechnicanID;
    static QString WorkOrder_CompletedDate;
    static QString WorkOrder_TimeStamp;
    static QString WorkOrder_DN_id;
    static QString WorkOrder_Expected_Date;

    static QString VoucherType;


    bool flag = true;
    DatabaseHelper *db;
    TransactionHelper *ledTransaction;



    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj);

    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID);

    int deleteVoucher(GeneralVoucherDataObject *obj);


    int commit();

    int getMaxID();

    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom,QDateTime dateTo) override;
    QString getNextVoucherNo(QString voucherPrefix)override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId)override;


    QMap<QString,float> getOccupiedTables(QString voucherPrefix = LoginValues::voucherPrefix);


//    QString getSerailNumber(qint64 reqID,int qtyNumber,QString SalesOrderNo);

//    void setSerialNumber(QString SerialNo,qint64 reqID,int qtyNumber,QString SalesOrderNo);


//    float getHowManyItemsDeliverableCustomItem(qint64 reqID);

//    float getHowManyItemsDeliverableReadyItem(qint64 reqID);

    //    QString getSerailNumberBYDN(qint64 reqID,int qtyNumber,QString SalesOrderNo);

    //    void setSerialNumberBYDN(QString SerialNo,qint64 reqID,int qtyNumber,QString SalesOrderNo);


//    QDate getExpectedCompletionDate(QString SO);

//    void setExpectedCompletionDate(QString SO,QDate date);

//    int getProductionStatus(qint64 reqID,int qtyNumber,QString SalesOrderNo);

//    void setProductionStatus(int status,qint64 reqID,int qtyNumber,QString SalesOrderNo);

//    int getTechnician(qint64 reqID,int qtyNumber,QString SalesOrderNo);

//    void setTechnician(int techID,qint64 reqID,int qtyNumber,QString SalesOrderNo);

    QDate getCompletedDate(qint64 reqID,int qtyNumber,QString SalesOrderNo);

    void setCompletedDate(QDate date,qint64 reqID,int qtyNumber,QString SalesOrderNo);

    QDate getExpectedDate(qint64 reqID,int qtyNumber,QString SalesOrderNo);

    void setExpectedDate(QDate date,qint64 reqID,int qtyNumber,QString SalesOrderNo);

    QString getWorkOrderNumberbySo(QString SO);

    QStringList getSerialNumbersbyRequirement(qint64 reqid);

    QStringList getSerailNumbersLeftonCusomtItemforDN(qint64 reqid);

    QStringList getSerailNumbersLeftonCusomtItemforDNwithdelID(qint64 reqid,QString  delID);

    QStringList getSerailNumbersLeftonReadyItemforDNwithdelID(qint64 reqid,QString  delID);

    void setDNID(qint64 rqID,int qtyno,QString dnID);

    QStringList getSerialNumbersbyRequirementandDeliveryNote(qint64 reqid,QString delID);

    QStringList getSerialNumbersbyRequirementandSOID(qint64 reqid,QString delID);

    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo,
                                                       QString voucherPrefix = LoginValues::voucherPrefix) override;

    QString getVoucherNumberByLedID(QString LedID,QDate Date) override;

//    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom,QDate dateTo);



    QSqlQueryModel *getVoucherListByDateQModel(QDate dateFrom,QDate dateTo,
                                               int empID=0,int plistID=0,
                                               QString route = "%", QString allowdStatus="",
                                               QString xmptList="",QString voucherPrefix="%");

//    QList<GeneralVoucherDataObject> getVoucherListByDatewithStatus(QDate dateFrom,QDate dateTo,int status);

//    QList<GeneralVoucherDataObject> getVoucherListByDatewithStatusEqual(QDate dateFrom, QDate dateTo, int status);

//    QList<GeneralVoucherDataObject> getVoucherListByDateandStatusList(QDate dateFrom,QDate dateTo,QList<int> statusList);

    int updateSalesOrderStatus(GeneralVoucherDataObject *voucher);

    void setItemVoucherStatus(float qty,QString id);

    void setAllItemVoucherStatusAsQStatus(GeneralVoucherDataObject *voucher,int status);

    void resetItemVoucherStatus(float qty,QString id);

    void setSerialNumberOnSo(QString serial,qint64 reqID);

    void setItemProductionStatus(int status,QString reqID);

    void setSalesOrderStatus(int status,QString qNo,QString voucherPrefix);

    void switchSalesMan(int newEmpID,QString vNO,QString pref,QDateTime dTime=QDateTime::currentDateTime());

    float getMaxAllowedQty(qint64 requirementItemId);

    QPair<QString, QString> getVoucherNumberByRef(QString ref);

    int getMaxIDwithPrefix(QString prefix);

    int getNextKotNumber();

    int getNextTakeWayNumber();

    void updateReference(QString vNo,QString ref);

    void updateLedger(QString ledID,QString ledName,QString vNo,QString vPrefix);

    int deleteSalesOrderItemByListID(int id);

    int getInventoryItemsCount(QString vNo);

    void truncateKotNumberTable();
    
    int getNewKotNumber();

    int getSOPending();

    QSqlQueryModel *getVoucherListByDateGModel(QDateTime dateFrom, QDateTime dateTo);
    GeneralVoucherDataObject *getPendingSalesOrders();
    QSqlQueryModel *getPendingSalesOrdersByDeliveryDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getItemwisePendingOrdersByDeliveryDate(QDate fromDate, QDate toDate, QString time = "%");

    QSqlQueryModel *getEmployeesBookedBetween(QDateTime fromDate,QDateTime toDate,QString EmployeeGroupID="%");

    QSqlQueryModel *getEmployeeBookedSlotsByDate(QString EmployeeID,QDateTime fromDate,QDateTime toDate);



    // VoucherDBAbstract interface
public:
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override;
    QSqlQueryModel *getItemwisePendingOrders(QDate fromDate, QDate toDate);
    QSqlQueryModel *getVouchersForDelivery(QDate dateFrom, QDate dateTo, int empID, int plistID, QString route, QString allowdStatus,
                                           QString xmptList, QString time,int status);
    QSqlQueryModel *getVouchersForBooking(QDate dateFrom, QDate dateTo, int empID, int plistID, QString route, QString allowdStatus,
                                           QString xmptList, QString time,int status);

    void updateSalesMan(int empID, QString vNo, QString vPrefix);
    void updateKOTPrintStatus(QString voucherNo, QString voucherPrefix, bool status);

};

#endif // SALESORDERDATABASEHELPER_H
