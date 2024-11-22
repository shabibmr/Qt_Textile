#ifndef STOCKREQUESTDATABASEHELPER_H
#define STOCKREQUESTDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"
#include "database/inventory/godown/godowndatabasehelper.h"

#include "database/voucherdbabstract.h"
class StockRequestDatabaseHelper : public VoucherDBAbstract
{
public:
    StockRequestDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~StockRequestDatabaseHelper() override;

    DatabaseHelper* db;

    QString Stock_Request_Main_TableName = "Stock_Request_Main";

    QString Stock_Request_Main_id = "_id";
    QString Stock_Request_Main_Voucher_No = "Voucher_No";
    QString Stock_Request_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Stock_Request_Main_Date = "Voucher_Date";
    QString Stock_Request_Main_Kitchen_Area = "Kitchen_Area";
    QString Stock_Request_Main_TimeStamp = "Time_Stamp";
    QString Stock_Request_Main_TimeStampC = "Time_Stamp";

    QString Stock_Request_Main_Last_Updated = "Last_Updated";

    QString Stock_Request_Main_Added_By = "Added_By";
    QString Stock_Request_Main_Narration = "Narration";

    QString Stock_Request_Main_FromLocation = "From_Location";
    QString Stock_Request_Main_ToLocation = "To_Location";
    QString Stock_Request_Main_VoucherStatus = "VoucherStatus";
    QString Stock_Request_Main_TransactionId = "TransactionId";
    QString Stock_Request_Main_Salesman_ID ="Salesman_ID";

    QString Stock_Request_Details_Table_Name = "Stock_Request_Details";

    QString Stock_Request_Details_id = "_id";
    QString Stock_Request_Details_Voucher_No = "Voucher_No";
    QString Stock_Request_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Stock_Request_Details_WorkOrderId = "WorkOrder_Id";
    QString Stock_Request_Details_Item_ID = "Item_ID";
    QString Stock_Request_Details_UOM_id = "UOM_ID";
    QString Stock_Request_Details_Calculated_Qty = "Calulated_Qty";

    QString Stock_Request_Details_Request_Qty = "Request_Qty";
    QString Stock_Request_Details_Issue_Qty  = "Issue_Qty";
    QString Stock_Request_Details_ItemReqId = "Requirement_ItemID";
    QString Stock_Request_Details_TimeStamp = "Time_Stamp";

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::IndentRequest); //"Stock Request";
    bool flag = true;

    ItemTransactionDatabaseHelper *itemTransaction;

    int startTransaction();

    int commit();
    int insertVoucher(GeneralVoucherDataObject *obj) override;
    int updateVoucher(GeneralVoucherDataObject *obj,QString id) override;
    int deleteVoucher(GeneralVoucherDataObject *obj) override;


    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;
    GeneralVoucherDataObject *getVoucherByWorkOrderId(QString WoNo);

    QSqlQueryModel *getVoucherListByDateModel(QDate fromDate, QDate toDate) override;

    int getMaxID();
    QString getNextVoucherNo(QString voucherPrefix) override;


    QSqlQueryModel* getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;
    QString getVoucherNumberByLedID(QString LedID,QDate Date) override;
};

#endif // STOCKREQUESTDATABASEHELPER_H
