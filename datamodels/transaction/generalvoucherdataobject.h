#ifndef GENERALVOUCHERDATAOBJECT_H
#define GENERALVOUCHERDATAOBJECT_H

#include <QJsonObject>


#include "datamodels/accounts/ledgermasterdatamodel.h"
#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include "datamodels/addressbook/addressbookdatamodel.h"
#include "datamodels/hr/userprofiledatamodel.h"
#include "datamodels/accounts/chequedetail.h"
#include "login/loginvalues.h"
#include "gmvouchertypes.h"

class GeneralVoucherDataObject
{

public:

    GeneralVoucherDataObject();
    ~GeneralVoucherDataObject();


    QString _DisplayVoucherNumber;

    QString voucherNumber;
    QDate VoucherDate;
    QString VoucherPrefix;
    QString invoiceNumber;
    QDate invoiceDate;
    QDate DateCreated;
    QDateTime timestamp;
    QDateTime lastEditedDateTime;

    LedgerMasterDataModel ledgerObject;
    QList<CompoundItemDataObject> InventoryItems;
    QList<inventoryItemDataModel> deletedItems;
    QList<LedgerMasterDataModel> ledgersList;
    QString narration;
    QString priceListId;
    QString priceListName;

    float discount=0;
    float discountPercent=0;

    float subTotal=0;
    float grossTotal=0;
    float discountinAmount=0;
    float grandTotal=0;
    float taxTotalAmount=0;
    float otherLedgersTotal=0;
    float cessAmount = 0;

    float currencyConversionRate=1;
    QString currency;
    QString ProjectId;
    QString AddedBy;
    int AddedById=0;
    QDateTime DeliveryDate;
    QDateTime pickedTime;
    float CompletionProbability=100;
    int CreditPeriod=0;
    QDateTime completedTimeStamp;

    int RevisionNo=0;
    QString ConvertedToSalesOrder ="";
    bool QuotationPrepared;
    bool QuotationDropped;
    QString QuotationDroppedReason;
    int SalesmanID=0;
    QString TermsAndConditionsID;
    QString RequirementVoucherNo;
    AddressBookDataModel Contact;
    QString LPO="";
    QString BillingName;

    QString prevTransVouchers="";
    float roundOff=0;

    int status=10;

    QString voucherType;

    bool ManagerApprovalStatus=false;
    bool ClientApprovalStatus=false;

    //    int Pax=0;  // changed to numBoxes
    int NoOfCopies=0;
    int ModeOfService=0;

    float quantityTotal=0;

    float BalanceAmount = 0;
    float PaidAmount = 0;

    QString reference;
    QString Location;
    QString POCName;
    QString POCEmail;
    QString POCPhone;
    QString kotNumber;

    bool BillSplit = false;
    bool paymentSplit=false;

    float advanceCash = 0;

    float balance = 0;

    QString fromGodownName,toGodownName;
    QString fromGodownID = "" ,toGodownID ="";
    QList<BillwiseRecordDataModel*> mapList;
    ChequeDetail* chequeEntry;

    float crTotal=0;
    float drTotal=0;
    float ledgersTotal=0;
    bool fromExternal=false;
    bool sendFlag=false;
    bool voucherToExport=false;
    QString TransactionId;

    int action;

    QDateTime CustomerExpectingDate;

    QStringList HashTags;

    ContactMeasurement *measurement;


    // NOT DONE
    QString LRNO;
    int numBoxes=0;
    float totalWeight=0;
    int Origin=0;
    //    QString Currency;
    int currencyDecimalPoints=2;
    QJsonArray ReqVoucherList;

    bool isCoupled = false;

    //TO DO TILL HERE
    QJsonObject ObjToJson(GeneralVoucherDataObject *obj);
    GeneralVoucherDataObject *JsonToObj(QJsonObject json);


    void segItem();

    GeneralVoucherDataObject* copy (GeneralVoucherDataObject &OBJ){

        this->_DisplayVoucherNumber=                                        OBJ._DisplayVoucherNumber;
        this->voucherNumber=                                             OBJ.voucherNumber;
        this->VoucherDate=                                                  OBJ.VoucherDate;
        this->VoucherPrefix=                                               OBJ.VoucherPrefix;
        this->invoiceNumber=                                               OBJ.invoiceNumber;
        this->invoiceDate   =                                               OBJ.invoiceDate;
        this->timestamp      =                                             OBJ.timestamp;
        this->lastEditedDateTime    =                                     OBJ.lastEditedDateTime;
        this->ledgerObject           =                                      OBJ.ledgerObject;
        this->InventoryItems          =                                     OBJ.InventoryItems;
        this->ledgersList              =                                    OBJ.ledgersList;
        this->narration                 =                                   OBJ.narration;
        this->priceListId                =                                  OBJ.priceListId;
        this->priceListName               =                                OBJ.priceListName;
        this->discount                     =                                OBJ.discount;
        this->discountPercent               =                               OBJ.discountPercent;
        this->subTotal                       =                              OBJ.subTotal;
        this->grossTotal                      =                             OBJ.grossTotal;
        this->discountinAmount                 =                            OBJ.discountinAmount;
        this->grandTotal                        =                      OBJ.grandTotal;
        this->taxTotalAmount                     =                          OBJ.taxTotalAmount;
        this->otherLedgersTotal                   =                         OBJ.otherLedgersTotal;
        this->currencyConversionRate               =                        OBJ.currencyConversionRate;
        this->currency                             =                        OBJ.currency;
        this->ProjectId              =                                      OBJ.ProjectId;
        this->AddedBy               =                                      OBJ.AddedBy;
        this->AddedById              =                                      OBJ.AddedById;
        this->DeliveryDate  =                                               OBJ.DeliveryDate;
        this->CompletionProbability=                                        OBJ.CompletionProbability;
        this->RevisionNo            =                                       OBJ.RevisionNo;
        this->ConvertedToSalesOrder=                                        OBJ.ConvertedToSalesOrder;
        this->QuotationPrepared=                                            OBJ.QuotationPrepared;
        this->QuotationDropped=                                             OBJ.QuotationDropped;
        this->QuotationDroppedReason   =                                    OBJ.QuotationDroppedReason;
        this->SalesmanID                =                                   OBJ.SalesmanID;
        this->TermsAndConditionsID       =                                  OBJ.TermsAndConditionsID;
        this->RequirementVoucherNo        =                                 OBJ.RequirementVoucherNo;
        this->Contact=                                                      OBJ.Contact;
        this->LPO                 =                                         OBJ.LPO;
        this->BillingName          =                                        OBJ.BillingName;
        this->prevTransVouchers     =                                       OBJ.prevTransVouchers;
        this->roundOff    =                                                 OBJ.roundOff;
        //       this->AddressBookDataModel ClientAddress;                  //       OBJ.AddressBookDataModel ClientAddress;
        this->status=                                                       OBJ.status;
        this->voucherType=                                                  OBJ.voucherType;
        this->ManagerApprovalStatus=                                        OBJ.ManagerApprovalStatus;
        this->ClientApprovalStatus=                                         OBJ.ClientApprovalStatus;
        //         this->Pax=                                                          OBJ.Pax;
        this->NoOfCopies=                                                   OBJ.NoOfCopies;
        this->ModeOfService=                                                OBJ.ModeOfService;
        this->quantityTotal=                                                OBJ.quantityTotal;
        this->BalanceAmount=                                                OBJ.BalanceAmount;
        this->PaidAmount=                                                   OBJ.PaidAmount;
        this->reference=                                                    OBJ.reference;
        this->Location=                                                     OBJ.Location;
        this->POCName=                                                      OBJ.POCName;
        this->POCEmail=                                                     OBJ.POCEmail;
        this->POCPhone=                                                     OBJ.POCPhone;
        this->kotNumber=                                                    OBJ.kotNumber;
        this->BillSplit=                                                    OBJ.BillSplit;
        this->paymentSplit=                                                 OBJ.paymentSplit;
        this->advanceCash=                                                     OBJ.advanceCash;
        this->balance=                                                      OBJ.balance;
        this->fromGodownName=                                               OBJ.fromGodownName;
        this->toGodownName=                                                 OBJ.toGodownName;
        this->mapList=                                                      OBJ.mapList;
        this->chequeEntry=                                                  OBJ.chequeEntry;
        this->crTotal=                                                      OBJ.crTotal;
        this->drTotal=                                                      OBJ.drTotal;
        this->ledgersTotal=                                                 OBJ.ledgersTotal;


        /*

        this->LPO = A.LPO;
        this->Pax = A.Pax;
        this->status=A.status;
        this->AddedBy=A.AddedBy;
        this->Contact;
        this->POCName;
        balance;
        crTotal;
        drTotal;
        mapList;
        Location;
        POCEmail;
        POCPhone;
        cashPaid;
        currency;
        discount;
        roundOff;
        subTotal;
        AddedById;
        BillSplit;
        ProjectId;
        kotNumber;
        narration;
        reference;
        timestamp;
        NoOfCopies;

        */
        //9995277773
        return  this;

    }




    void aggregate(){

        QString prevID ="";
        float prevPrice = 0;
        QStringList scannedItems;
        QList<float> prices;


        for(int i=0;i<InventoryItems.size();i++){
            for(int j=0;j<InventoryItems.size();j++){
                if( InventoryItems[i].BaseItem.ItemID == InventoryItems[j].BaseItem.ItemID){
                    if(  InventoryItems[i].BaseItem.price ==  InventoryItems[i].BaseItem.price){

                    }
                }
            }


        }
    }



};


Q_DECLARE_METATYPE(GeneralVoucherDataObject)


#endif // GENERALVOUCHERDATAOBJECT_H
