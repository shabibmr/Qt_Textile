#include "voucherrecalculater.h"

#include "finance/transactions/calculator/transactioncalculator.h"
#include <QThread>
#include <QFile>
#include <QMessageBox>

VoucherRecalculater::VoucherRecalculater(QObject *parent)
{
    auto dbName = DatabaseValues::connectionString + QString::number((quint64) QThread::currentThread(),16);
    if(QSqlDatabase::contains(dbName))
        db = QSqlDatabase::database(dbName);
    else {
        qDebug()<<"ADD NEW"<<dbName;
        db = QSqlDatabase::addDatabase("QMYSQL",dbName);
        db.setDatabaseName(DatabaseValues::dbName);
        db.setHostName(DatabaseValues::dbHost);
        db.setPort(DatabaseValues::dbPort);
        db.setPassword(DatabaseValues::dbPassword);
        db.setUserName(DatabaseValues::dbUser);
    }

    if(!db.open())
        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ERROR^^^^^^^^^^^^^^^^^^"<<dbName;
    else {
//        qDebug()<<"db opened"<<dbName;
        this->dbhelper = new salesVoucherDatabaseHelper(dbName);
    }

    //    connect(dbhelper, &salesVoucherDatabaseHelper::voucherUpdated,
    //            this, &VoucherRecalculater::voucherUpdated,Qt::QueuedConnection);
}

void VoucherRecalculater::recalculateVouchers(QList<QPair<QString, QString> > voucherList)
{
    


    QString functionName = Q_FUNC_INFO;
    QString errorData = "\nFunction : " + functionName + " at " + QTime::currentTime().toString();
    errorData += "\n Get Vouchers "  ;

    for(int i=0;i<voucherList.size();i++){
//        qDebug()<<"in loop"<<i<<QTime::currentTime();
        GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherList[i].first, voucherList[i].second);
            errorData += voucher->voucherNumber + " Current total " + QString::number(voucher->grandTotal, 'f',2) + ", ";

//        qDebug()<<"run recalc"<<voucher->voucherNumber<<QTime::currentTime();;
        TransactionCalculator::VoucherCalculateSales(voucher);
//        qDebug()<<"calc done"<<voucher->InventoryItems.size()<<voucher->grandTotal<<QTime::currentTime();
        dbhelper->prepareSalesVoucherInsert(voucher);
        emit voucherUpdated(i);
        delete voucher;
    }

    QFile File("featadjlog.txt");
    File.open(QFile::WriteOnly | QFile::Text | QFile::Append);
    File.write(errorData.toUtf8());
    File.close();
    qDebug()<<"run db ops"<<QTime::currentTime();
    dbhelper->insertSalesVoucherRecalc(voucherList);
    emit recalcCompleted();
    qDebug()<<"end of recalc"<<QTime::currentTime();
    db.close();

}

void VoucherRecalculater::deleteItemsinPeriod(QStringList items, QDateTime fromDateTime, QDateTime toDateTime, QList<QPair<QString, QString> > voucherList)
{
    dbhelper->deleteItemsFromVouchers(items, fromDateTime, toDateTime, voucherList );
}
