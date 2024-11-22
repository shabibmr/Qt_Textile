#include "hashtagdatabasehelper.h"

HashtagDatabaseHelper::HashtagDatabaseHelper()
{

}

int HashtagDatabaseHelper::insertSalesVoucher(GeneralVoucherDataObject *obj)
{
    QMap<QString,QVariant> map;
    for(int i=0;i<obj->HashTags.length();i++){
         map.clear();
         map.insert(HashTag_VoucherNo,obj->voucherNumber);
         map.insert(HashTag_VoucherType,obj->voucherType);
         map.insert(HashTag_VoucherPrefix,obj->VoucherPrefix);
         map.insert(HashTag_Tag,obj->HashTags[i]);
         DatabaseHelper::insertRecordsPlusStatic(map,HashTag_Tag_Table);

    }
}

int HashtagDatabaseHelper::updateSalesVoucher(GeneralVoucherDataObject *obj)
{
    QMap<QString,QVariant> clause;
    clause.insert(HashTag_VoucherNo,obj->voucherNumber);
    clause.insert(HashTag_VoucherType,obj->voucherType);
    clause.insert(HashTag_VoucherPrefix,obj->VoucherPrefix);
    DatabaseHelper::insertRecordsPlusStatic(clause,HashTag_Tag_Table);

    QMap<QString,QVariant> map;
    for(int i=0;i<obj->HashTags.length();i++){
         map.clear();
         map.insert(HashTag_VoucherNo,obj->voucherNumber);
         map.insert(HashTag_VoucherType,obj->voucherType);
         map.insert(HashTag_VoucherPrefix,obj->VoucherPrefix);
         map.insert(HashTag_Tag,obj->HashTags[i]);
         DatabaseHelper::insertRecordsPlusStatic(map,HashTag_Tag_Table);

    }
}

int HashtagDatabaseHelper::deleteSalesVoucher(GeneralVoucherDataObject *obj)
{
    QMap<QString,QVariant> clause;
    clause.insert(HashTag_VoucherNo,obj->voucherNumber);
    clause.insert(HashTag_VoucherType,obj->voucherType);
    clause.insert(HashTag_VoucherPrefix,obj->VoucherPrefix);
    DatabaseHelper::insertRecordsPlusStatic(clause,HashTag_Tag_Table);
}

QSqlQueryModel *HashtagDatabaseHelper::getAllTags()
{
   QString qStr = "";
}
