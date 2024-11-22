#ifndef COUNTERSETTINGSDATABASEHELPER_H
#define COUNTERSETTINGSDATABASEHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "database/databasehelper/databasehelper.h"


class CounterSettingsDatabaseHelper
{
public:
    CounterSettingsDatabaseHelper();

    DatabaseHelper *db;

    bool flag=true;

    QString Settings_TableName = "Settings";

    QString Settings_id = "_id";
    QString Settings_Cash_Out = "Cash_Out";
    QString Settings_Delivery = "Delivery";
    QString Settings_All_Other = "All_Other";
    QString Settings_KOT_Copy = "KOT_Copy";
    QString Settings_Default_Cash = "Default_Cash";
    QString Settings_MAC_ID = "MAC_ID";
    QString Settings_Type_Of_Trade = "Type_Of_Trade";
    QString Settings_POS_Number = "POS_Number";
    QString Settings_Voucher_Prefix = "Voucher_Prefix";
    QString Settings_Port_No = "Port_No";
    QString Settings_SplitTables = "SplitTables";
    QString Settings_Roundoff = "Round_Off";
    QString Settings_StartTime = "Start_Time";
    QString Settings_EndTime = "End_Time";
    QString Settings_EndDay = "End_Day";
    QString Settings_Inclusive = "Inclusive";
    QString Settings_TokenSystem = "Token_System";
    QString Settings_FIFO = "Fifo_Valuation";



    int startTransaction();
    void insertCounterSettings();
    void updateCounterSettings(QString mac);
    void deleteCounterSettings(int settingsID);
    void getCounterSettings(QString mac);

    int commit();

    void changeCollation();

    void updatesplitTable(bool val);

};

#endif // COUNTERSETTINGSDATABASEHELPER_H
