#ifndef PRICELISTDATABASEHELPER_H
#define PRICELISTDATABASEHELPER_H


#include "datamodels/inventory/pricelist/pricelistdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

#include <QMap>
#include <QDebug>
#include <QSqlQueryModel>

class PricelistDatabaseHelper
{
public:
    PricelistDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~PricelistDatabaseHelper();

    static QString PriceListEntries_TableName ;

    QString PriceListEntries_PriceListID = "Price_List_ID";
    static QString PriceListEntries_ItemID;
    QString PriceListEntries_Price = "Price";
    QString PriceListEntries_Percent = "percent";
    QString PriceListEntries_UomId = "UOM_Id";
    QString PriceListEntries_Last_Modified = "Timestamp";

    static QString PriceLists_TableName;
    static QString PriceListID;
    static QString PriceListName;
    QString PriceListStartDate = "Price_List_Start_Date";
    QString PriceListEndDate = "Price_List_End_Date";
    QString PriceListDefault = "Price_List_Default";
    QString PriceListShowInPos = "Price_List_ShowInPos";
    QString PriceList_Last_Modified = "Last_Modified";

    bool flag = true;
    int startTransaction();
    int commit();
    DatabaseHelper *db;
    int insertPriceList(PriceListDataObject *obj);
    int updatePriceList(PriceListDataObject *obj, int id);
    int deletePriceList(QString id);

    QString getMaxID();
    QString static getPriceListNameByID(QString id);
    PriceListDataObject getPriceListByName(QString priceName);
    PriceListDataObject getPriceListByID(int priceListID);
    PriceListDataObject *getPriceListByIDasPtr(int priceListID);

    QString getDefaultPriceList(QString ledgerID = "");

    //    int getPriceListIDByName(int id);
    QMap<QString,QString> getPriceListAsMap();
    QList<PriceListDataObject> getAllPriceLists(bool all =true);
    QSqlQueryModel *getPriceListsAsQueryModel();

    int getPriceListIDByName(QString name);

    float getPriceByItemID(QString itemID,QString priceListID);

    float getPriceByItemIDAndDate(QString itemID,QString priceListID,QDate Date);

    QSqlQueryModel *getAllPricesOfItem(QString itemID);

    QMap<QString,float> getAllPricesOfItemAsMap(QString itemID);

    void deletePricesByItem(QString itemID);

    void insertItemPrices(QString itemID,QString priceID, QString uomID, float price);

    float getPriceByItemIDAndUOM(QString itemID, QString priceListID = "", QString uomId = "");
};

#endif // PRICELISTDATABASEHELPER_H
