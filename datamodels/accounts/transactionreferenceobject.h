#ifndef TRANSACTIONREFERENCEOBJECT_H
#define TRANSACTIONREFERENCEOBJECT_H
#include <QString>

class TransactionReferenceObject
{
public:
    TransactionReferenceObject();
    ~TransactionReferenceObject();
    QString PaidAs;
    QString VoucherType;
    QString id;
    QString VoucherNo;

    float Amount;
    QString ledgerID;
    QString ledgerName;
    bool creditTrue;
};

#endif // TRANSACTIONREFERENCEOBJECT_H
