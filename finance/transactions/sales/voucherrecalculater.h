#ifndef VOUCHERRECALCULATER_H
#define VOUCHERRECALCULATER_H

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"

class VoucherRecalculater : public QObject
{
    Q_OBJECT

public:
    VoucherRecalculater( QObject *parent=nullptr);

    QSqlDatabase db;

    salesVoucherDatabaseHelper *dbhelper;

public slots:
    void recalculateVouchers(QList<QPair<QString, QString>> voucherList);
    void deleteItemsinPeriod(QStringList items, QDateTime fromDate, QDateTime toDate, QList<QPair<QString, QString> > voucherList);

private:
    QThread *thread1;
    QThread *thread2;
    QThread *thread3;
    QThread *thread4;
    QThread *thread5;    

signals:
    void voucherUpdated(int);
    void recalcCompleted();
};

#endif // VOUCHERRECALCULATER_H
