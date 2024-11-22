#ifndef ITEMTRANSACTIONDATAMODEL_H
#define ITEMTRANSACTIONDATAMODEL_H

#include <QString>
#include <QDate>

class ItemTransactionDataModel
{
public:
    ItemTransactionDataModel();

    int _id=0;
    QString ItemID;
    QString From_Ledger;
    QString To_Ledger;
    QString VoucherID;
    QString VoucherPrefix;
    QDate VoucherDate;
    float Price=0;
    QString VoucherType;
    QString From_Location;
    QString To_Location;
    float crQty=0;
    float drQty=0;
    float drAmount=0;
    float crAmount=0;
    float consumed=0;
    QString Narration;
    QString itemReqId="X";

    QString SerialNumber="";
    QString BatchCode="";
    int tagID=0;
    QString SKU;
    QDate ManufacturingDate;
    QDate ExpiryDate;




};

#endif // ITEMTRANSACTIONDATAMODEL_H
