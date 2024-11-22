#ifndef SALESINVENTORYITEMDATABASEHELPER_H
#define SALESINVENTORYITEMDATABASEHELPER_H

#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include "datamodels/inventory/sales_inventory_item/itemimagedatamodel.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

#include "database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h"
#include "database/inventory/property/propertydatabasehelper.h"
#include "database/inventory/brands/brandsdatabasehelper.h"
#include "database/inventory/sales_inventory_items/inventoryitemimagesdatabasehelper.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"

#include "database/inventory/uom/uomdatabasehelper.h"
#include "database/Notifications/sendchanneldatabasehelper.h"
#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"

//#include "database/Reports/closingstockhelper.h"

#include <QSqlQueryModel>

#include <QMap>
#include <QDebug>

class SalesInventoryItemDatabaseHelper
{
public:
    //Sales Items Columns
    static QString Sales_Inventory_Table_Name ;
    QString Sales_Inventory_Id ="_Id";
    QString Sales_Inventory_ItemId ="Item_Id";

    QString Sales_Inventory_ItemName = "Item_Name";
    QString Sales_Inventory_ItemName_Arabic = "Item_Name_Arabic";
    QString Sales_Inventory_ItemAlias = "Item_Alias";
    QString Sales_Inventory_ItemCode = "Item_Code";
    QString Sales_Inventory_PartNumber = "Part_Number";
    QString Sales_Inventory_GroupId = "Group_Id";

    QString Sales_Inventory_lastModifiedBy = "Last_Modified_By";
    QString Sales_Inventory_OpeningStock= "Opening_Stock";
    QString Sales_Inventory_OpeningBalanceDate = "Opening_Balance_Date";
    QString Sales_Inventory_OpeningRate = "Opening_Rate";
    QString Sales_Inventory_OpeningValue = "Opening_Value";
    QString Sales_Inventory_Price = "Price";
    QString Sales_Inventory_Price2 = "Price_2";

    QString Sales_Inventory_Narration = "Narration";
    QString Sales_Inventory_SerailNumber = "Serial_Number";
    QString Sales_Inventory_ClosingStock = "Closing_Stock";
    QString Sales_Inventory_ReorderLevel = "Reorder_Level";
    QString Sales_Inventory_StdCost= "Std_Cost";
    QString Sales_Inventory_Favourite = "Favourite";

    QString Sales_Inventory_defaultSalesLedgerID = "Default_Sales_Ledger_id";
    QString Sales_Inventory_defaultPurchaseLedgerID = "Default_PurchaseLedger_id";
    QString Sales_Inventory_defaultInputTaxLedgerID = "Default_Input_Tax_Ledger";
    QString Sales_Inventory_defaultOutputTaxLedgerID = "Default_Output_Tax_Ledger";
    QString Sales_Inventory_defaultSalesReturnLedgerID = "Default_Sales_Return_Ledger";
    QString Sales_Inventory_defaultPurchaseReturnLedgerID = "Default_Purchase_Return_Ledger";

    QString Sales_Inventory_VatRate = "Vat_Rate";

    QString Sales_Inventory_defaultUOM = "Default_UOM_ID";

    QString Sales_Inventory_lastModified = "Last_Modified";
    QString Sales_Inventory_dateCreated = "Date_Created";
    QString Sales_Inventory_timestamp = "Timestamp";

    QString Sales_Inventory_WarrantyDays = "Warranty_Days";
    QString Sales_Inventory_ShelfLife = "Shelf_Life";
    QString Sales_Inventory_BrandID ="Brand_Id";
    QString Sales_Inventory_Description = "Item_Description";
    QString Sales_Inventory_isCustomItem = "isCustomItem";
    QString Sales_Inventory_isPurchaseItem = "isPurchaseItem";
    QString Sales_Inventory_isSalesItem = "isSalesItem";
    QString Sales_Inventory_KOT_Printer = "KOT_Printer";


    QString Sales_Inventory_Dimension="Dimension";

    // 8-10-2021
    QString Sales_Inventory_Item_Weight = "Item_Weight";
    QString Sales_Inventory_Deleted = "Item_Deleted";
    QString Sales_Inventory_Hidden = "Item_Hidden";
    //----

    // 21-6-23
    QString Sales_Inventory_hideInBill = "Hide_in_Bill";


    QString Item_Properties_TableName = "Sales_Inventory_Item_Properties";
    QString Item_Properties_id = "_id";
    QString Item_Properties_PropertyID = "Property_ID";
    QString Item_Properties_Item_ID = "Item_ID";

    QString Item_Properties_DefaultValue = "Default_Value";


    QString Sales_Inventory_From_Time   = "From_Time";
    QString Sales_Inventory_To_Time     = "To_Time";

    QString Sales_Inventory_HSN_CODE = "HSN_CODE";
    QString Sales_Inventory_Section = "Section";
    QString Sales_Inventory_Flags = "Flags";

    QString Sales_Inventory_IsStockItem="IsStockItem";
    QString Sales_Inventory_Category="Category";
    QString Sales_Inventory_DefaultLedgerID = "DefaultLedgerID";

    QString Sales_Inventory_isBatchProcessed = "isBatchProcessed";
    QString Sales_Inventory_isSerailNumbered = "isSerailNumbered";
    QString Sales_Inventory_isBatched = "isBundle";

    QString Transaction_Item_Helper_TableName = "Transaction_Item_Helper";
    QString Transaction_Bom_Helper_TableName = "Transaction_Bom_Helper";
    QString Transaction_Item_Helper_id = "_id";
    QString Transaction_Item_Helper_ItemID = "ItemID";
    QString Transaction_Item_Helper_From_Ledger = "From_Ledger";
    QString Transaction_Item_Helper_To_Ledger = "To_Ledger";
    QString Transaction_Item_Helper_VoucherID = "VoucherID";
    QString Transaction_Item_Helper_VoucherPrefix = "VoucherPrefix";
    QString Transaction_Item_Helper_VoucherDate = "VoucherDate";
    QString Transaction_Item_Helper_VoucherType = "VoucherType";
    QString Transaction_Item_Helper_From_Location = "From_Location";
    QString Transaction_Item_Helper_To_Location = "To_Location";
    QString Transaction_Item_Helper_Cr_Qty = "Cr_Qty";
    QString Transaction_Item_Helper_Dr_Qty = "Dr_Qty";
    QString Transaction_Item_Helper_Cr_Amount = "Cr_Amount";
    QString Transaction_Item_Helper_Dr_Amount = "Dr_Amount";
    QString Transaction_Item_Helper_Narration = "Narration";
    QString Transaction_Item_Helper_Time = "Time";
    QString Transaction_Item_Helper_ItemReqId = "Requirement_ItemID";
    QString Transaction_Item_Helper_Price = "Price";
    QString Transaction_Item_Helper_Consumed = "Consumed";

    static QString SKU_MASTER_TABLENAME;

    QString SKU_id = "_id";
    QString SKU_ID = "SKU_ID";
    QString SKU_Name = "SKU_Name";
    QString SKU_ITEM_ID= "Item_Id";
    QString SKU_PROPERTY_ID = "SKU_Prop_ID";
    QString SKU_PROPRTY_VALUE= "Prop_Value";
    QString SKU_Description= "SKU_Descrption";
    QString SKU_Barcode= "SKU_Barcode";
    QString SKU_Uom= "SKU_Uom";



    int insertSKU(inventoryItemDataModel *item);
    int deleteSKU(inventoryItemDataModel *item);

    DatabaseHelper *db;

    SalesInventoryItemDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~SalesInventoryItemDatabaseHelper();
    bool flag = true;

    int startTransaction();

    int insertSalesInventoryItem(CompoundItemDataObject *obj,ItemImageDatamodel image);

    int updateSalesInventoryItem(CompoundItemDataObject *obj, QString oldiD,ItemImageDatamodel image);

    int deleteSalesInventoryItem(QString oldID);

    int commit();

    QString getMaxID();

    //    QList<inventoryItemDataModel> getALLInventoryObjects();  // delete



    QList<QSharedPointer<inventoryItemDataModel>> getALLInventoryObjectsAsPtr(int filter=0);

    QSqlQueryModel* getInventoryItemsQueryModel(int filter = 0);

    QSqlQueryModel *getInventoryItemsQueryModelForSearch(int filter = 0);

    QString getInventoryItemNameByID(QString id);

    bool barcodeExist(QString barcode);

    QList<PropertyDataModel> getAllPropertiesOfItem(QString itemID);

    inventoryItemDataModel getInventoryItemByID(QString id,
                                                bool withCsStk=true,
                                                QDate toDate = QDate::currentDate(),
                                                QString priceid =""
            );

    QString getItemIDBySKUID(QString SKUID);

    CompoundItemDataObject getInventoryItemByBarcode(QString barcode,bool isSerial=false,QDate toDate = QDate::currentDate());

    inventoryItemDataModel *getInventoryItemByIDasPtr(QString id, QDate toDate = QDate::currentDate(), QString dbName = "");

    QList<QSharedPointer<inventoryItemDataModel> > getALLInventoryObjectsByGroup(QString arg);

    QList<QSharedPointer<inventoryItemDataModel> > getALLInventoryObjectsFavourites();


    float getPrice(QString id);

    void updateKOTPrinter(QString itemID,QString kotprinter);

    void updateOpening(QString itemID,float openingStk,float openingRate,float openingValue);

    QSqlQueryModel *getInventoryItemsQueryModelForScale(int filter = 0);


    bool isSalesItemByItemId(QString itemID);
    void updatePrice(QString itemID, float Price);
    void updateGST(QString itemID, float taxRate);
    float getCostByItem(QString ItemId);
//    float getClsStkOfItemTillDate(QString ItemID, QDate dateTo);

    float getClosingStockofItemByDate(QString itemID, QDate date);
    bool SerialExist(QString serial);
    QString getMaxSerialNo();
    QString getMaxCode();

    void updateItemCost(QString itemID,float cost);
    float getItemStockByFifo(QString ItemID, QDate toDate);
    float getTaxRate(QString id);
    static float getLastPurchPrice(QString itemID, QDate vDate);

    QSqlQueryModel *getInventoryItemsByGroupModel(QString groupId);
    float getLastSalesPrice(QString itemID, QDate vDate);
    QString getInventoryGroupNameByItemID(QString id);

    bool checkItemExists(QString itemID);
    float getClsStkOfItemTillDate(QString ItemID, QDate dateTo, QString godownID = "%");

    bool updateItemIdInMasters(QString oldId, QString newId);
    bool updateItemName(QString itmName, QString itemID);
    bool updateItemIdInTrans(QString oldId, QString newId);
    bool deleteItemFromMasters(QString itemID);
    QString checkItemNameInRefDb(QString itemName, QString refDb);

    QString createItemID(QString groupID);

    QList<inventoryItemDataModel *> getMissingItemsFromRefDb(QString refDb, QString currDb);
    bool checkItemTransactionExists(QString itemID);
    QMap<int, QList<QPair<QString, QString> > > getDuplicateItems();
    QSqlQueryModel *getDuplicateItemsBetweenDbs(QString currDB, QString refDb);
    QSqlQueryModel *getSameItemIdForDiffItemsBetweenDbs(QString currDB, QString refDb);
    bool checkBarCodeExists(QString itemCode, QString itemID);
    void updateLastModified(QString itemID);
    QSqlQueryModel *getInventoryItemsAllFavos();
};

#endif // SALESINVENTORYITEMDATABASEHELPER_H
