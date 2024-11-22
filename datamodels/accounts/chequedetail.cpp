#include "chequedetail.h"

ChequeDetail::ChequeDetail()
{


}

QJsonObject ChequeDetail::ObjToJson(ChequeDetail *obj)
{
    QJsonObject jsonObject;
    jsonObject.insert("id",obj->id);
    jsonObject.insert("LedgerName",obj->LedgerName);
    jsonObject.insert("Ledger_ID",obj->LedgerID);
    jsonObject.insert("BankID",obj->BankID);
    jsonObject.insert("TransactionType",obj->TransactionType);
    jsonObject.insert("Amount",obj->Amount);
    jsonObject.insert("Cheque_Number",obj->ChequeNo);
    jsonObject.insert("VoucherNo",obj->VoucherID);
    jsonObject.insert("VoucherType",obj->VoucherType);
    jsonObject.insert("VoucherPrefix",obj->VoucherPrefix);
    jsonObject.insert("Issued_On",obj->IssuedOn.toString());
    jsonObject.insert("Bank_Name",obj->BankName);
    jsonObject.insert("Branch_Name",obj->Branch);
    jsonObject.insert("IFSC",obj->IFSC);
    jsonObject.insert("Cheque_Date",obj->InstrumentDate.toString());
    jsonObject.insert("RefNumber",obj->RefNumber);
    jsonObject.insert("is_Returned",obj->reconciled);
    jsonObject.insert("Reconciled_On",obj->reconciledDate.toString());
    jsonObject.insert("is_Presented",obj->isPresented);
    jsonObject.insert("Presented_On",obj->presentedOn.toString());
    jsonObject.insert("is_Cleared",obj->isCleared);
    jsonObject.insert("Cleared_On",obj->clearedOn.toString());
    jsonObject.insert("Narration",obj->Narration);

    return jsonObject;

}
