#include "countersettingsdatabasehelper.h"
#include <QtNetwork/QNetworkInterface>

CounterSettingsDatabaseHelper::CounterSettingsDatabaseHelper()
{
    db = new DatabaseHelper();
}

int CounterSettingsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

void CounterSettingsDatabaseHelper::insertCounterSettings()
{
    db->startTransaction();
    QMap<QString,QVariant> valuesMap;

    valuesMap.insert(Settings_Cash_Out,CounterSettingsDataModel::CashOutPrintCopies);
    valuesMap.insert(Settings_Delivery,CounterSettingsDataModel::DeliveryPrintCopies);
    valuesMap.insert(Settings_All_Other,CounterSettingsDataModel::AllOtherPrintCopies);
    valuesMap.insert(Settings_KOT_Copy,CounterSettingsDataModel::KOTPrintCopies);
    valuesMap.insert(Settings_Default_Cash,CounterSettingsDataModel::DefaultCash);
    valuesMap.insert(Settings_Voucher_Prefix,CounterSettingsDataModel::VoucherPrefix);
    valuesMap.insert(Settings_MAC_ID,CounterSettingsDataModel::MAC_ID);
    valuesMap.insert(Settings_Type_Of_Trade,CounterSettingsDataModel::TypeOfTrade);
    valuesMap.insert(Settings_POS_Number,CounterSettingsDataModel::POSNumber);
    valuesMap.insert(Settings_Port_No,CounterSettingsDataModel::PortNo);
    valuesMap.insert(Settings_SplitTables,CounterSettingsDataModel::splitTables);
    valuesMap.insert(Settings_Roundoff,CounterSettingsDataModel::roundOff);
    valuesMap.insert(Settings_StartTime,CounterSettingsDataModel::StartTime);
    valuesMap.insert(Settings_EndDay,CounterSettingsDataModel::EndDay);
    valuesMap.insert(Settings_EndTime,CounterSettingsDataModel::EndTime);
    valuesMap.insert(Settings_TokenSystem,CounterSettingsDataModel::tokenSystem);
    valuesMap.insert(Settings_Inclusive,CounterSettingsDataModel::inclusiveVat);
    valuesMap.insert(Settings_FIFO,CounterSettingsDataModel::FifoValuation);
    db->insertRecordsPlus(valuesMap,Settings_TableName);
    commit();
}

void CounterSettingsDatabaseHelper::updateCounterSettings(QString mac)
{
    QMap<QString,QVariant> valuesMap;
    valuesMap.insert(Settings_Cash_Out,CounterSettingsDataModel::CashOutPrintCopies);
    valuesMap.insert(Settings_Delivery,CounterSettingsDataModel::DeliveryPrintCopies);
    valuesMap.insert(Settings_All_Other,CounterSettingsDataModel::AllOtherPrintCopies);
    valuesMap.insert(Settings_KOT_Copy,CounterSettingsDataModel::KOTPrintCopies);
    valuesMap.insert(Settings_Default_Cash,CounterSettingsDataModel::DefaultCash);
    valuesMap.insert(Settings_Voucher_Prefix,CounterSettingsDataModel::VoucherPrefix);
    valuesMap.insert(Settings_MAC_ID,CounterSettingsDataModel::MAC_ID);
    valuesMap.insert(Settings_Type_Of_Trade,CounterSettingsDataModel::TypeOfTrade);
    valuesMap.insert(Settings_POS_Number,CounterSettingsDataModel::POSNumber);
    valuesMap.insert(Settings_Port_No,CounterSettingsDataModel::PortNo);
    valuesMap.insert(Settings_SplitTables,CounterSettingsDataModel::splitTables);
    valuesMap.insert(Settings_Roundoff,CounterSettingsDataModel::roundOff);
    valuesMap.insert(Settings_StartTime,CounterSettingsDataModel::StartTime);
    valuesMap.insert(Settings_EndDay,CounterSettingsDataModel::EndDay);
    valuesMap.insert(Settings_EndTime,CounterSettingsDataModel::EndTime);
    valuesMap.insert(Settings_TokenSystem,CounterSettingsDataModel::tokenSystem);
    valuesMap.insert(Settings_Inclusive,CounterSettingsDataModel::inclusiveVat);
    valuesMap.insert(Settings_FIFO,CounterSettingsDataModel::FifoValuation);
    QMap<QString,QVariant> clauseMap;
    clauseMap.insert(Settings_Voucher_Prefix,mac);
    startTransaction();
    db->updateRecordsPlus(valuesMap,clauseMap,Settings_TableName);
    commit();
}

void CounterSettingsDatabaseHelper::deleteCounterSettings(int settingsID)
{
    QMap<QString,QVariant> clauseMap;
    clauseMap.insert(Settings_id,settingsID);
    db->deleteRecordsPlus(clauseMap,Settings_TableName);
}

int CounterSettingsDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

void CounterSettingsDatabaseHelper::getCounterSettings(QString vpref)
{
    QString qStr = "SELECT ";
    qStr += Settings_id + ", ";
    qStr += Settings_Cash_Out + ", ";
    qStr += Settings_Delivery + ", ";
    qStr += Settings_All_Other + ", ";
    qStr += Settings_KOT_Copy + ", ";
    qStr += Settings_Default_Cash + ", ";
    qStr += Settings_Type_Of_Trade + ", ";
    qStr += Settings_POS_Number + ", ";
    qStr += Settings_Voucher_Prefix + ", ";
    qStr += Settings_SplitTables + ", ";
    qStr += Settings_Roundoff + ", ";
    qStr += Settings_StartTime + ", ";
    qStr += Settings_EndDay + ", ";
    qStr += Settings_EndTime + ", ";
    qStr += Settings_Port_No + ",";
    qStr += Settings_Inclusive +", ";
    qStr += Settings_TokenSystem + ", ";
    qStr += Settings_FIFO +" ";

    qStr += " FROM "+ Settings_TableName ;
//    qStr += " where " + Settings_Voucher_Prefix + " like '%"+ vpref +"%'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<qStr;
    query.prepare(qStr);

    if(query.exec()){
        int i=0;
        while(query.next()){
            i=0;
            CounterSettingsDataModel::setId(query.value(i++).toInt());
            CounterSettingsDataModel::setCashOutPrintCopies(query.value(i++).toInt());
            CounterSettingsDataModel::setDeliveryPrintCopies(query.value(i++).toInt());
            CounterSettingsDataModel::setAllOtherPrintCopies(query.value(i++).toInt());
            CounterSettingsDataModel::setKOTPrintCopies(query.value(i++).toInt());
            CounterSettingsDataModel::setDefaultCash(query.value(i++).toString());
            CounterSettingsDataModel::setTypeOfTrade(query.value(i++).toString());
            CounterSettingsDataModel::setPOSNumber(query.value(i++).toString());
            i++;
//            CounterSettingsDataModel::setVoucherPrefix(query.value(i++).toString());
            CounterSettingsDataModel::setSplitTables(query.value(i++).toBool());
            CounterSettingsDataModel::setRoundOff(query.value(i++).toFloat());
            CounterSettingsDataModel::setStartTime(query.value(i++).toTime());
            CounterSettingsDataModel::setEndDay(query.value(i++).toInt());
            CounterSettingsDataModel::setEndTime(query.value(i++).toTime());
            CounterSettingsDataModel::setPortNo(query.value(i++).toString());
            CounterSettingsDataModel::setInclusiveVat(query.value(i++).toBool());
            CounterSettingsDataModel::setTokenSystem(query.value(i++).toBool());
            CounterSettingsDataModel::setFifoValuation(query.value(i++).toBool());
        }
    }
    else{
        qDebug()<<Q_FUNC_INFO<<"ER";
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
//    qDebug()<<qStr;

//    qDebug()<<"Default cash = "<<CounterSettingsDataModel::DefaultCash;
}



void CounterSettingsDatabaseHelper::changeCollation()
{
    QStringList TableNames;
    QString Table;
    QString qStr = "SHOW TABLES";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            Table = query.value(0).toString();
            TableNames<<Table;
            qDebug()<<"Table = "<<Table;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    for(int i=0; i<TableNames.size();i++){
        qStr = "ALTER TABLE ";
        qStr += TableNames[i];
        qStr += " CONVERT TO CHARACTER SET utf8 COLLATE utf8_general_ci";
        query.prepare(qStr);
        if(query.exec()){
            while(query.next()){
                qDebug()<<"convert = "<<Table;
            }
        }
        else{
            qDebug()<<query.lastError();
            qDebug()<<query.lastQuery();
        }

    }

}

void CounterSettingsDatabaseHelper::updatesplitTable(bool val)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Settings_SplitTables,val);
    clause.insert(Settings_MAC_ID,CounterSettingsDataModel::MAC_ID);
    startTransaction();
    db->updateRecordsPlus(map,clause,Settings_TableName);
    commit();

}
