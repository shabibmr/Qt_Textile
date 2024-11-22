#ifndef CHEQUEDETAIL_H
#define CHEQUEDETAIL_H

#include <QString>
#include <QDate>
#include <QJsonObject>

class ChequeDetail
{
public:
    ChequeDetail();
    QString id;
    QString LedgerName;
    QString LedgerID;
    QString OurBankId;
    QString BankID;
    QString BankName;
    QString TransactionType;
    float Amount=0;
    float crAmount=0;
    float drAmount=0;
    QString ChequeNo;
    QString VoucherID;
    QString VoucherType;
    QString VoucherPrefix;
    QDate IssuedOn;
    QString Branch;
    QString IFSC;
    QDate InstrumentDate;
    QString RefNumber;
    bool reconciled = false;
    QDate reconciledDate;
    bool isPresented = false;
    QDate presentedOn;
    bool isCleared = false;
    QDate clearedOn;
    bool isRejected = false;
    QString Narration;

    QJsonObject ObjToJson(ChequeDetail* obj);
};

#endif // CHEQUEDETAIL_H
