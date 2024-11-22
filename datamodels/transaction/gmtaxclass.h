#ifndef GMTAXCLASS_H
#define GMTAXCLASS_H

#include <QString>
#include "GMTaxDataModel.h"
#include <QList>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include <QSqlQuery>
class GMTaxClass
{
public:
    GMTaxClass();

    GMTaxClassModel baseClass;
    QList<GMTaxClassModel*> dependents;
    QString voucherType;
    int transactionType;


    QString VoucherTYPE="Voucher_TYPE";
    QString VoucherPrefix="Voucher_Prefix";
    QString TransactionType="Transaction_Type";
    QString BaseLedID="BaseLedID";
    QString AffectLedID="AffectLedID";
    QString CrPercent="CrPercent";
    QString CrAdd="CrAdd";
    QString DrPercent="DrPercent";
    QString DrAdd="DrAdd";
    QString description = "Description";

    QString tableName = "TAX_CLASS_MAPPING";



    void calculateVoucher(GeneralVoucherDataObject *voucher,
                          QString voucherType,
                          QString voucherPref="%"
            ){

        QSqlQuery query;
        QMap<QString,GMTaxClass*> list;

        QString qStrBase = "Select ";
        qStrBase += AffectLedID+",";
        qStrBase += CrPercent+",";
        qStrBase += CrAdd+",";
        qStrBase += DrPercent+",";
        qStrBase += DrAdd+" ";

        qStrBase += " from "+ tableName;
        qStrBase += " where ";
        qStrBase += VoucherTYPE + " = :vtype";
        qStrBase += " AND ";
        qStrBase +=  VoucherPrefix + " = :vpref";
        qStrBase += " AND ";
        qStrBase += BaseLedID + " = "+AffectLedID;

        QString qStrList = "Select ";
        qStrList += AffectLedID+",";
        qStrList += CrPercent+",";
        qStrList += CrAdd+",";
        qStrList += DrPercent+",";
        qStrList += DrAdd+" ";

        qStrList += " from "+ tableName;
        qStrList += " where ";
        qStrList += VoucherTYPE + " = :vtype";
        qStrList += " AND ";
        qStrList +=  VoucherPrefix + " like :vpref";
        qStrList += " AND ";
        qStrList += BaseLedID + " <> "+AffectLedID;
        qStrList += " AND ";
        qStrList += BaseLedID + " = :baseledid";

        query.prepare(qStrBase);
        query.bindValue("vtype",voucherType);
        query.bindValue("vpref",voucherPref);

        query.exec();
        while(query.next()){
            GMTaxClass *taxClass = new GMTaxClass();
            taxClass->baseClass.ledgerID = query.value(0).toString();
            taxClass->baseClass.crPercent = query.value(1).toFloat();
            taxClass->baseClass.crAddendum = query.value(2).toFloat();
            taxClass->baseClass.drPercent = query.value(3).toFloat();
            taxClass->baseClass.drAddendum = query.value(4).toFloat();


            QSqlQuery query2;
            query2.prepare(qStrList);
            query2.bindValue("vtype",voucherType);
            query2.bindValue("vpref",voucherPref);
            query2.bindValue("baseledid",taxClass->baseClass.ledgerID);

            query2.exec();
            while(query2.next()){
                GMTaxClassModel *depTaxClass = new GMTaxClassModel();
                depTaxClass->ledgerID = query2.value(0).toString();
                depTaxClass->crPercent = query2.value(1).toFloat();
                depTaxClass->crAddendum = query2.value(2).toFloat();
                depTaxClass->drPercent = query2.value(3).toFloat();
                depTaxClass->drAddendum = query2.value(4).toFloat();

                taxClass->dependents.append(depTaxClass);

            }


            list.insert(taxClass->baseClass.ledgerID,taxClass);
        }


        voucher->quantityTotal = 0;
        voucher->grossTotal = 0;
        voucher->subTotal = 0;
        QMap<QString,GMTaxClass*> ledgersList;
        //READ LEDGERSLIST BASED ON VOUCHERTYPE & PREFIX
        // base class and dependindent classes


        for(int i=0;i<voucher->InventoryItems.length();i++){
            inventoryItemDataModel* item = &voucher->InventoryItems[i].BaseItem;

            float vatAmt  = 0;
            //            float taxRate = 0;

            float itemNetValue = 0;


            voucher->quantityTotal += item->quantity;
            //            taxRate = item->taxRate;

            item->subTotal = (item->price) * item->quantity;

            itemNetValue = item->subTotal - item->discountinAmount;

            item->taxAmount = vatAmt;
            item->grandTotal = itemNetValue + vatAmt;

            voucher->subTotal += item->subTotal;
            voucher->grossTotal += itemNetValue;

            QString ledID;

            switch(transactionType){

            case 1:{
                ledID = item->defaultSalesLedgerID;
                break;
            }
            case 2:{
                ledID = item->defaultPurchaseLedgerID;
                break;
            }
            case 3:{
                ledID = item->DefaultSalesReturnLedgerID;
                break;
            }
            case 4:{
                ledID = item->DefaultPurchaseReturnLedgerID;
                break;
            }
            }

            float value = itemNetValue ;

            if(ledgersList.contains(ledID)){
                GMTaxClassModel* baseItemClass = &ledgersList[ledID]->baseClass;
                baseItemClass->crTotalAmount+=
                        value *baseItemClass->crPercent/100 + baseItemClass->crAddendum;
                baseItemClass->drTotalAmount+=
                        value *baseItemClass->drPercent/100 + baseItemClass->drAddendum;
                if(transactionType == 1 || transactionType==4)
                    baseItemClass->amount = baseItemClass->crTotalAmount - baseItemClass->drTotalAmount;
                else if(transactionType == 2 || transactionType==3)
                    baseItemClass->amount = baseItemClass->drTotalAmount - baseItemClass->crTotalAmount;


                for(int i=0;i<ledgersList[ledID]->dependents.length();i++){
                    GMTaxClassModel* depLedger = ledgersList[ledID]->dependents[i];
                    depLedger->crTotalAmount =
                            baseItemClass->crTotalAmount *depLedger->crPercent/100 + depLedger->crAddendum;
                    depLedger->drTotalAmount =
                            baseItemClass->drTotalAmount *depLedger->drPercent/100 + depLedger->drAddendum;
                    if(transactionType == 1 || transactionType==4)
                        depLedger->amount = depLedger->crTotalAmount - depLedger->drTotalAmount;
                    else if(transactionType == 2 || transactionType==3)
                        depLedger->amount = depLedger->drTotalAmount - depLedger->crTotalAmount;
                }
            }

        }

        for(int i=0;i<voucher->ledgersList.size();i++){
            if(voucher->ledgersList[i].isInvoiceItem){
                qDebug()<<"removing from ledgerslist at "
                       <<voucher->ledgersList[i].LedgerName<<Q_FUNC_INFO;
                voucher->ledgersList.removeAt(i);
                i--;

            }
        }

        QMapIterator<QString, GMTaxClass*> i(ledgersList);
        QJsonObject x;


        voucher->grandTotal = 0;
        while (i.hasNext()) {
            i.next();
            QString colname = i.key();

            GMTaxClass* value = i.value();
            LedgerMasterDataModel ledger;
            ledger.LedgerID = value->baseClass.ledgerID;
            ledger.LedgerName = value->baseClass.ledgerName;
            ledger.crAmount = value->baseClass.crTotalAmount;
            ledger.drAmount = value->baseClass.drTotalAmount;
            ledger.amount = value->baseClass.amount;
            ledger.isInvoiceItem = true;
            voucher->ledgersList.append(ledger);
            voucher->grandTotal += ledger.amount;

            for(int i=0;i<value->dependents.length();i++){
                LedgerMasterDataModel ledger;
                ledger.LedgerID = value->dependents[i]->ledgerID;
                ledger.LedgerName = value->dependents[i]->ledgerName;
                ledger.crAmount = value->dependents[i]->crTotalAmount;
                ledger.drAmount = value->dependents[i]->drTotalAmount;
                ledger.amount = value->dependents[i]->amount;
                ledger.isInvoiceItem = true;
                voucher->ledgersList.append(ledger);
                voucher->grandTotal += ledger.amount;
            }
        }



    }



};

#endif // GMTAXCLASS_H
