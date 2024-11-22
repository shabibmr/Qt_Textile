
#ifndef BUNDLEDATABASEHELPER_H
#define BUNDLEDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
class BundleDatabaseHelper
{
public:
    BundleDatabaseHelper();
    ~BundleDatabaseHelper(){}
//    DatabaseHelper *db;
    QString Bundle_Master_TableName="Bundle_Master";
    QString Bundle_Master_id = "_id";
    QString Bundle_Master_Base_Item = "Base_Item";
    QString Bundle_Master_Free_Item = "Free_Item";
    QString Bundle_Master_Free_Quantity = "Free_Quantity";

//    "CREATE TABLE `items_bundle_master` ( `_id` INT NOT NULL AUTO_INCREMENT , `Base_Item` VARCHAR(200) NOT NULL , `Free_Item` VARCHAR(200) NOT NULL , `Free_Quantity` FLOAT(10,3) NOT NULL , `Free_UOM` INT NOT NULL , PRIMARY KEY (`_id`)) ENGINE = InnoDB;"
//    "CREATE TABLE `items_bundle_transaction` ( `_id` INT NOT NULL AUTO_INCREMENT , `Ledger_ID` VARCHAR(200) NULL , `Contact_ID` VARCHAR(200) NULL , `Voucher_Type` VARCHAR(100) NULL , `Voucher_Date` DATETIME NULL , `Voucher_No` VARCHAR(200) NULL , `Voucher_Prefix` VARCHAR(50) NULL , `Transaction_ID` VARCHAR(100) NULL , `Ref_Transaction_ID` VARCHAR(100) NULL , `Item_ID` VARCHAR(200) NULL , `Free_Quantity` FLOAT(10,3) NULL DEFAULT '0' , `Used_Quantity` FLOAT(10,3) NULL DEFAULT '0' , PRIMARY KEY (`_id`)) ENGINE = InnoDB;"

    QString Bundle_Transaction_TableName="Bundle_Transaction";
    QString Bundle_Transaction_id="_id";
    QString Bundle_Transaction_Ledger_ID="Ledger_ID";
    QString Bundle_Transaction_Contact_ID="Contact_ID";
    QString Bundle_Transaction_VType="Voucher_Type";
    QString Bundle_Transaction_VPref="Voucher_Prefix";
    QString Bundle_Transaction_VNo="Voucher_No";
    QString Bundle_Transaction_VDate="Voucher_Date";
    QString Bundle_Transaction_TransactionID="Transaction_ID";
    QString Bundle_Transaction_RefTransID="Ref_Transaction_ID";
    QString Bundle_Transaction_Item_ID="Item_ID";
    QString Bundle_Transaction_Free_Quantity="Free_Quantity";
    QString Bundle_Transaction_Used_Quantity="Used_Quantity";




    void insertMaster(CompoundItemDataObject *item);
    void deleteMaster(QString itemID);

    QList<inventoryItemDataModel> getBundledItemsMaster(QString itemID);


    // For Base Product
    void insertBundleTransaction(GeneralVoucherDataObject *voucher);
    void deleteBundleTransaction(GeneralVoucherDataObject *voucher);

    QSqlQueryModel* getPendingFreeListByContact(QString contactID,QDate asOfDate,QString voucherDate);
    QSqlQueryModel* getPendingFreeListByLedger(QString LedgerID,QDate asOfDate,QString voucherDate);


};

#endif // BUNDLEDATABASEHELPER_H
