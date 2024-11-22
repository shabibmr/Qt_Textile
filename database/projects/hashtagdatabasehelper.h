#ifndef HASHTAGDATABASEHELPER_H
#define HASHTAGDATABASEHELPER_H

#include <QSqlQueryModel>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"

class HashtagDatabaseHelper
{
public:
    HashtagDatabaseHelper();

    QString HashTag_Tag_Table        = "Hash_Tag_Table";

    QString HashTag_id               = "_id";
    QString HashTag_VoucherPrefix    = "Voucher_Prefix";
    QString HashTag_VoucherNo        = "Voucher_No";
    QString HashTag_VoucherType      = "Voucher_Type";
    QString HashTag_Tag              = "Hash_Tag";


    DatabaseHelper *db;

    int startTransaction();

    int insertSalesVoucher(GeneralVoucherDataObject *obj);

    int updateSalesVoucher(GeneralVoucherDataObject *obj);

    int deleteSalesVoucher(GeneralVoucherDataObject *obj);

    int commit();

    QSqlQueryModel *getAllTags();



};

#endif // HASHTAGDATABASEHELPER_H
