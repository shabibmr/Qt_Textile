#ifndef UISETTINGSDATABASEHELPER_H
#define UISETTINGSDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/Settings/uisettingsdatamodel.h"

class UiSettingsDatabaseHelper
{
public:
    UiSettingsDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());


    static QString UI_Config_TableName   ;
    static QString UI_Config_id          ;
    static QString UI_Config_Name         ;
    static QString UI_Config_Module ;
    static QString UI_Config_Type       ;
    static QString UI_Config_SubType   ;
    static QString UI_Config_Query   ;
    static QString UI_Config_Filters;
    static QString UI_Config_Totals ;
    static QString UI_Config_Redirect_To ;
    static QString UI_Config_Extras ;


    QString Widget_Master_TableName  = "algo_config.Widget_Master" ;
    QString Widget_Master_id     ="_id"     ;
    QString Widget_Master_WidgetId     ="Widget_id"     ;
    QString Widget_Master_Name      ="Widget_Name"   ;
    QString Widget_Master_Module ="Modules";
    QString Widget_Master_Category       ="Category";
    QString Widget_Master_type   ="Widget_Type";
    QString Widget_Master_Priority = "Priority";
    QString Widget_Master_DbConfigured = "isConfiguredInDb";

    UiSettingsDataModel *getUiConfigById(int Ui_Id);
    QSqlQueryModel *getUiTypesByModule(QString module);
    QSqlQueryModel *getUiListByModule(QString module, QString type);

    QSqlQueryModel *getAllUIList(QString module = "%");

    QSqlQueryModel *getAlScreensByModule(QString module, QString type);
    QSqlQueryModel *getAllScreensForSearchModel();
    QSqlQueryModel *getScreenIdByName(QString name);



    static int getUiIdByVoucherType(QString voucherType);
private:
    DatabaseHelper *db;
};

#endif // UISETTINGSDATABASEHELPER_H
