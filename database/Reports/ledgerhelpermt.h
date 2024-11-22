#ifndef LEDGERHELPERMT_H
#define LEDGERHELPERMT_H

#include <QObject>
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "database/finance/creditnote/creditnotevoucherdatabasehelper.h"
#include "database/finance/debitnote/debitnotevoucherdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "datamodels/users/userprofiledataobject.h"

#include <QSqlQueryModel>
#include <QThread>
#include <database/databasehelper/databasevalues.h>

class LedgerHelperMT : public QObject
{
    Q_OBJECT
public:
    explicit LedgerHelperMT(QObject *parent = nullptr);

    QSqlDatabase db;

public slots:
    void *getLedgerTransValueByFrequency(int voucherType, QDate fromDate, QDate toDate, QString LedgerId="%");
    void *getQueryModelofQueryString(QString qStr);

signals:
    void readComplete(QSqlQueryModel* model);

    void valueCaluclated(QVariant value);

};

#endif // LEDGERHELPERMT_H
