#include "uisettingsdatabasehelper.h"
#include <QMessageBox>

QString UiSettingsDatabaseHelper::UI_Config_TableName = "algo_config.ui_config"   ;
QString UiSettingsDatabaseHelper::UI_Config_id   ="_id"       ;
QString UiSettingsDatabaseHelper::UI_Config_Name  ="ui_name";//report name
QString UiSettingsDatabaseHelper::UI_Config_Type   ="ui_type";//
QString UiSettingsDatabaseHelper::UI_Config_Module   ="Module";//
QString UiSettingsDatabaseHelper::UI_Config_SubType ="sub_type";// primary column type
QString UiSettingsDatabaseHelper::UI_Config_Query = "query";
QString UiSettingsDatabaseHelper::UI_Config_Filters ="filters";// report
QString UiSettingsDatabaseHelper::UI_Config_Totals = "display_options";//
QString UiSettingsDatabaseHelper::UI_Config_Redirect_To ="redirect_to"; //
QString UiSettingsDatabaseHelper::UI_Config_Extras = "extras";


UiSettingsDatabaseHelper::UiSettingsDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

UiSettingsDataModel *UiSettingsDatabaseHelper::getUiConfigById(int Ui_Id)
{
    UiSettingsDataModel* obj = new UiSettingsDataModel;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = " SELECT ";
    qStr += UI_Config_id +", " + UI_Config_Name + ", " + UI_Config_Type + ", " + UI_Config_SubType + ", ";
    qStr += UI_Config_Query + ", " + UI_Config_Totals + ", " + UI_Config_Filters +", " + UI_Config_Redirect_To + ", ";
    qStr += UI_Config_Extras ;
    qStr += " FROM " + UI_Config_TableName + " WHERE " + UI_Config_id + " =:id";
    query.prepare(qStr);
    query.bindValue(":id", Ui_Id);
    if(query.exec())
    {
        if(query.first())
        {
            int i=0;
            obj->ui_id = query.value(i++).toInt();
            obj->ui_name = query.value(i++).toString();
            obj->ui_type = query.value(i++).toString();
            obj->sub_type = query.value(i++).toString();
            obj->query = /*QJsonDocument::fromJson(*/query.value(i++).toString()/*.toUtf8()).object()*/;
            obj->displayOptions = QJsonDocument::fromJson(query.value(i++).toString().toUtf8()).object();
            obj->filters =  QJsonDocument::fromJson(query.value(i++).toString().toUtf8()).object();
            obj->redirect_to = QJsonDocument::fromJson(query.value(i++).toString().toUtf8()).object();
            QString extras = query.value(i++).toString();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<extras;
            obj->extras = QJsonDocument::fromJson(extras.toUtf8()).array();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<obj->extras;

            qDebug()<<"set display options"<<obj->displayOptions;
            for(QJsonValue totalCol:obj->displayOptions["Totals"].toArray()){
                obj->totalCols.append(totalCol.toInt())    ;
            }
            qDebug()<<"$$$ Total of"<<obj->totalCols;
            for(QJsonValue col:obj->displayOptions["Hide"].toArray()){
                obj->hideCols.append(col.toInt())    ;
            }
            qDebug()<<"$$$ hide"<<obj->hideCols;

            for(QJsonValue col:obj->displayOptions["width-list"].toArray()){
                obj->widthList.append(col.toInt())    ;
            }
            qDebug()<<"$$$ width"<<obj->widthList;

            for(QJsonValue col:obj->displayOptions["align-list"].toArray()){
                obj->AlignmentList.append(col.toInt())    ;
            }

            for(QJsonValue col:obj->displayOptions["print-cols"].toArray()){
                obj->avlCols.append(col.toInt())    ;
            }
            qDebug()<<"$$$ available"<<obj->avlCols;

            obj->stretchCol = obj->displayOptions["stretch"].toInt();
        }

    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
        QMessageBox box; box.setText("Query Error"); box.exec();

    }



    return obj;

}

QSqlQueryModel *UiSettingsDatabaseHelper::getUiTypesByModule(QString module)
{
    QStringList uiTypes;

    QString  qStr = " SELECT distinct " + Widget_Master_type + " FROM " + Widget_Master_TableName;
    qStr += " WHERE " + Widget_Master_Module + " like '%" + module + "%'";
    qStr += " ORDER BY 1";


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *UiSettingsDatabaseHelper::getUiListByModule(QString module, QString type)
{

    QStringList uiTypes;

    QString  qStr = " SELECT " + UI_Config_id + " , " + UI_Config_Name + " FROM " + UI_Config_TableName;
    qStr += " WHERE " + UI_Config_Module + " = '" + module + "'";
    qStr += " AND " + UI_Config_Type + " = '" + type + "'";
    qStr += " ORDER BY " + UI_Config_Name;
//    qStr += " LIMIT 1";


    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *UiSettingsDatabaseHelper::getAllUIList(QString module)
{

    QString  qStr = " SELECT " + Widget_Master_WidgetId + " , " + Widget_Master_Name + ", ";
    qStr += Widget_Master_type + ", " + Widget_Master_Module +  " FROM "
            + Widget_Master_TableName;
    if(module != "%")
        qStr += " WHERE " + Widget_Master_Module + " like '%" + module + "%' ";

    qStr += " ORDER BY " +Widget_Master_type + ", " + Widget_Master_Name;
    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *UiSettingsDatabaseHelper::getAlScreensByModule(QString module, QString type)
{
    QString qStr = " SELECT " + Widget_Master_WidgetId + ", " + Widget_Master_Name + ",  " + Widget_Master_DbConfigured ;
    qStr += " FrOM " + Widget_Master_TableName ;
    qStr += " WHERE " + Widget_Master_Category + " = 'Screen' ";
    qStr += " AND " + Widget_Master_Module + " like '%" + module + "%' ";
    qStr += " AND " + Widget_Master_type + " like '%" + type + "%' ";
    qStr += " AND " + Widget_Master_Priority + " > 0";
    qStr += " ORDER BY " + Widget_Master_Priority;

    return  DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *UiSettingsDatabaseHelper::getAllScreensForSearchModel()
{
    QString qStr = " SELECT " + Widget_Master_WidgetId + ", " + Widget_Master_Name + " " ;
    qStr += " FrOM " + Widget_Master_TableName ;
    qStr += " WHERE " + Widget_Master_Category + " = 'Screen' ";

    return  DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *UiSettingsDatabaseHelper::getScreenIdByName(QString name)
{
    QString qStr = " SELECT " + Widget_Master_WidgetId + ", " +  Widget_Master_DbConfigured;
    qStr += " FrOM " + Widget_Master_TableName ;
    qStr += " WHERE " + Widget_Master_Name + " = '"+name +"' ";

    return  DatabaseHelper::getQueryModel(qStr);
}

int UiSettingsDatabaseHelper::getUiIdByVoucherType(QString voucherType)
{
    QString qStr = " SELECT " + UI_Config_id + " FROM " + UI_Config_TableName;
    qStr += " WHERE " + UI_Config_SubType + " = '" + voucherType + "'";

    return DatabaseHelper::getSingletonQueryResult(qStr).toInt();
}
