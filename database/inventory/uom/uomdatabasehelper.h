#ifndef UOMDATABASEHELPER_H
#define UOMDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/inventory/uom/uomdatamodel.h"
#include "datamodels/inventory/uom/uomconversiondatamodel.h"
#include <QSqlQueryModel>

class UomDataBaseHelper
{
public:
    UomDataBaseHelper(DatabaseHelper *db = new DatabaseHelper);
    ~UomDataBaseHelper();


    DatabaseHelper* db;

    bool flag = true;
    int startTransaction();

    int commit();

    static QString UOM_TableName ;
    static QString UOM_UOMID  ;
    static QString UOM_UOMName ;
    static QString UOM_UOMSymbol ;
    static QString UOM_DecimalPoints ;
    static QString UOM_Narration;
    static QString UOM_Last_Modified;

    static QString UOM_Conversion_TableName ;
    static QString UOM_Conversion_id ;
    static QString UOM_Conversion_BaseUnit;
    static QString UOM_Conversion_ToUnit ;
    static QString UOM_Conversion_Value ;
    static QString UOM_Conversion_Narration  ;
    static QString UOM_Conversion_ItemId ;
    static QString UOM_Conversion_BarCode;
    static QString qStrUomForItemDetail;



    int insertUOM(UomDataModel *obj);
    int updateUOM(UomDataModel *obj,QString id);
    int deleteUOM(QString id);
    QString getMaxID();

    int insertUOMConversion(UomConversionDataModel *obj);
    int updateUOMConversion(UomConversionDataModel *obj, QString id);
    int deleteUOMConversion(QString id);

    QList<UomDataModel> getAllUOM();
    UomDataModel getUomObjectByID(QString uomiD);

    int getUOMDecimalCountByID(QString id);
    UomConversionDataModel* getUOMConversionById(QString id);
    QSqlQueryModel *getDefaultUOMConversions();
    QMap<int, QString> getUOMForSearch();
    QSqlQueryModel *getUomConversionByItem(QString item);
    QList<UomConversionDataModel> getUomByItem(QString item);

    QString getUOMIDByName(QString name);
    QString getUOMNameById(QString id);

    int deleteBaseUOMConversion(UomConversionDataModel *obj);
    QSqlQueryModel *getUomModelByItem(QString itemID);
    QList<UomDataModel *> getAllUOMasPtr();
    bool checkItemExists(QString uomID);
    bool checkBarCodeExists(UomConversionDataModel *uom);
    UomDataModel *getUomPtrByID(QString uomiD);
};

#endif // UOMDATABASEHELPER_H
