#ifndef ITEMWISESALESORDERDATABASEHELPER_H
#define ITEMWISESALESORDERDATABASEHELPER_H

#include "salesorderdatabasehelper.h"
#include "database/databasehelper/databasehelper.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "customwidgets/Models/gmsqlquerymodel.h"

class ItemwiseSalesOrderDatabaseHelper
{

public:
    ItemwiseSalesOrderDatabaseHelper(QString mainTableName,QString detailsTable);

    ~ItemwiseSalesOrderDatabaseHelper();


    static QString Order_Itemwise_TableName ;
    static QString Purchase_Order_Itemwise_TableName ;

    static QString main_TableName;
    static QString details_TableName;

    static QString Sales_Order_Itemwise_TableName ;
    static QString Sales_Order_Itemwise_id;
    static QString Sales_Order_Itemwise_Voucher_No;
    static QString Sales_Order_Itemwise_Voucher_Prefix;
    static QString Sales_Order_Itemwise_Date_Created ;
    static QString Sales_Order_Itemwise_Voucher_Date ;
    static QString Sales_Order_Itemwise_TimeStamp ;
    static QString Sales_Order_Itemwise_SalesmanID  ;
    static QString Sales_Order_Itemwise_Item_ID ;
    static QString Sales_Order_Itemwise_Qty_Total;
    static QString Sales_Order_Itemwise_Narration ;
    static QString Sales_Order_Itemwise_Added_By;
    static QString Sales_Order_Itemwise_Damage;
    static QString Sales_Order_Itemwise_Total_Amount;
    static QString Sales_Order_Itemwise_Voucher_Status;

    bool flag =true;
    DatabaseHelper *db;

    void startTransaction();

    int commit();

    void insertOrder(GeneralVoucherDataObject *voucher);

    void updateOrder(GeneralVoucherDataObject *voucher,QString oldID);

    void deleteOrder(QString oldID);

    QSqlQueryModel *getiSalesOrdersbetweenDates(QDate fromDate,QDate toDate);

    GMSQLQUeryModel *getiiSalesOrdersbetweenDates(QDate fromDate,QDate toDate);

    QSqlQueryModel *getDatesOfPurchaseByItem(QString itemID,QDate salesDate);

    QSqlQueryModel *getSOitemsByPDate(QDate purchDate);
    QSqlQueryModel *getSOitemsBySDate(QDate salesDate);
    QSqlQueryModel *getItemDistinctDatesSO(QString itemID,QDate salesDate);
    QSqlQueryModel *getItemSOPendingReportByitem(QString itemID,QDate salesDate,QDate PurchDate);

    QSqlQueryModel *getPurchaseQuantitiesBYITEMDATE(QString itemID,QDate pDate);
    QSqlQueryModel *getSalesOrderReportByPO(QString itemID, QDate salesDate);


    GeneralVoucherDataObject *getVoucherByVoucherNoAsPtr(QString vNo,QString vPrefix);

    QStringList getVoucherswithOrderID(QString vID);


    int getMaxID();

    static void setSales_Order_Itemwise_TableName(const QString &value);
    static void setOrder_Itemwise_TableName(const QString &value);
    static void setMain_TableName(const QString &value);
    static void setDetails_TableName(const QString &value);
};

#endif // ITEMWISESALESORDERDATABASEHELPER_H
