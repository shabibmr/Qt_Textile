#ifndef LEDGERDETAILDATAMODEL_H
#define LEDGERDETAILDATAMODEL_H

#include <QString>
#include <QDate>
#include "datamodels/inventory/pricelist/pricelistdataobject.h"
#include "datamodels/accounts/accountgroupdatamodel.h"


class LedgerDetailDataModel
{
public:
    LedgerDetailDataModel();
    ~LedgerDetailDataModel();

    QString LedgerID;
    QString LedgerName;
    QString LedgerGroupId;
    QString LedgerGroupName;
    QString openingBalance;
    QDate openingBalanceDate;

    QString closingBalance;     //calulate
    QString totalTurnover;      //calulate

    QString LedgerType;
    QString narration;
    QString City;
    QString Address;
    QString emailAddress;
    QString phoneNumber;
    QString mobileNumber;
    QString website;

    QString ContactPersonName;
    QString ContactPersonNumber;
    QString PoBox;
    QString Country;

    PriceListDataObject defaultPriceList;

    QString PriceListName;
    QString PriceListID;

    float amount=0;
    float crAmount=0;
    float drAmount=0;

    bool isInvoiceItem;

};

#endif // LEDGERDETAILDATAMODEL_H
