#ifndef COUNTERSETTINGSDATAMODEL_H
#define COUNTERSETTINGSDATAMODEL_H

#include <QString>
#include <QObject>
#include <QTime>

class CounterSettingsDataModel
{
public:
    CounterSettingsDataModel();

    static int _id;
    static int CashOutPrintCopies;
    static int DeliveryPrintCopies;
    static int AllOtherPrintCopies;
    static int KOTPrintCopies;
    static bool splitTables;
    static bool inclusiveVat;
    static float roundOff;

    static QString DefaultCash;
    static QString MAC_ID;
    static QString TypeOfTrade;
    static QString POSNumber;
    static QString VoucherPrefix;
    static QString PortNo;
    static QTime StartTime;
    static QTime EndTime;
    static int EndDay;
    static bool tokenSystem;
    static bool FifoValuation;

    static void setId(int id);
    static void setCashOutPrintCopies(int value);
    static void setDeliveryPrintCopies(int value);
    static void setAllOtherPrintCopies(int value);
    static void setKOTPrintCopies(int value);
    static void setDefaultCash(const QString &value);
    static void setMAC_ID(const QString &value);
    static void setTypeOfTrade(const QString &value);
    static void setPOSNumber(const QString &value);
    static void setVoucherPrefix(const QString &value);
    static void setPortNo(const QString &value);
    static void setSplitTables(bool value);
    static void setRoundOff(float value);
    static void setStartTime(const QTime &value);
    static void setEndTime(const QTime &value);
    static void setEndDay(int value);
    static void setInclusiveVat(bool value);
    static bool getTokenSystem();
    static void setTokenSystem(bool value);
    static void setFifoValuation(bool value);
    static bool getFifoValuation();
};

#endif // COUNTERSETTINGSDATAMODEL_H
