#include "countersettingsdatamodel.h"

int CounterSettingsDataModel::_id =0;
int CounterSettingsDataModel::CashOutPrintCopies =0;
int CounterSettingsDataModel::DeliveryPrintCopies=0;
int CounterSettingsDataModel::AllOtherPrintCopies=0;
int CounterSettingsDataModel::KOTPrintCopies=0;
bool CounterSettingsDataModel::splitTables=false;
float CounterSettingsDataModel::roundOff=0;
QTime CounterSettingsDataModel::StartTime = QTime::fromString("00:00:00");
QTime CounterSettingsDataModel::EndTime = QTime::fromString("23:59:59");
int CounterSettingsDataModel::EndDay = 0;
bool CounterSettingsDataModel::inclusiveVat=false;
bool CounterSettingsDataModel::tokenSystem=false;
bool CounterSettingsDataModel::FifoValuation=false;


QString CounterSettingsDataModel::DefaultCash;
QString CounterSettingsDataModel::MAC_ID;
QString CounterSettingsDataModel::TypeOfTrade;
QString CounterSettingsDataModel::POSNumber;
QString CounterSettingsDataModel::VoucherPrefix="A";
QString CounterSettingsDataModel::PortNo;

CounterSettingsDataModel::CounterSettingsDataModel()
{

}

bool CounterSettingsDataModel::getFifoValuation()
{
    return FifoValuation;
}

void CounterSettingsDataModel::setFifoValuation(bool value)
{
    FifoValuation = value;
}

bool CounterSettingsDataModel::getTokenSystem()
{
    return tokenSystem;
}

void CounterSettingsDataModel::setTokenSystem(bool value)
{
    tokenSystem = value;
}

void CounterSettingsDataModel::setInclusiveVat(bool value)
{
    inclusiveVat = value;
}

void CounterSettingsDataModel::setEndDay(int value)
{
    EndDay = value;
}

void CounterSettingsDataModel::setEndTime(const QTime &value)
{
    EndTime = value;
}

void CounterSettingsDataModel::setStartTime(const QTime &value)
{
    StartTime = value;
}

void CounterSettingsDataModel::setRoundOff(float value)
{
    roundOff = value;
}

void CounterSettingsDataModel::setSplitTables(bool value)
{
    splitTables = value;
}

void CounterSettingsDataModel::setPortNo(const QString &value)
{
    PortNo = value;
}

void CounterSettingsDataModel::setVoucherPrefix(const QString &value)
{
    VoucherPrefix = value;
}

void CounterSettingsDataModel::setPOSNumber(const QString &value)
{
    POSNumber = value;
}

void CounterSettingsDataModel::setTypeOfTrade(const QString &value)
{
    TypeOfTrade = value;
}

void CounterSettingsDataModel::setMAC_ID(const QString &value)
{
    MAC_ID = value;
}

void CounterSettingsDataModel::setDefaultCash(const QString &value)
{
    DefaultCash = value;
}

void CounterSettingsDataModel::setKOTPrintCopies(int value)
{
    KOTPrintCopies = value;
}

void CounterSettingsDataModel::setAllOtherPrintCopies(int value)
{
    AllOtherPrintCopies = value;
}

void CounterSettingsDataModel::setDeliveryPrintCopies(int value)
{
    DeliveryPrintCopies = value;
}

void CounterSettingsDataModel::setCashOutPrintCopies(int value)
{
    CashOutPrintCopies = value;
}

void CounterSettingsDataModel::setId(int id)
{
    _id = id;
}
