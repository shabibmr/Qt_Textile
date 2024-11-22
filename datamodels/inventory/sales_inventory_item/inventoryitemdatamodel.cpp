#include "inventoryitemdatamodel.h"

inventoryItemDataModel::inventoryItemDataModel()
{
    OpeningStockDate = QDate::fromString("2000-01-01","yyyy-MM-dd");
    lastModified = QDate::fromString("2000-01-01","yyyy-MM-dd");
    manufactureDate = QDate::fromString("2000-01-01","yyyy-MM-dd");
    dateCreated = QDate::fromString("2000-01-01","yyyy-MM-dd");
    fromTime = QTime::fromString("00:00:00", "HH:mm:ss");
    toTime =  QTime::fromString("00:00:00", "HH:mm:ss");
    // measurement = nullptr;
}



inventoryItemDataModel::~inventoryItemDataModel()
{


}

QJsonObject inventoryItemDataModel::objToJson(inventoryItemDataModel obj)
{

    QJsonObject jsonObject;

//    jsonObject.insert("ItemName",obj.ItemName);
//    jsonObject.insert("itemNameArabic",obj.itemNameArabic);
//    jsonObject.insert("GroupName",obj.GroupName);
//    jsonObject.insert("GroupID",obj.GroupID);
//    jsonObject.insert("ItemID",obj.ItemID);
//    jsonObject.insert("ItemAlias",obj.ItemAlias);
//    jsonObject.insert("ItemCode",obj.ItemCode);
//    jsonObject.insert("narration",obj.narration);
//    jsonObject.insert("stdCost",obj.stdCost);
//    jsonObject.insert("Dimension",obj.Dimension);
//    jsonObject.insert("price",obj.price);
//    jsonObject.insert("quantity",obj.quantity);
//    jsonObject.insert("discount",obj.discount);
//    jsonObject.insert("discountinAmount",obj.discountinAmount);
//    jsonObject.insert("discountPercentage",obj.discountPercentage);
//    jsonObject.insert("subTotal",obj.subTotal);
//    jsonObject.insert("grandTotal",obj.grandTotal);
//    jsonObject.insert("requirementItemId",obj.requirementItemIdOld);
//    jsonObject.insert("maxQuantity",obj.maxQuantity);
//    jsonObject.insert("listId",obj.listId);
//    jsonObject.insert("fromGodownID",obj.fromGodownID);
//    jsonObject.insert("toGodownID",obj.toGodownID);
//    jsonObject.insert("PriceLevel",obj.PriceLevel);
//    jsonObject.insert("PartNumber",obj.PartNumber);
//    jsonObject.insert("SerailNumber",obj.SerailNumber);
//    jsonObject.insert("OpeningStock",obj.OpeningStock);
//    jsonObject.insert("ClosingStock",obj.ClosingStock);
//    jsonObject.insert("OpeningStockDate",obj.OpeningStockDate.toString());
//    jsonObject.insert("OpeningStockValue",obj.OpeningStockValue);
//    jsonObject.insert("OpeningStockPrice",obj.OpeningStockPrice);
//    jsonObject.insert("ReorderLevel",obj.ReorderLevel);
//    jsonObject.insert("defaultSalesLedgerID",obj.defaultSalesLedgerID);
//    jsonObject.insert("defaultPurchaseLedgerID",obj.defaultPurchaseLedgerID);
//    jsonObject.insert("brandID",obj.brandID);
//    jsonObject.insert("brandName",obj.brandName);
//    jsonObject.insert("vatRate",obj.vatRate);
//    jsonObject.insert("vatAmount",obj.vatAmount);
//    QJsonArray propertyIDsJArray;
//    for(QString a:propertyIDsList){
//        propertyIDsJArray.append(a);
//    }
//    jsonObject.insert("propertyIDsList",propertyIDsJArray);
//    QJsonArray subItemsJArray;
//    for(QString a:subItemsIDsList){
//        subItemsJArray.append(a);
//    }

//    jsonObject.insert("subItemsIDsList",subItemsJArray);
//    jsonObject.insert("defaultUOMID",obj.defaultUOMID);

//    UomDataModel Uomobj;
//    jsonObject.insert("uomObject",Uomobj.ObjToJson(obj.uomObject));
//    jsonObject.insert("priceListID",obj.priceListID);
//    jsonObject.insert("priceListName",obj.priceListName);
//    QJsonArray imageIdJArray;
//    for(QString a:ImageIDS){
//        imageIdJArray.append(a);
//    }
//    jsonObject.insert("ImageIDS",imageIdJArray);
//    jsonObject.insert("customItem",obj.customItem);
//    jsonObject.insert("lastModified",obj.lastModified.toString());
//    jsonObject.insert("dateCreated",obj.dateCreated.toString());
//    jsonObject.insert("fromTime",obj.fromTime.toString());
//    jsonObject.insert("toTime",obj.toTime.toString());
//    jsonObject.insert("CreatedBy",obj.CreatedBy);
//    QJsonArray basePropsJArray;
//    PropertyDataModel p;
//    for(PropertyDataModel prop:obj.baseProperties){
//        basePropsJArray.append(p.ObjToJson(prop));
//    }
//    jsonObject.insert("baseProperties",basePropsJArray);
//    jsonObject.insert("ItemDescription",obj.ItemDescription);
//    jsonObject.insert("length",obj.length);
//    jsonObject.insert("warrantyDays",obj.warrantyDays);
//    jsonObject.insert("shelfLife",obj.shelfLife);
//    jsonObject.insert("IsCompoundItem",obj.IsCompoundItem);
//    jsonObject.insert("isCustomItem",obj.isCustomItem);
//    jsonObject.insert("isPurchaseItem",obj.isPurchaseItem);
//    jsonObject.insert("isSalesItem",obj.isSalesItem);
//    jsonObject.insert("favo",obj.favo);
//    jsonObject.insert("KOTPrinter",obj.KOTPrinter);
//    jsonObject.insert("itemProductionStatus",obj.itemProductionStatus);
//    jsonObject.insert("DefaultInputTaxLedgerID",obj.DefaultInputTaxLedgerID);
//    jsonObject.insert("DefaultOutputTaxLedgerID",obj.DefaultOutputTaxLedgerID);
//    jsonObject.insert("DefaultSalesReturnLedgerID",obj.DefaultSalesReturnLedgerID);
//    jsonObject.insert("DefaultPurchaseReturnLedgerID",obj.DefaultPurchaseReturnLedgerID);
//    jsonObject.insert("TechnicianID",obj.TechnicianID);
//    jsonObject.insert("drQty",obj.drQty);
//    jsonObject.insert("crQty",obj.crQty);
//    jsonObject.insert("HSN_CODE",obj.hsnCode);

    //###########
    jsonObject.insert("Item_Name",obj.ItemName);
    jsonObject.insert("Item_Name_Arabic",obj.itemNameArabic);
    jsonObject.insert("GroupName",obj.GroupName);
    jsonObject.insert("Group_Id",obj.GroupID);
    jsonObject.insert("Item_ID",obj.ItemID);
    jsonObject.insert("Item_Alias",obj.ItemAlias);
    jsonObject.insert("Item_Code",obj.ItemCode);
    jsonObject.insert("Narration",obj.narration);
    jsonObject.insert("Std_Cost",obj.stdCost);
//    jsonObject.insert("Dimension",obj.Dimension);
    jsonObject.insert("Price",obj.price);
    jsonObject.insert("Price2",obj.price_2);
    jsonObject.insert("Quantity",obj.quantity);
    jsonObject.insert("discount",obj.discount);
    jsonObject.insert("discountinAmount",obj.discountinAmount);
    jsonObject.insert("discountPercentage",obj.discountPercentage);
    jsonObject.insert("subTotal",obj.subTotal);
    jsonObject.insert("grandTotal",obj.grandTotal);
    jsonObject.insert("Requirement_ItemID",obj.ItemReqUuid);
    jsonObject.insert("maxQuantity",obj.maxQuantity);
    jsonObject.insert("listId",obj.listId);
    jsonObject.insert("fromGodownID",obj.fromGodownID);
    jsonObject.insert("toGodownID",obj.toGodownID);
    jsonObject.insert("PriceLevel",obj.PriceLevel);
    jsonObject.insert("PartNumber",obj.PartNumber);
    jsonObject.insert("SerailNumber",obj.SerailNumber);
    jsonObject.insert("Opening_Stock",obj.OpeningStock);
    jsonObject.insert("Closing_Stock",obj.ClosingStock);
    jsonObject.insert("OpeningStockDate",obj.OpeningStockDate.toString());
    jsonObject.insert("OpeningStockValue",obj.OpeningStockValue);
    jsonObject.insert("OpeningStockPrice",obj.OpeningStockPrice);
    jsonObject.insert("Reorder_Level",obj.ReorderLevel);
    jsonObject.insert("Default_Sales_Ledger_id",obj.defaultSalesLedgerID);
    jsonObject.insert("Default_PurchaseLedger_id",obj.defaultPurchaseLedgerID);
    jsonObject.insert("Brand_Id",obj.brandID);
    jsonObject.insert("brandName",obj.brandName);
    jsonObject.insert("Vat_Rate",obj.taxRate);
    jsonObject.insert("vatAmount",obj.taxAmount);
//    QJsonArray propertyIDsJArray;
//    for(QString a:propertyIDsList){
//        propertyIDsJArray.append(a);
//    }
//    jsonObject.insert("propertyIDsList",propertyIDsJArray);
//    QJsonArray subItemsJArray;
//    for(QString a:subItemsIDsList){
//        subItemsJArray.append(a);
//    }

//    jsonObject.insert("subItemsIDsList",subItemsJArray);
    jsonObject.insert("defaultUOMID",obj.defaultUOMID);

    UomDataModel Uomobj;
    qDebug()<<"172 uom"<<obj.uomObject.UomName;
    jsonObject.insert("uomObject",Uomobj.ObjToJson(obj.uomObject));
    jsonObject.insert("priceListID",obj.priceListID);
    jsonObject.insert("priceListName",obj.priceListName);
//    QJsonArray imageIdJArray;
//    for(QString a:ImageIDS){
//        imageIdJArray.append(a);
//    }
//    jsonObject.insert("ImageIDS",imageIdJArray);
    jsonObject.insert("customItem",obj.removeItem);
    jsonObject.insert("Last_Modified",obj.lastModified.toString());
    jsonObject.insert("Date_Created",obj.dateCreated.toString());
    jsonObject.insert("fromTime",obj.fromTime.toString());
    jsonObject.insert("toTime",obj.toTime.toString());
    jsonObject.insert("CreatedBy",obj.CreatedBy);
//    QJsonArray basePropsJArray;
//    PropertyDataModel p;
//    for(PropertyDataModel prop:obj.baseProperties){
//        basePropsJArray.append(p.ObjToJson(prop));
//    }
//    jsonObject.insert("baseProperties",basePropsJArray);
    jsonObject.insert("Item_Description",obj.ItemDescription);
    jsonObject.insert("length",obj.length);
    jsonObject.insert("Warranty_Days",obj.warrantyDays);
    jsonObject.insert("Shelf_Life",obj.shelfLife);
    jsonObject.insert("IsCompoundItem",obj.IsCompoundItem);
    jsonObject.insert("isCustomItem",obj.isCustomItem);
    jsonObject.insert("isPurchaseItem",obj.isPurchaseItem);
    jsonObject.insert("isSalesItem",obj.isSalesItem);
    jsonObject.insert("isStockItem",obj.isStockItem);
    jsonObject.insert("Favourite",obj.favo);
    jsonObject.insert("KOT_Printer",obj.KOTPrinter);
    jsonObject.insert("itemProductionStatus",obj.itemProductionStatus);
    jsonObject.insert("Default_Input_Tax_Ledger",obj.DefaultInputTaxLedgerID);
    jsonObject.insert("Default_Output_Tax_Ledger",obj.DefaultOutputTaxLedgerID);
    jsonObject.insert("Default_Sales_Return_Ledger",obj.DefaultSalesReturnLedgerID);
    jsonObject.insert("Default_Purchase_Return_Ledger",obj.DefaultPurchaseReturnLedgerID);
    jsonObject.insert("TechnicianID",obj.TechnicianID);
    jsonObject.insert("drQty",obj.drQty);
    jsonObject.insert("crQty",obj.crQty);
    jsonObject.insert("HSN_CODE",obj.hsnCode);
    jsonObject.insert("Section", obj.section);
    jsonObject.insert("Category", obj.Category.toInt());

    RestaurantIngredientsDataModel i;
    QJsonArray bomJArray;
    for(RestaurantIngredientsDataModel bom:obj.bomList){
        bomJArray.append(i.objToJson(bom));
    }
    jsonObject.insert("bomList",bomJArray);

    return jsonObject;

}

inventoryItemDataModel inventoryItemDataModel::JsonToObj(QJsonObject json)
{
    inventoryItemDataModel obj;

    obj.ItemName = json["Item_Name"].toString();
    obj.itemNameArabic = json["Item_Name_Arabic"].toString();
    obj.GroupName = json["GroupName"].toString();
    obj.GroupID = json["Group_Id"].toString();
    obj.ItemID = json["Item_ID"].toString();
    obj.ItemAlias = json["Item_Alias"].toString();
    obj.ItemCode = json["Item_Code"].toString();
    obj.narration = json["Narration"].toString();
    obj.stdCost = json["Std_Cost"].toVariant().toFloat();
    obj.Dimension = json["Dimension"].toString();
    obj.price = json["Price"].toVariant().toFloat();
    obj.quantity = json["Quantity"].toVariant().toFloat();
    obj.discount = json["discount"].toVariant().toFloat();
    obj.discountinAmount = json["discountinAmount"].toVariant().toFloat();
    obj.discountPercentage = json["discountPercentage"].toVariant().toFloat();
    obj.subTotal = json["subTotal"].toVariant().toFloat();
    obj.grandTotal = json["grandTotal"].toVariant().toFloat();
    obj.requirementItemIdOld = json["requirementItemId"].toVariant().toLongLong();
    obj.maxQuantity = json["maxQuantity"].toVariant().toFloat();
    obj.listId = json["listId"].toInt();
    obj.fromGodownID = json["fromGodownID"].toString();
    obj.toGodownID = json["toGodownID"].toString();
    obj.PriceLevel = json["PriceLevel"].toString();
    obj.PartNumber = json["PartNumber"].toString();
    obj.SerailNumber = json["SerailNumber"].toString();
    obj.OpeningStock = json["Opening_Stock"].toVariant().toFloat();
    obj.ClosingStock = json["ClosingStock"].toVariant().toFloat();
    obj.OpeningStockDate = json["Closing_Stock"].toVariant().toDate();
    obj.OpeningStockValue = json["OpeningStockValue"].toVariant().toFloat();
    obj.OpeningStockPrice = json["OpeningStockPrice"].toVariant().toFloat();
    obj.ReorderLevel = json["Reorder_Level"].toVariant().toFloat();
    obj.defaultSalesLedgerID = json["Default_Sales_Ledger_id"].toString();
    obj.defaultPurchaseLedgerID = json["Default_PurchaseLedger_id"].toString();
    obj.brandID = json["Brand_Id"].toInt();
    obj.brandName = json["brandName"].toInt();
    obj.taxRate = json["Vat_Rate"].toVariant().toFloat();
    obj.taxAmount = json["vatAmount"].toVariant().toFloat();
    QJsonArray propertyIDsJArray = json["propertyIDsList"].toArray();
    for(QJsonValue propertyId:propertyIDsJArray){
        obj.propertyIDsList.append(propertyId.toString());
    }
    QJsonArray subItemsJArray = json["subItemsIDsList"].toArray();
    for(QJsonValue subItem:subItemsJArray){
        obj.subItemsIDsList.append(subItem.toString());
    }
    obj.defaultUOMID = json["defaultUOMID"].toString();
    UomDataModel Uomobj;
    obj.uomObject = Uomobj.JsonToObj(json["uomObject"].toObject());
    obj.priceListID = json["priceListID"].toString();
    obj.priceListName = json["priceListName"].toString();
    QJsonArray imageIdJArray = json["ImageIDS"].toArray();
    for(QJsonValue imageId:imageIdJArray){
        obj.ImageIDS.append(imageId.toString());
    }
    obj.removeItem = json["customItem"].toBool();
    obj.lastModified = json["Last_Modified"].toVariant().toDate();
    obj.dateCreated = json["Date_Created"].toVariant().toDate();
    obj.fromTime = json["fromTime"].toVariant().toTime();
    obj.toTime = json["toTime"].toVariant().toTime();
    obj.CreatedBy = json["CreatedBy"].toString();
    QJsonArray basePropsJArray= json["baseProperties"].toArray();
    PropertyDataModel p;
    for(QJsonValue prop: basePropsJArray){
       obj.baseProperties.append(p.JsonToObj(prop.toObject())) ;
    }
    obj.ItemDescription = json["Item_Description"].toString();
    obj.length = json["length"].toVariant().toFloat();
    obj.warrantyDays = json["Warranty_Days"].toInt();
    obj.shelfLife = json["Shelf_Life"].toVariant().toFloat();
    obj.IsCompoundItem = json["IsCompoundItem"].toBool();
    obj.isCustomItem = json["isCustomItem"].toBool();
    obj.isPurchaseItem = json["isPurchaseItem"].toBool();
    obj.isSalesItem = json["isSalesItem"].toBool();
    obj.isStockItem = json["isStockItem"].toBool();
    obj.favo = json["Favourite"].toBool();
    obj.KOTPrinter = json["KOT_Printer"].toString();
    obj.itemProductionStatus = json["itemProductionStatus"].toInt();
    obj.DefaultInputTaxLedgerID = json["Default_Input_Tax_Ledger"].toString();
    obj.DefaultOutputTaxLedgerID = json["Default_Output_Tax_Ledger"].toString();
    obj.DefaultSalesReturnLedgerID = json["Default_Sales_Return_Ledger"].toString();
    obj.DefaultPurchaseReturnLedgerID = json["Default_Purchase_Return_Ledger"].toString();
    obj.TechnicianID = json["TechnicianID"].toInt();
    obj.drQty = json["drQty"].toVariant().toFloat();
    obj.crQty = json["crQty"].toVariant().toFloat();
    obj.hsnCode = json["HSN_CODE"].toString();
    obj.section = json["Section"].toString();
    obj.Category = json["Category"].toString();

    RestaurantIngredientsDataModel i;

    QJsonArray bomJArray = json["bomList"].toArray();
    for(QJsonValue bomItem:bomJArray){
        obj.bomList.append(i.JsonToObj(bomItem.toObject()));
    }

    return obj;
}

inventoryItemDataModel *inventoryItemDataModel::JsonToObjPtr(QJsonObject json)
{
    inventoryItemDataModel *obj = new inventoryItemDataModel();

    obj->ItemName = json["Item_Name"].toString();
    obj->itemNameArabic = json["Item_Name_Arabic"].toString();
    obj->GroupName = json["GroupName"].toString();
    obj->GroupID = json["Group_Id"].toString();
    obj->ItemID = json["Item_ID"].toString();
    obj->ItemAlias = json["Item_Alias"].toString();
    obj->ItemCode = json["Item_Code"].toString();
    obj->narration = json["Narration"].toString();
    obj->stdCost = json["Std_Cost"].toVariant().toFloat();
    obj->Dimension = json["Dimension"].toString();
    obj->price = json["Price"].toVariant().toFloat();
    obj->price_2 = json["Price2"
                        ""].toVariant().toFloat();
    obj->quantity = json["quantity"].toVariant().toFloat();
    obj->discount = json["discount"].toVariant().toFloat();
    obj->discountinAmount = json["discountinAmount"].toVariant().toFloat();
    obj->discountPercentage = json["discountPercentage"].toVariant().toFloat();
    obj->subTotal = json["subTotal"].toVariant().toFloat();
    obj->grandTotal = json["grandTotal"].toVariant().toFloat();
    obj->requirementItemIdOld = json["requirementItemId"].toVariant().toLongLong();
    obj->maxQuantity = json["maxQuantity"].toVariant().toFloat();
    obj->listId = json["listId"].toInt();
    obj->fromGodownID = json["fromGodownID"].toString();
    obj->toGodownID = json["toGodownID"].toString();
    obj->PriceLevel = json["PriceLevel"].toString();
    obj->PartNumber = json["PartNumber"].toString();
    obj->SerailNumber = json["SerailNumber"].toString();
    obj->OpeningStock = json["Opening_Stock"].toVariant().toFloat();
    obj->ClosingStock = json["ClosingStock"].toVariant().toFloat();
    obj->OpeningStockDate = json["Closing_Stock"].toVariant().toDate();
    obj->OpeningStockValue = json["OpeningStockValue"].toVariant().toFloat();
    obj->OpeningStockPrice = json["OpeningStockPrice"].toVariant().toFloat();
    obj->ReorderLevel = json["Reorder_Level"].toVariant().toFloat();
    obj->defaultSalesLedgerID = json["Default_Sales_Ledger_id"].toString();
    obj->defaultPurchaseLedgerID = json["Default_PurchaseLedger_id"].toString();
    obj->brandID = json["Brand_Id"].toInt();
    obj->brandName = json["brandName"].toString();
    obj->taxRate = json["Vat_Rate"].toVariant().toFloat();
    obj->taxAmount = json["vatAmount"].toVariant().toFloat();
    QJsonArray propertyIDsJArray = json["propertyIDsList"].toArray();
    for(QJsonValue propertyId:propertyIDsJArray){
        obj->propertyIDsList.append(propertyId.toString());
    }
    QJsonArray subItemsJArray = json["subItemsIDsList"].toArray();
    for(QJsonValue subItem:subItemsJArray){
        obj->subItemsIDsList.append(subItem.toString());
    }
    obj->defaultUOMID = json["defaultUOMID"].toString();
    UomDataModel Uomobj;
    obj->uomObject = Uomobj.JsonToObj(json["uomObject"].toObject());
    obj->priceListID = json["priceListID"].toString();
    obj->priceListName = json["priceListName"].toString();
    QJsonArray imageIdJArray = json["ImageIDS"].toArray();
    for(QJsonValue imageId:imageIdJArray){
        obj->ImageIDS.append(imageId.toString());
    }
    obj->removeItem = json["customItem"].toBool();
    obj->lastModified = json["Last_Modified"].toVariant().toDate();
    obj->dateCreated = json["Date_Created"].toVariant().toDate();
    obj->fromTime = json["fromTime"].toVariant().toTime();
    obj->toTime = json["toTime"].toVariant().toTime();
    obj->CreatedBy = json["CreatedBy"].toString();
    QJsonArray basePropsJArray= json["baseProperties"].toArray();
    PropertyDataModel p;
    for(QJsonValue prop: basePropsJArray){
       obj->baseProperties.append(p.JsonToObj(prop.toObject())) ;
    }
    obj->ItemDescription = json["Item_Description"].toString();
    obj->length = json["length"].toVariant().toFloat();
    obj->warrantyDays = json["Warranty_Days"].toInt();
    obj->shelfLife = json["Shelf_Life"].toVariant().toFloat();
    obj->IsCompoundItem = json["IsCompoundItem"].toBool();
    obj->isCustomItem = json["isCustomItem"].toBool();
    obj->isPurchaseItem = json["isPurchaseItem"].toBool();
    obj->isSalesItem = json["isSalesItem"].toBool();
    obj->isStockItem = json["isStockItem"].toBool();
    obj->favo = json["Favourite"].toBool();
    obj->KOTPrinter = json["KOT_Printer"].toString();
    obj->itemProductionStatus = json["itemProductionStatus"].toInt();
    obj->DefaultInputTaxLedgerID = json["Default_Input_Tax_Ledger"].toString();
    obj->DefaultOutputTaxLedgerID = json["Default_Output_Tax_Ledger"].toString();
    obj->DefaultSalesReturnLedgerID = json["Default_Sales_Return_Ledger"].toString();
    obj->DefaultPurchaseReturnLedgerID = json["Default_Purchase_Return_Ledger"].toString();
    obj->TechnicianID = json["TechnicianID"].toInt();
    obj->drQty = json["drQty"].toVariant().toFloat();
    obj->crQty = json["crQty"].toVariant().toFloat();
    obj->hsnCode = json["HSN_CODE"].toString();
    obj->section = json["Section"].toString();
    obj->Category = json["Category"].toString();

    RestaurantIngredientsDataModel i;

    QJsonArray bomJArray = json["bomList"].toArray();
    for(QJsonValue bomItem:bomJArray){
        obj->bomList.append(i.JsonToObj(bomItem.toObject()));
    }



    qDebug()<<"RETURN ID"<<obj->ItemID;
    return obj;
}
