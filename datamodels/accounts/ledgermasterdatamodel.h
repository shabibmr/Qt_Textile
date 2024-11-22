#ifndef LEDGERMASTERDATAMODEL_H
#define LEDGERMASTERDATAMODEL_H

#include <QObject>
#include <QDate>
#include <QJsonObject>
#include "datamodels/inventory/pricelist/pricelistdataobject.h"
#include "datamodels/accounts/accountgroupdatamodel.h"
#include "datamodels/transaction/billwiserecorddatamodel.h"

class LedgerMasterDataModel
{
public:
    LedgerMasterDataModel();

    QString LedgerID;
    QString LedgerName;
    QString ledgerNameArabic;
    QString LedgerGroupId;
    QString LedgerGroupName;
    float openingBalance=0;
    QDate openingBalanceDate;

    float closingBalance=0;     //calulate
    float totalTurnover=0;      //calulate

    QString LedgerType;
    QString narration="";
    QString City;
    QString Address;
    QString emailAddress;
    QString phoneNumber;
    QString fax;
    QString parentCompany;
    QString mobileNumber;
    QString website;

    QString ContactPersonName;
    QString ContactPersonNumber;
    QString ContactPersonEmail;
    QString PoBox;
    QString Country;
    QString TRN;

    QString defaultPriceListID;

    float amount =0;

    int listid=-1;

    QString State ;
    QDate Birth_Date;
    int Credit_Period=-1;
    float Credit_Limit=-1;
    bool isIndividual;

    float crAmount =0;
    float drAmount =0;
    QString voucherNo;
    QDate voucherDate;
    QString voucherType;
    QString voucherPrefix;

    QDateTime timestamp;

    bool isInvoiceItem=false;

    float discountPercent=0;
    bool isFrequent=false;

    QString DefaultTaxLedger;
    QString TypeOfSupply;
    float taxRate=0;
    QString AgainstLedger;
    bool hasBillwiseMappings=false;
    QList<BillwiseRecordDataModel*> mapList;
    QString dbName;

    bool fromExternal=false;
    int action;

    QJsonObject ObjToJson(LedgerMasterDataModel obj);
    QJsonObject ObjPtrToJson(LedgerMasterDataModel *obj);
    LedgerMasterDataModel JsonToObj(QJsonObject json);
    LedgerMasterDataModel *JsonToObjPtr(QJsonObject json);

};

Q_DECLARE_METATYPE(LedgerMasterDataModel)

#endif // LEDGERMASTERDATAMODEL_H
