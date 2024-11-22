#ifndef GENERALVOUCHERDATABASEHELPER_H
#define GENERALVOUCHERDATABASEHELPER_H

#include <QString>
#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/inventory/godown/godowndatabasehelper.h"

class GeneralVoucherDatabaseHelper
{
public:
    GeneralVoucherDatabaseHelper();

    static QString getNextVoucherNoByType(QString voucherType, QString voucherPrefix, QString tableName);
    static QString setNextVoucherNoByType(QString voucherType, QString voucherPrefix, QString tableName);

    static QString Voucher_No_Seq_Tablename;
    static QString Voucher_No_Seq_VoucherType;
    static QString Voucher_No_Seq_VoucherPrefix;
    static QString Voucher_No_Seq_VoucherNo ;
    static QString Voucher_No_seq_Voucher_Date;
    static QString Transaction_Id;
    static QString Requirement_Voucher_No;

    QString General_Voucher_ItemID = "Inventory_Item_Id";
    QString General_Voucher_Quantity = "Quantity";
    QString General_Voucher_Price = "Price";
    QString General_Voucher_ItemReqId = "Requirement_ItemId";
    QString General_Voucher_itemProductionStatus = "itemProductionStatus";
    QString General_Voucher_Section = "section";
    QString General_Voucher_LedgerId = "Ledger_ID";

//    QString Sales_Inventory_Item_TableName = "Sales_Inventory_Items";
    QString Sales_Inventory_Item_ItemId = "Item_Id";


    bool flag = true;
    DatabaseHelper *db;
    int startTransaction();
    int commit();
    int getMaxID();

    QPair<QString,QString> getVoucherNoByTransactionId(QString transId, QString tableName); //static ?

    GeneralVoucherDataObject *getVoucherToExportByItem(int sourceVoucherType, int destinationVoucherType, QString ItemReqId);
    float getPendingQtyToExportByItem(int sourceVoucherType, int targetVoucherType, QString ItemReqId);
    QList<GeneralVoucherDataObject *> getVoucherListToexportByType(int sourceVoucherType, int targetVoucherType, QMap<QString,QVariant> filterMap);
    QList<GeneralVoucherDataObject *> getVoucherListToexportByDate(QDateTime fromDate,QDateTime toDate,int sourceVoucherType, int targetVoucherType, QMap<QString,QVariant> filterMap);
    QMap<QString, QString> getPendingSectionToExportByType(int sourceVoucherType, int targetVoucherType);


    QPair<QString, QString> getVoucherNoByReqVoucherNo(QString transId, QString tableName);
};

#endif // GENERALVOUCHERDATABASEHELPER_H
