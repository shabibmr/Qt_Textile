#ifndef BILLWISERECORDDATAMODEL_H
#define BILLWISERECORDDATAMODEL_H

#include "QDateTime"
#include "QDate"
#include <QObject>
#include <QJsonObject>
#include <QDebug>

class BillwiseRecordDataModel
{

public:
    BillwiseRecordDataModel();

    int _id;
    QString VoucherPrefix;
    QString VoucherType;
    QString VoucherNo;
    QString LedgerID;
    QString RefPrefix;
    QString RefType;
    QString RefVoucherNo;
    QString MethodOfAdjustment;
    float DebitAmount =0;
    float CreditAmount =0;
    QDate VoucherDate;
    QDate DueDate;
    float Amount;
    float selectedAmount;
    bool mapFlag;
    int pos;

    QJsonObject ObjToJson(BillwiseRecordDataModel* obj);

};

Q_DECLARE_METATYPE(BillwiseRecordDataModel)

#endif // BILLWISERECORDDATAMODEL_H
