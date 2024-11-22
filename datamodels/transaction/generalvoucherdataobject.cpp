#include "generalvoucherdataobject.h"

GeneralVoucherDataObject::GeneralVoucherDataObject()
{
    // VoucherPrefix = LoginValues::voucherPrefix;
    VoucherDate = QDate::currentDate();
    invoiceDate =  QDate::currentDate();
    DateCreated =   QDate::currentDate();
    timestamp = QDateTime::currentDateTime();
    lastEditedDateTime = QDateTime::currentDateTime();
    DeliveryDate = QDateTime::currentDateTime();

    measurement = nullptr;

}

GeneralVoucherDataObject::~GeneralVoucherDataObject()
{

}

QJsonObject GeneralVoucherDataObject::ObjToJson(GeneralVoucherDataObject *obj)
{
    QJsonObject jsonObject;
    LedgerMasterDataModel ledObj;

    jsonObject.insert("_DisplayVoucherNumber",obj->_DisplayVoucherNumber);
    jsonObject.insert("Voucher_No",obj->voucherNumber);
    jsonObject.insert("Voucher_Date",obj->VoucherDate.toString());
    jsonObject.insert("Date_Created",obj->DateCreated.toString());
    jsonObject.insert("Voucher_Prefix",obj->VoucherPrefix);
    jsonObject.insert("invoiceNumber",obj->invoiceNumber);
    jsonObject.insert("invoiceDate",obj->invoiceDate.toString());
    jsonObject.insert("Timestamp",obj->timestamp.toString());
    jsonObject.insert("lastEditedDateTime",obj->lastEditedDateTime.toString());
    jsonObject.insert("ledgerObject",ledObj.ObjToJson(obj->ledgerObject));

    CompoundItemDataObject c;
    QJsonArray InventoryItemsJArray;
    for(CompoundItemDataObject item:obj->InventoryItems){

        InventoryItemsJArray.append(c.ObjToJson(item));
    }
    jsonObject.insert("InventoryItems",InventoryItemsJArray);

    QJsonArray ledgersJArray;
    for(LedgerMasterDataModel led:obj->ledgersList){
        ledgersJArray.append(ledObj.ObjToJson(led));
    }
    jsonObject.insert("ledgersList",ledgersJArray);
    jsonObject.insert("Narration",obj->narration);
    jsonObject.insert("priceListId",obj->priceListId);
    jsonObject.insert("priceListName",obj->priceListName);
    jsonObject.insert("discount",obj->discount);
    jsonObject.insert("discountPercent",obj->discountPercent);
    jsonObject.insert("subTotal",obj->subTotal);
    jsonObject.insert("grossTotal",obj->grossTotal);
    jsonObject.insert("discountinAmount",obj->discountinAmount);
    jsonObject.insert("grandTotal",obj->grandTotal);
    jsonObject.insert("vatAmount",obj->taxTotalAmount);
    jsonObject.insert("otherLedgersTotal",obj->otherLedgersTotal);
    jsonObject.insert("currencyConversionRate",obj->currencyConversionRate);
    jsonObject.insert("currency",obj->currency);
    jsonObject.insert("Project_ID",obj->ProjectId);
    jsonObject.insert("AddedBy",obj->AddedBy);
    jsonObject.insert("AddedById",obj->AddedById);
    jsonObject.insert("DeliveryDate",obj->DeliveryDate.toString());
    jsonObject.insert("CompletionProbability",obj->CompletionProbability);
    jsonObject.insert("RevisionNo",obj->RevisionNo);
    jsonObject.insert("ConvertedToSalesOrder",obj->ConvertedToSalesOrder);
    jsonObject.insert("QuotationPrepared",obj->QuotationPrepared);
    jsonObject.insert("QuotationDropped",obj->QuotationDropped);
    jsonObject.insert("QuotationDroppedReason",obj->QuotationDroppedReason);
    jsonObject.insert("Salesman_ID",obj->SalesmanID);
    jsonObject.insert("TermsAndConditionsID",obj->TermsAndConditionsID);
    jsonObject.insert("RequirementVoucherNo",obj->RequirementVoucherNo);
    //    AddressBookDataModel a;
    //    jsonObject.insert("Contact",a.ObjToJson(obj->Contact));
    jsonObject.insert("LPO",obj->LPO);
    jsonObject.insert("BillingName",obj->BillingName);
    jsonObject.insert("prevTransVouchers",obj->prevTransVouchers);
    jsonObject.insert("roundOff",obj->roundOff);
    jsonObject.insert("status",obj->status);
    jsonObject.insert("Voucher_Type",obj->voucherType);

    jsonObject.insert("ManagerApprovalStatus",obj->ManagerApprovalStatus);
    jsonObject.insert("ClientApprovalStatus",obj->ClientApprovalStatus);
    //    jsonObject.insert("Pax",obj->Pax);
    jsonObject.insert("NoOfCopies",obj->NoOfCopies);
    jsonObject.insert("ModeOfService",obj->ModeOfService);
    jsonObject.insert("quantityTotal",obj->quantityTotal);
    jsonObject.insert("BalanceAmount",obj->BalanceAmount);
    //    jsonObject.insert("vatAmount", obj->vatAmount);
    jsonObject.insert("PaidAmount",obj->PaidAmount);
    jsonObject.insert("reference",obj->reference);
    jsonObject.insert("Location",obj->Location);
    jsonObject.insert("POCName",obj->POCName);
    jsonObject.insert("POCEmail",obj->POCEmail);
    jsonObject.insert("POCPhone",obj->POCPhone);
    jsonObject.insert("kotNumber",obj->kotNumber);
    jsonObject.insert("BillSplit",obj->BillSplit);

    jsonObject.insert("paymentSplit",obj->paymentSplit);
    jsonObject.insert("cashPaid",obj->advanceCash);
    jsonObject.insert("balance",obj->balance);
    jsonObject.insert("toGodownName",obj->toGodownName);
    jsonObject.insert("fromGodownName",obj->fromGodownName);

    //    BillwiseRecordDataModel b;
    //    QJsonArray mapListJArray;
    //    for(BillwiseRecordDataModel* mappingObj:obj->mapList){
    //        mapListJArray.append(b.ObjToJson(mappingObj));
    //    }
    //    jsonObject.insert("mapList",mapListJArray);
    //    ChequeDetail chq;
    //    jsonObject.insert("chequeEntry",chq.ObjToJson(obj->chequeEntry));
    jsonObject.insert("crTotal",obj->crTotal);
    jsonObject.insert("drTotal",obj->drTotal);
    jsonObject.insert("ledgersTotal",obj->ledgersTotal);
    jsonObject.insert("TransactionId", obj->TransactionId);

    jsonObject.insert("sendFlag", obj->sendFlag);
    jsonObject.insert("voucherToExport", obj->voucherToExport);
    jsonObject.insert("TransactionId", obj->TransactionId);
    jsonObject.insert("action", obj->action);
    jsonObject.insert("CustomerExpectingDate", obj->CustomerExpectingDate.toString());
    jsonObject.insert("LRNO", obj->LRNO);
    jsonObject.insert("numBoxes", obj->numBoxes);
    jsonObject.insert("totalWeight", obj->totalWeight);
    jsonObject.insert("Origin", obj->Origin);
    jsonObject.insert("currencyDecimalPoints", obj->currencyDecimalPoints);
    jsonObject.insert("ReqVoucherList", obj->ReqVoucherList);

    return jsonObject;
}

GeneralVoucherDataObject *GeneralVoucherDataObject::JsonToObj(QJsonObject json)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();

    QString time_format = "ddd MMM d  HH:mm:ss yyyy";
    QString date_format = "ddd MMM d yyyy";
    LedgerMasterDataModel l;
    obj->_DisplayVoucherNumber = json["_DisplayVoucherNumber"].toString();
    obj->voucherNumber = json["Voucher_No"].toString();

    obj->VoucherDate = QDate::fromString(json["Voucher_Date"].toString(), date_format);
    obj->DateCreated = QDate::fromString(json["Date_Created"].toString(), date_format);
    obj->VoucherPrefix = json["Voucher_Prefix"].toString();
    obj->invoiceNumber = json["invoiceNumber"].toString();
    obj->invoiceDate = QDate::fromString(json["invoiceDate"].toString(), date_format);

    // Tue Mar 31 16:45:41 2020
    obj->timestamp = json["Timestamp"].toVariant().toDateTime();
    obj->lastEditedDateTime = QDateTime::fromString(json["lastEditedDateTime"].toString(), time_format);
    qDebug()<<"lastEdited"<<json["lastEditedDateTime"];
    obj->ledgerObject = l.JsonToObj(json["ledgerObject"].toObject());
    CompoundItemDataObject c;
    QJsonArray InventoryItemsJArray = json["InventoryItems"].toArray();
    for(QJsonValue invItem:InventoryItemsJArray)    {
        obj->InventoryItems.append(c.JsonToObj(invItem.toObject()));
    }
    QJsonArray ledgersJArray = json["ledgersList"].toArray();
    for(QJsonValue led:ledgersJArray){
        obj->ledgersList.append(l.JsonToObj(led.toObject()));
    }
    obj->narration = json["Narration"].toString();
    obj->priceListId = json["priceListId"].toString();
    obj->priceListName = json["priceListName"].toString();
    obj->discount = json["discount"].toVariant().toFloat();
    obj->discountPercent = json["discountPercent"].toVariant().toFloat();
    obj->subTotal = json["subTotal"].toVariant().toFloat();
    obj->grossTotal = json["grossTotal"].toVariant().toFloat();
    obj->discountinAmount = json["discountinAmount"].toVariant().toFloat();
    obj->grandTotal = json["grandTotal"].toVariant().toFloat();
    obj->taxTotalAmount = json["vatAmount"].toVariant().toFloat();
    obj->otherLedgersTotal = json["otherLedgersTotal"].toVariant().toFloat();
    obj->currencyConversionRate = json["currencyConversionRate"].toVariant().toFloat();
    obj->currency = json["currency"].toString();
    obj->ProjectId = json["Project_ID"].toString();
    obj->AddedBy = json["AddedBy"].toString();
    obj->AddedById = json["AddedById"].toInt();
    if(json["DeliveryDate"].toString().length()>0)
        obj->DeliveryDate = QDateTime::fromString(json["DeliveryDate"].toString(), time_format);
    else
        obj->DeliveryDate = QDateTime::currentDateTime();
    obj->CompletionProbability = json["CompletionProbability"].toVariant().toFloat();
    obj->RevisionNo = json["RevisionNo"].toInt();
    obj->ConvertedToSalesOrder = json["ConvertedToSalesOrder"].toString();
    obj->QuotationPrepared = json["QuotationPrepared"].toBool();
    obj->QuotationDropped = json["QuotationDropped"].toBool();
    obj->QuotationDroppedReason = json["QuotationDroppedReason"].toString();
    obj->SalesmanID = json["Salesman_ID"].toInt();
    obj->TermsAndConditionsID = json["TermsAndConditionsID"].toString();
    obj->RequirementVoucherNo = json["RequirementVoucherNo"].toString();
    AddressBookDataModel a;
    obj->Contact = a.JsonToObj(json["Contact"].toObject());
    obj->LPO = json["LPO"].toString();
    obj->BillingName = json["BillingName"].toString();
    obj->prevTransVouchers = json["prevTransVouchers"].toString();
    obj->roundOff = json["roundOff"].toVariant().toFloat();
    obj->status = json["status"].toInt();
    obj->voucherType = json["Voucher_Type"].toString();
    obj->ManagerApprovalStatus = json["ManagerApprovalStatus"].toBool();
    obj->ClientApprovalStatus = json["ClientApprovalStatus"].toBool();
    //    obj->Pax = json["Pax"].toInt();
    obj->NoOfCopies = json["NoOfCopies"].toInt();
    obj->ModeOfService = json["ModeOfService"].toInt();
    obj->quantityTotal = json["quantityTotal"].toVariant().toFloat();
    obj->BalanceAmount = json["BalanceAmount"].toVariant().toFloat();
    obj->PaidAmount = json["PaidAmount"].toVariant().toFloat();
    obj->reference = json["reference"].toString();
    obj->Location = json["Location"].toString();
    obj->POCName = json["POCName"].toString();
    obj->POCEmail = json["POCEmail"].toString();
    obj->POCPhone = json["POCPhone"].toString();
    obj->kotNumber = json["kotNumber"].toString();
    obj->BillSplit = json["BillSplit"].toBool();
    obj->paymentSplit = json["paymentSplit"].toBool();
    obj->advanceCash = json["cashPaid"].toVariant().toFloat();
    obj->balance = json["balance"].toVariant().toFloat();
    obj->toGodownName = json["toGodownName"].toString();
    obj->fromGodownName = json["fromGodownName"].toString();
    obj->TransactionId = json["TransactionId"].toString();

    obj->sendFlag = json["sendFlag"].toBool();
    obj->voucherToExport = json["voucherToExport"].toBool();
    obj->action = json["action"].toInt();
    obj->CustomerExpectingDate = QDateTime::fromString(json["CustomerExpectingDate"].toString(), date_format);
    obj->LRNO = json["LRNO"].toString();
    obj->numBoxes = json["numBoxes"].toInt();
    obj->totalWeight = json["totalWeight"].toVariant().toFloat();
    obj->Origin = json["Origin"].toInt();
    obj->currencyDecimalPoints = json["currencyDecimalPoints"].toInt();
    obj->ReqVoucherList = json["ReqVoucherList"].toArray();

    obj->fromExternal = true;
    return obj;

}

void GeneralVoucherDataObject::segItem()
{
    QList<CompoundItemDataObject> items = QList<CompoundItemDataObject>(InventoryItems);
    for(int i=0;i<items.length();i++){
        items[i].BaseItem.listId = i;
    }
}


