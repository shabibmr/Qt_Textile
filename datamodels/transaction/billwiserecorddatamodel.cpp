#include "billwiserecorddatamodel.h"

BillwiseRecordDataModel::BillwiseRecordDataModel()
{
    VoucherDate = QDate::fromString("2000-01-01","yyyy-MM-dd");
    DueDate = QDate::fromString("2000-01-01","yyyy-MM-dd");
}

QJsonObject BillwiseRecordDataModel::ObjToJson(BillwiseRecordDataModel *obj)
{
    QJsonObject jsonObject;
    qDebug()<<"BILLWISE RECORDS MAP OBJECT TO JSON";
    jsonObject.insert("_id",obj->_id);
    jsonObject.insert("VoucherPrefix",obj->VoucherPrefix);
    jsonObject.insert("VoucherType",obj->VoucherType);
    jsonObject.insert("VoucherNo",obj->VoucherNo);
    jsonObject.insert("LedgerId",obj->LedgerID);
    jsonObject.insert("RefPrefix",obj->RefPrefix);
    jsonObject.insert("RefType",obj->RefType);
    jsonObject.insert("RefNo",obj->RefVoucherNo);
    jsonObject.insert("Method_Of_Adj",obj->MethodOfAdjustment);
    jsonObject.insert("DebitAmount",obj->DebitAmount);
    jsonObject.insert("CreditAmount",obj->CreditAmount);
    jsonObject.insert("VoucherDate",obj->VoucherDate.toString());
    jsonObject.insert("Due_Date",obj->DueDate.toString());
    jsonObject.insert("Amount",obj->Amount);
    jsonObject.insert("selectedAmount",obj->selectedAmount);
    jsonObject.insert("mapFlag",obj->mapFlag);
    jsonObject.insert("pos",obj->pos);

    return jsonObject;

}
