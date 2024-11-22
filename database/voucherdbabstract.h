#ifndef VOUCHERDBABSTRACT_H
#define VOUCHERDBABSTRACT_H

#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/databasehelper/bulkloaddatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include "database/Notifications/sendchanneldatabasehelper.h"


#include <QMap>
#include <QDebug>

class VoucherDBAbstract
{

public :

    virtual ~VoucherDBAbstract(){}

    virtual int insertVoucher(GeneralVoucherDataObject *obj);

    virtual int updateVoucher(GeneralVoucherDataObject *obj, QString oldID);

    virtual int deleteVoucher(GeneralVoucherDataObject *obj);

    virtual QString getNextVoucherNo(QString voucherPrefix);

    virtual GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix);

    virtual QString getVoucherNumberByLedID(QString LedID,QDate Date);

    virtual QPair<QString, QString> getVoucherNoByTransactionId(QString transId);

    virtual QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom,QDateTime dateTo);

    virtual QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo);

    virtual int updateVoucherStatus(GeneralVoucherDataObject *voucher);

 /*
 * getVoucherListByDateModel
 * virtual QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom,QDate dateTo);
 * virtual QSqlQueryModel* getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo);
 */


};

#endif // VOUCHERDBABSTRACT_H
