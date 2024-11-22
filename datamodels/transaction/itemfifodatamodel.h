#ifndef ITEMFIFODATAMODEL_H
#define ITEMFIFODATAMODEL_H

#include <QString>
#include <QDate>

class ItemFifoDataModel
{
public:
    ItemFifoDataModel();

    int _id=0;
    QString ItemID;
    QString VoucherID;
    QString VoucherPrefix;
    QString VoucherType;
    QString ItemReqId;
    QDate VoucherDate;
    float Price;
    float crQty=0;
    float drQty=0;
    float consumedQty=0;
    bool updated=false;

    QString From_Location;
    QString To_Location;
    float marginPercent=10;
    float marinByValue=0;
    float sellingPrice=0;
};

#endif // ITEMFIFODATAMODEL_H
