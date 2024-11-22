#ifndef INVENTORYITEMDATAMODEL_H
#define INVENTORYITEMDATAMODEL_H
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDate>
#include "datamodels/inventory/property/propertydatamodel.h"
#include "datamodels/inventory/uom/uomdatamodel.h"
#include "datamodels/transaction/itemfifodatamodel.h"
#include "datamodels/godowns/godowntransactiondatamodel.h"
#include "datamodels/Counter/restaurantingredientsdatamodel.h"
#include "skudatamodel.h"
#include "datamodels/inventory/measurements/contactmeasurement.h"

#include <QPointer>

class inventoryItemDataModel
{

//    Q_OBJECT

public:
    inventoryItemDataModel();
    ~inventoryItemDataModel();

    QList<QPointer<SKUDataModel>> skuList;
    QPointer<SKUDataModel> currentSKU;
    QString SKUID;
    QString ItemName;
    QString itemNameArabic;
    QString GroupName;
    QString GroupID;
    QString ItemID="";
    QString ItemAlias;
    QString ItemCode;
    QString narration;
    float stdCost=0;
    QString Dimension="";

    float price =0;
    float price_1=0;
    float price_2=0;
    float priceLastPurchase =0;

    float discount=0;
    float discountinAmount=0;
    float discountPercentage=0;
    float subTotal=0;
    float grandTotal=0;
    float moq =0;

    qint64 requirementItemIdOld=0;
    QString ItemReqUuid="X";
    int listId=0;

    QString fromGodownID;
    QString toGodownID;

    QList<GodownTransactionDataModel> godownList;

    QString PriceLevel;
    QString PartNumber;
    QString SerailNumber;
    float OpeningStock=0;
    float ClosingStock=0;
    QDate OpeningStockDate ;
    float OpeningStockValue=0;
    float OpeningStockPrice=0;
    float ReorderLevel=0;
    QString defaultSalesLedgerID;
    QString defaultPurchaseLedgerID;

    QString ProjectID;
    int brandID =0 ;
    QString brandName;

    float taxRate=0;
    float taxAmount =0;
    QStringList propertyIDsList;
    QStringList subItemsIDsList;

    QString defaultUOMID;

    UomDataModel uomObject;

    QString priceListID;
    QString priceListName;

    QStringList ImageIDS;

    bool removeItem = false;

    QDate lastModified;
    QDate dateCreated;

    QTime fromTime;
    QTime toTime;

    QString CreatedBy;
    QList<PropertyDataModel> baseProperties;

    QList<RestaurantIngredientsDataModel> bomList;
    QString ItemDescription;
    float length =1;

    int warrantyDays=0;
    float shelfLife=0;

    bool IsCompoundItem=false;
    bool isCustomItem=false;
    bool isPurchaseItem=false;
    bool isSalesItem=false;
    bool isSerailNumbered=false;
    bool isBatchProcessed = false;

    bool isBundled = false;

    bool favo = false;

    bool itemPriceEditted = false;

    bool hideInPrint = false;

    QString KOTPrinter;

    int itemProductionStatus = 0;
    int itemVoucherStatus = 0;
    QString DefaultInputTaxLedgerID;
    QString DefaultOutputTaxLedgerID;
    QString DefaultSalesReturnLedgerID;
    QString DefaultPurchaseReturnLedgerID;

    int TechnicianID =0;

    float drQty=0;
    float crQty=0;
    float consumedQty=0;
    float quantity=0;
    float maxQuantity=99999;
    float calculatedQty =0 ;
    float requestQty =0;
    float prevQty=0;
    float currQty=0;
    float discQuantity=0;
    float quantityFull=0;

    // TO BE PREP
    QString hsnCode;

    QString section;
    QJsonObject flags;
    QString salesMan_id;

    bool fromExternal = false;
    int action;


    QDate orderCompletedDate;
    QDate manufactureDate;
    QDate expiry;


    bool isStockItem=true;
    QString Category;
    QString defaultLedgerId;

    QString location;

    ContactMeasurement measurement;


    // TO DO TILL HERE

    QJsonObject objToJson(inventoryItemDataModel obj);
    inventoryItemDataModel JsonToObj(QJsonObject json);
    inventoryItemDataModel *JsonToObjPtr(QJsonObject json);
};

Q_DECLARE_METATYPE(inventoryItemDataModel)

#endif // INVENTORYITEMDATAMODEL_H
