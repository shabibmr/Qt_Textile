#include "transactioncalculator.h"

TransactionCalculator::TransactionCalculator()
{
    
}


void TransactionCalculator::VoucherCalculateSales(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;

    QMap<QString,float> LedgersListTemp;
    voucher->quantityTotal = 0;

    for(int i=0;i<voucher->InventoryItems.length();i++){

        float vatAmt  = 0;

        float taxRate = 0;
        float subT = 0;
        float grandTotolItem = 0;


        voucher->quantityTotal += voucher->InventoryItems[i].BaseItem.quantity;
        taxRate = voucher->InventoryItems[i].BaseItem.taxRate;
        //        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;
        subT = (voucher->InventoryItems[i].BaseItem.price) *
               (voucher->InventoryItems[i].BaseItem.quantity - voucher->InventoryItems[i].BaseItem.discQuantity);

        grandTotolItem = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;

        //        voucher->discountinAmount += voucher->InventoryItems[i].BaseItem.discountinAmount;
        //        // qDebug()<<"Discount in Amount : "<<voucher->InventoryItems[i].BaseItem.discountinAmount;

        if(taxRate >= 0)
        {
            vatAmt = grandTotolItem * taxRate/100;

            vatAmt = QString::number(vatAmt,'f',2).toFloat();
        }



        subTotal += subT;
        grossTotal += grandTotolItem;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;

        QString SalesLedger="";
//        QString CGSTLedger="";
//        QString SGSTLedger="";

        QString vatLedger="";

        if(taxRate == 0){
            SalesLedger = "0x7x3";
        }
        else if (taxRate == 5){
            SalesLedger = "0x7x5";
            vatLedger  = "0x2x14x6";
        }

        //Sales Ledgers
        if(LedgersListTemp.keys().contains(SalesLedger)){
            LedgersListTemp[SalesLedger] += grandTotolItem;
        }
        else{
            if(SalesLedger.length()>0)
                LedgersListTemp.insert(SalesLedger,grandTotolItem);
        }

        //VAT ledger
        if(LedgersListTemp.keys().contains(vatLedger)){
            LedgersListTemp[vatLedger] += QString::number(vatAmt,'f',2).toFloat();
            vatTotal += QString::number(vatAmt,'f',2).toFloat();

        }
        else{
            if(vatLedger.length()>0){
                LedgersListTemp.insert(vatLedger, QString::number(vatAmt,'f',2).toFloat());
                vatTotal += QString::number(vatAmt,'f',2).toFloat();

            }
        }

        voucher->InventoryItems[i].BaseItem.subTotal = subT;//+vatAmt;

        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;

        voucher->InventoryItems[i].BaseItem.grandTotal = grandTotolItem + vatAmt;
    }

    voucher->discountinAmount = discountTotal;
    voucher->subTotal = subTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;

    voucher->grandTotal = voucher->grossTotal+ voucher->taxTotalAmount ;

    //    // qDebug()<<"Grand Total : "<<voucher->grandTotal;

//    if(cessTotal > 0 && voucher->ledgerObject.TRN.length()<1 ){
//        LedgersListTemp["0x2x14xCess"] = voucher->cessAmount;
//    }

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
             qDebug()<<"removing from ledgerslist at "
                    <<voucher->ledgersList[i].LedgerName<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;

        }
    }
    if(voucher->paymentSplit == false)
        for(int i=0;i<voucher->ledgersList.size();i++){
//            // qDebug()<<" Total : "<<voucher->grandTotal << "Cr : "<<voucher->ledgersList[i].crAmount << "Dr : "<<voucher->ledgersList[i].drAmount;
            voucher->grandTotal += voucher->ledgersList[i].crAmount - voucher->ledgersList[i].drAmount;
        }

    QMapIterator<QString,float>iterator(LedgersListTemp);

    LedgerMasterDatabaseHelper ledHelper;
    qDebug()<<"Ledger  List Length : "<<voucher->ledgersList.size();
    int iX=0;
    while (iterator.hasNext()) {
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.crAmount = iterator.value();
        ledger.amount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.isInvoiceItem = true;
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        voucher->ledgersList.insert(iX,ledger);
        iX++;
    }


    float round = getRoundOff(voucher->grandTotal);

    voucher->roundOff = round - voucher->grandTotal;

    LedgerMasterDataModel roundledger ;
    if(voucher->roundOff > 0){
        roundledger.crAmount = voucher->roundOff;
    }
    else{
        roundledger.drAmount = -voucher->roundOff;
    }

    roundledger.LedgerID = "0x12x11";

    roundledger.isInvoiceItem = true;


    if(voucher->roundOff != 0){
        roundledger.amount = voucher->roundOff;
        roundledger.LedgerName = "Round Off";
        voucher->ledgersList.insert(iX,roundledger);
        voucher->grandTotal += voucher->roundOff;

    }


    voucher->BalanceAmount = voucher->grandTotal - voucher->PaidAmount;


    voucher->crTotal=0;
    voucher->drTotal=0;
    for(LedgerMasterDataModel obj:voucher->ledgersList){
        qDebug()<<"Ledgers finally : "<<obj.LedgerName;
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
    }
    //       qDebug()<<"Ledger  List Length : "<<voucher->ledgersList.size();
    //    // qDebug()<<"At trans : : : Cr totl"<<voucher->crTotal;
    //    // qDebug()<<"At trans : : : Dr totl"<<voucher->drTotal;
}

void TransactionCalculator::VoucherCalculatePurchase(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO<<voucher->ledgersList.size();

    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;

    voucher->quantityTotal=0;

    QMap<QString,float> LedgersListTemp;
    for(int i=0;i<voucher->InventoryItems.length();i++){
        float vatAmt = 0, taxRate =0, subT =0, grandT =0;
        voucher->quantityTotal+= voucher->InventoryItems[i].BaseItem.quantity;

        taxRate = voucher->InventoryItems[i].BaseItem.taxRate;
        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;

        grandT = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;
        vatAmt = grandT * taxRate/100;


        subTotal += subT;
        grossTotal += grandT;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;

        QString PurchaseLedger="";
        QString vatLedger="";



        if(taxRate == 0){
            PurchaseLedger = "0x8x4";
        }
        else if (taxRate == 5){
            PurchaseLedger = "0x8x8";
            vatLedger  = "0x2x14x7";

        }


        //FOR PURCHASE LEDGER
        if(LedgersListTemp.keys().contains(PurchaseLedger)){
            LedgersListTemp[PurchaseLedger] += grandT;
        }
        else{
            // qDebug()<<"inserting ledgers temp"<<PurchaseLedger;
            LedgersListTemp.insert(PurchaseLedger,grandT);
        }

        //VAT ledger-
        if(LedgersListTemp.keys().contains(vatLedger)){
            LedgersListTemp[vatLedger] += QString::number(vatAmt,'f',2).toFloat();
            vatTotal += QString::number(vatAmt,'f',2).toFloat();
        }
        else{
            if(vatLedger.length()>0){
                // qDebug()<<"inserting ledgers temp"<<CGSTLedger;
                LedgersListTemp.insert(vatLedger, QString::number(vatAmt,'f',2).toFloat());
                vatTotal += QString::number(vatAmt,'f',2).toFloat();
            }
        }

        voucher->InventoryItems[i].BaseItem.subTotal  = subT;//+vatAmt;
        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;
        voucher->InventoryItems[i].BaseItem.grandTotal = grandT + vatAmt;
    }
    voucher->discountinAmount = discountTotal;
    voucher->subTotal = subTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;
    voucher->grandTotal = voucher->grossTotal + vatTotal;

     // qDebug()<<"Voucher TOTal ( Others ): "<<voucher->grandTotal;

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }



    for(int i=0;i<voucher->ledgersList.size();i++){
        voucher->grandTotal += voucher->ledgersList[i].drAmount - voucher->ledgersList[i].crAmount;
    }

//    // qDebug()<<"Led TOTal ( WITH  Others ): "<<voucher->grandTotal;
//    voucher->grandTotal += LedgersTotal;

    // qDebug()<<"ledgerslist temp size"<<LedgersListTemp.size();

    LedgerMasterDatabaseHelper ledHelper;
    //    // qDebug()<<"Ledgers to be auto added : "<<LedgersListTemp.size();
    QMapIterator<QString,float>iterator(LedgersListTemp);
    while (iterator.hasNext()) {
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.drAmount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        ledger.isInvoiceItem = true;
        voucher->ledgersList.append(ledger);
        // qDebug()<<"purchase append"<<voucher->ledgersList.size();
        //        // qDebug()<<iterator.key()<<iterator.value();
    }

    voucher->crTotal=0;
    voucher->drTotal=0;
    for(LedgerMasterDataModel obj:voucher->ledgersList){
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
        // qDebug()<<"Trans vals "<<obj.LedgerName<<obj.crAmount<<obj.drAmount;
    }
//    voucher->grandTotal = voucher->drTotal - voucher->crTotal;
    //    for(int i=0;i<voucher->ledgersList.size();i++){
    //        voucher->grandTotal += voucher->ledgersList[i].drAmount - voucher->ledgersList[i].crAmount;
    //    }

}

void TransactionCalculator::VoucherCalculateSalesReturn(GeneralVoucherDataObject *voucher)
{
    // qDebug()<<Q_FUNC_INFO<<voucher->ledgersList.size();
    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;

    QMap<QString,float> LedgersListTemp;
    voucher->quantityTotal = 0;

    for(int i=0;i<voucher->InventoryItems.length();i++){

        float vatAmt  = 0;
        float vatRate = 0;
        float subT = 0;
        float grandT = 0;

        voucher->quantityTotal+= voucher->InventoryItems[i].BaseItem.quantity;
        vatRate = voucher->InventoryItems[i].BaseItem.taxRate;
        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;

        grandT = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;
        vatAmt = grandT * vatRate/100;

        subTotal += subT;
        grossTotal += grandT;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;

        QString SalesRetLedger;
        QString vatLedger;



        if(vatRate == 0){
            SalesRetLedger = "0x7x3";
        }
        else if (vatRate == 5){
            SalesRetLedger = "0x7xSR5";
            vatLedger  = "0x2x14xOCG25";


        }


        if(LedgersListTemp.keys().contains(SalesRetLedger)){
            LedgersListTemp[SalesRetLedger] += grandT;
        }
        else{
            // qDebug()<<"inserting ledgers temp"<<SalesRetLedger;
            LedgersListTemp.insert(SalesRetLedger,grandT);
        }


        //CGST
        if(LedgersListTemp.keys().contains(vatLedger)){
            LedgersListTemp[vatLedger] += QString::number(vatAmt,'f',2).toFloat();
            vatTotal += QString::number(vatAmt,'f',2).toFloat();
        }
        else{
            if(vatLedger.length()>0){
                // qDebug()<<"inserting ledgers temp"<<CGSTLedger;
                LedgersListTemp.insert(vatLedger, QString::number(vatAmt,'f',2).toFloat());
                vatTotal += QString::number(vatAmt,'f',2).toFloat();
            }
        }


        voucher->InventoryItems[i].BaseItem.subTotal = subT;//+vatAmt;
        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;
        voucher->InventoryItems[i].BaseItem.grandTotal = grandT +vatAmt;
    }

    voucher->subTotal = subTotal;
    voucher->discountinAmount = discountTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;
    voucher->grandTotal = voucher->grossTotal +vatTotal;

//    for(int i=0;i<voucher->ledgersList.size();i++){
//        if(voucher->ledgersList[i].isInvoiceItem){
//            // qDebug()<<"removing from ledgerslist at 468 "<<Q_FUNC_INFO;
//            voucher->ledgersList.removeAt(i);
//            i--;
//        }
//    }

    float LedgersTotal=0;

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }

    for(int i=0;i<voucher->ledgersList.size();i++){
        LedgersTotal += voucher->ledgersList[i].drAmount - voucher->ledgersList[i].crAmount;
    }

    voucher->grandTotal += LedgersTotal;

    // qDebug()<<"ledgerslist temp size"<<LedgersListTemp.size();

    QMapIterator<QString,float>iterator(LedgersListTemp);

    LedgerMasterDatabaseHelper ledHelper;

    while (iterator.hasNext()) {
        // qDebug()<<"terate 1111";
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.drAmount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.isInvoiceItem = true;
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        // qDebug()<<"sales return"<<voucher->ledgersList.size();
        voucher->ledgersList.append(ledger);

        //        // qDebug()<<iterator.key()<<iterator.value();
    }

    // qDebug()<<"outside loop"<<voucher->ledgersList.size();
    voucher->crTotal=0;
    voucher->drTotal=0;

    for(LedgerMasterDataModel obj:voucher->ledgersList){
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
    }
    //    LedgerMasterDataModel roundledger ;
    //    if(voucher->roundOff>0){
    //        roundledger.crAmount = voucher->roundOff;
    //    }
    //    else{
    //        roundledger.drAmount = voucher->roundOff;
    //    }
    //    roundledger.LedgerID = ledHelper->getLedgerIDByName("Round Off");
    //    roundledger.isInvoiceItem = true;
    //    if(voucher->roundOff!=0)
    //        voucher->ledgersList.append(roundledger);

    // qDebug()<<"EOF *****************************************";
}

void TransactionCalculator::VoucherCalculatePurchaseReturn(GeneralVoucherDataObject *voucher)
{
    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;

    QMap<QString,float> LedgersListTemp;
    for(int i=0;i<voucher->InventoryItems.length();i++){

        float vatAmt = 0, vatRate =0, subT =0, grandT =0;
        voucher->quantityTotal+= voucher->InventoryItems[i].BaseItem.quantity;
        vatRate = voucher->InventoryItems[i].BaseItem.taxRate;
        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;

        grandT = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;
        vatAmt = grandT * vatRate/100;

        subTotal += subT;
        grossTotal += grandT;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;

        QString PurchaseLedger;
        QString CGSTLedger;
        QString SGSTLedger;


        if(vatRate == 0){
            PurchaseLedger = "0x8x4";

        }
        else if (vatRate == 5){
            PurchaseLedger = "0x8xPR5";
            CGSTLedger  = "0x2x14xICG25";
            SGSTLedger  = "0x2x14xISG25";

        }
        else if (vatRate == 12){
            PurchaseLedger = "0x8xPR12";
            CGSTLedger  = "0x2x14xICG6";
            SGSTLedger  = "0x2x14xISG6";

        }
        else if (vatRate == 18){
            PurchaseLedger = "0x8xPR18";
            CGSTLedger  = "0x2x14xICG9";
            SGSTLedger  = "0x2x14xISG9";

        }
        else if (vatRate == 28){
            PurchaseLedger = "0x8xPR28";
            CGSTLedger  = "0x2x14xICG14";
            SGSTLedger  = "0x2x14xISG14";
        }


        if(LedgersListTemp.keys().contains(PurchaseLedger)){
            LedgersListTemp[PurchaseLedger] += grandT;
        }
        else{
            LedgersListTemp.insert(PurchaseLedger,grandT);
        }


        // CGST
        if(LedgersListTemp.keys().contains(CGSTLedger)){
            LedgersListTemp[CGSTLedger] += QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(CGSTLedger.length()>0){
                LedgersListTemp.insert(CGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }

        //SGST
        if(LedgersListTemp.keys().contains(SGSTLedger)){
            LedgersListTemp[SGSTLedger] +=  QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(SGSTLedger.length()>0){
                LedgersListTemp.insert(SGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }


        voucher->InventoryItems[i].BaseItem.subTotal = subT;//+vatAmt;
        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;
        voucher->InventoryItems[i].BaseItem.grandTotal = grandT +vatAmt;
    }
    voucher->discountinAmount = discountTotal;
    voucher->subTotal = subTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;
    voucher->grandTotal = voucher->grossTotal +vatTotal;

    //    float rounded = getRoundOff(voucher->grandTotal,0);
    //    voucher->roundOff = rounded - voucher->grandTotal  ;
    //    voucher->grandTotal = rounded;

    //    ui->roundOff->setText(QString::number(voucher->roundOff,'f',2));

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }

    float LedgersTotal=0;

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }

    for(int i=0;i<voucher->ledgersList.size();i++){
        LedgersTotal += voucher->ledgersList[i].crAmount - voucher->ledgersList[i].drAmount;
    }

    voucher->grandTotal += LedgersTotal;

    LedgerMasterDatabaseHelper ledHelper;
    QMapIterator<QString,float>iterator(LedgersListTemp);
    while (iterator.hasNext()) {
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.crAmount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        ledger.isInvoiceItem = true;
        voucher->ledgersList.append(ledger);

        //        // qDebug()<<iterator.key()<<iterator.value();
    }

    voucher->crTotal=0;
    voucher->drTotal=0;
    for(LedgerMasterDataModel obj:voucher->ledgersList){
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
    }
}




void TransactionCalculator::VoucherCalculateSalesIND(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;

    
    QMap<QString,float> LedgersListTemp;
    voucher->quantityTotal = 0;
    
    for(int i=0;i<voucher->InventoryItems.length();i++){

        float vatAmt  = 0;
        float vatamtBy_2 = 0;
        float taxRate = 0;
        float subT = 0;
        float grandTotolItem = 0;
        float itemCess = 0;
        
        voucher->quantityTotal += voucher->InventoryItems[i].BaseItem.quantity;
        taxRate = voucher->InventoryItems[i].BaseItem.taxRate;
        //        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;
        subT = (voucher->InventoryItems[i].BaseItem.price) * voucher->InventoryItems[i].BaseItem.quantity;

        grandTotolItem = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;

        //        voucher->discountinAmount += voucher->InventoryItems[i].BaseItem.discountinAmount;
        //        // qDebug()<<"Discount in Amount : "<<voucher->InventoryItems[i].BaseItem.discountinAmount;

        if(taxRate >= 0)
        {
            vatAmt = grandTotolItem * taxRate/100;
            vatamtBy_2 = grandTotolItem * taxRate/200;
            vatAmt = QString::number(vatAmt,'f',2).toFloat();
        }
        
        if(taxRate>5 && voucher->ledgerObject.TRN.length()<3){
            itemCess = grandTotolItem * 1/100;
            itemCess = QString::number(itemCess,'f',2).toFloat();

        }

        subTotal += subT;
        grossTotal += grandTotolItem;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;

        QString SalesLedger="";
        QString CGSTLedger="";
        QString SGSTLedger="";
        
        if(taxRate == 0){
            SalesLedger = "0x7x3";
        }
        else if (taxRate == 5){
            SalesLedger = "0x7xSV5";
            CGSTLedger  = "0x2x14xOCG25";
            SGSTLedger  = "0x2x14xOSG25";
        }
        else if (taxRate == 12){
            SalesLedger = "0x7xSV12";
            CGSTLedger  = "0x2x14xOCG6";
            SGSTLedger  = "0x2x14xOSG6";
        }
        else if (taxRate == 18){
            SalesLedger = "0x7xSV18";
            CGSTLedger  = "0x2x14xOCG9";
            SGSTLedger  = "0x2x14xOSG9";
            
        }
        else if (taxRate == 28){
            SalesLedger = "0x7xSV28";
            CGSTLedger  = "0x2x14xOCG14";
            SGSTLedger  = "0x2x14xOSG14";
        }
        
        //Sales Ledgers
        if(LedgersListTemp.keys().contains(SalesLedger)){
            LedgersListTemp[SalesLedger] += grandTotolItem;
        }
        else{
            if(SalesLedger.length()>0)
                LedgersListTemp.insert(SalesLedger,grandTotolItem);
        }
        
        //CGST
        if(LedgersListTemp.keys().contains(CGSTLedger)){
            LedgersListTemp[CGSTLedger] += QString::number(vatamtBy_2,'f',2).toFloat();
//            qDebug()<<" Value : "<<LedgersListTemp[CGSTLedger];
//            qDebug()<<" Value(without ) : "<<vatAmt/2;
//            qDebug()<<" Vat/2 (without): "<<vatamtBy_2;
//            qDebug()<<" Vat/2 : "<<QString::number(vatamtBy_2,'f',2).toFloat();
            vatTotal += QString::number(vatamtBy_2,'f',2).toFloat();

        }
        else{
            if(CGSTLedger.length()>0){
                LedgersListTemp.insert(CGSTLedger, QString::number(vatamtBy_2,'f',2).toFloat());
                vatTotal += QString::number(vatamtBy_2,'f',2).toFloat();
//                qDebug()<<" Value : "<<LedgersListTemp[CGSTLedger];
//                qDebug()<<" Value(without ) : "<<vatAmt/2;
//                qDebug()<<" Vat/2 (without): "<<vatamtBy_2;
//                qDebug()<<" Vat/2 : "<<QString::number(vatamtBy_2,'f',2).toFloat();;
//                qDebug()<<"grand Total Item : "<<grandTotolItem;

            }
        }
        
        //SGST
        if(LedgersListTemp.keys().contains(SGSTLedger)){
            LedgersListTemp[SGSTLedger] +=  QString::number(vatamtBy_2,'f',2).toFloat();
            vatTotal += QString::number(vatamtBy_2,'f',2).toFloat();
        }
        else{
            if(SGSTLedger.length()>0){
                LedgersListTemp.insert(SGSTLedger, QString::number(vatamtBy_2,'f',2).toFloat());
                vatTotal += QString::number(vatamtBy_2,'f',2).toFloat();
            }
        }
        
        voucher->InventoryItems[i].BaseItem.subTotal = subT;//+vatAmt;
//        // qDebug()<<"tax amt update"<<voucher->InventoryItems[i].BaseItem.taxAmount;
        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;
//        // qDebug()<<"tax amt updated to"<<voucher->InventoryItems[i].BaseItem.taxAmount;
        voucher->InventoryItems[i].BaseItem.grandTotal = grandTotolItem +vatAmt;
    }
    
    voucher->discountinAmount = discountTotal;
    voucher->subTotal = subTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;

    voucher->grandTotal = voucher->grossTotal+ voucher->taxTotalAmount ;

    //    // qDebug()<<"Grand Total : "<<voucher->grandTotal;

//    if(cessTotal > 0 && voucher->ledgerObject.TRN.length()<1 ){
//        LedgersListTemp["0x2x14xCess"] = voucher->cessAmount;
//    }
    
    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
             qDebug()<<"removing from ledgerslist at "
                    <<voucher->ledgersList[i].LedgerName<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;

        }
    }
    if(voucher->paymentSplit == false)
        for(int i=0;i<voucher->ledgersList.size();i++){
//            // qDebug()<<" Total : "<<voucher->grandTotal << "Cr : "<<voucher->ledgersList[i].crAmount << "Dr : "<<voucher->ledgersList[i].drAmount;
            voucher->grandTotal += voucher->ledgersList[i].crAmount - voucher->ledgersList[i].drAmount;
        }
    
    QMapIterator<QString,float>iterator(LedgersListTemp);
    
    LedgerMasterDatabaseHelper ledHelper;
    qDebug()<<"Ledger  List Length : "<<voucher->ledgersList.size();
    int iX=0;
    while (iterator.hasNext()) {
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.crAmount = iterator.value();
        ledger.amount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.isInvoiceItem = true;
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        voucher->ledgersList.insert(iX,ledger);
        iX++;
    }

//    for(LedgerMasterDataModel obj:voucher->ledgersList){
//        // qDebug()<<obj.LedgerName<<obj.crAmount<<obj.drAmount;
//    }
    
    //    // qDebug()<<"grand total"<<voucher->grandTotal<<voucher->grossTotal;
    
    float round = getRoundOffToOne(voucher->grandTotal);

    voucher->roundOff = round - voucher->grandTotal;

    //    // qDebug()<<"Round off : "<<voucher->roundOff << "Total : "<<voucher->grandTotal;

    LedgerMasterDataModel roundledger ;
    if(voucher->roundOff > 0){
        roundledger.crAmount = voucher->roundOff;
    }
    else{
        roundledger.drAmount = -voucher->roundOff;
    }
    
    roundledger.LedgerID = "0x12x11";
    
    roundledger.isInvoiceItem = true;


    if(voucher->roundOff != 0){
        roundledger.amount = voucher->roundOff;
        roundledger.LedgerName = "Round Off";
        voucher->ledgersList.insert(iX,roundledger);
        voucher->grandTotal += voucher->roundOff;

    }
    voucher->BalanceAmount = voucher->grandTotal - voucher->PaidAmount;
    
    //    // qDebug()<<"Grand Total : "<<voucher->grandTotal;
    //    // qDebug()<<"Round Off: "<<voucher->roundOff;
    //for(int i=0;i<voucher->ledgersList.size();i++){
    //        // qDebug()<<"Ledger : "<<voucher->ledgersList[i].LedgerName;
    //        // qDebug()<<"Cr Amount : "<<voucher->ledgersList[i].crAmount;
    //        // qDebug()<<"Dr Amount : "<<voucher->ledgersList[i].drAmount;

    // }
    voucher->crTotal=0;
    voucher->drTotal=0;
    for(LedgerMasterDataModel obj:voucher->ledgersList){
        qDebug()<<"Ledgers finally : "<<obj.LedgerName;
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
    }
    qDebug()<<"Ledger  List Length : "<<voucher->ledgersList.size();
    //    // qDebug()<<"At trans : : : Cr totl"<<voucher->crTotal;
    //    // qDebug()<<"At trans : : : Dr totl"<<voucher->drTotal;
}

void TransactionCalculator::VoucherCalculatePurchaseIND(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO<<voucher->ledgersList.size();

    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;

    voucher->quantityTotal=0;
    
    QMap<QString,float> LedgersListTemp;
    for(int i=0;i<voucher->InventoryItems.length();i++){
        float vatAmt = 0, taxRate =0, subT =0, grandT =0;
        voucher->quantityTotal+= voucher->InventoryItems[i].BaseItem.quantity;
        
        taxRate = voucher->InventoryItems[i].BaseItem.taxRate;
        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;
        
        grandT = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;
        vatAmt = grandT * taxRate/100;

        
        subTotal += subT;
        grossTotal += grandT;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;
        
        QString PurchaseLedger;
        QString CGSTLedger;
        QString SGSTLedger;
        
        
        if(taxRate == 0){
            PurchaseLedger = "0x8x4";
        }
        else if (taxRate == 5){
            PurchaseLedger = "0x8xPV5";
            CGSTLedger  = "0x2x14xICG25";
            SGSTLedger  = "0x2x14xISG25";
        }
        else if (taxRate == 12){
            PurchaseLedger = "0x8xPV12";
            CGSTLedger  = "0x2x14xICG6";
            SGSTLedger  = "0x2x14xISG6";
        }
        else if (taxRate == 18){
            PurchaseLedger = "0x8xPV18";
            CGSTLedger  = "0x2x14xICG9";
            SGSTLedger  = "0x2x14xISG9";
        }
        else if (taxRate == 28){
            PurchaseLedger = "0x8xPV28";
            CGSTLedger  = "0x2x14xICG14";
            SGSTLedger  = "0x2x14xISG14";
        }
        
        //FOR PURCHASE LEDGER
        if(LedgersListTemp.keys().contains(PurchaseLedger)){
            LedgersListTemp[PurchaseLedger] += grandT;
        }
        else{
            // qDebug()<<"inserting ledgers temp"<<PurchaseLedger;
            LedgersListTemp.insert(PurchaseLedger,grandT);
        }
        
        //CGST
        if(LedgersListTemp.keys().contains(CGSTLedger)){
            LedgersListTemp[CGSTLedger] += QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(CGSTLedger.length()>0){
                // qDebug()<<"inserting ledgers temp"<<CGSTLedger;
                LedgersListTemp.insert(CGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }

        //SGST
        if(LedgersListTemp.keys().contains(SGSTLedger)){
            LedgersListTemp[SGSTLedger] +=  QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(SGSTLedger.length()>0){
                // qDebug()<<"inserting ledgers temp"<<SGSTLedger;
                LedgersListTemp.insert(SGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }
        
        voucher->InventoryItems[i].BaseItem.subTotal  = subT;//+vatAmt;
        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;
        voucher->InventoryItems[i].BaseItem.grandTotal = grandT + vatAmt;
    }
    voucher->discountinAmount = discountTotal;
    voucher->subTotal = subTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;
    voucher->grandTotal = voucher->grossTotal + vatTotal;
    
     // qDebug()<<"Voucher TOTal ( Others ): "<<voucher->grandTotal;

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }



    for(int i=0;i<voucher->ledgersList.size();i++){
        voucher->grandTotal += voucher->ledgersList[i].drAmount - voucher->ledgersList[i].crAmount;
    }

//    // qDebug()<<"Led TOTal ( WITH  Others ): "<<voucher->grandTotal;
//    voucher->grandTotal += LedgersTotal;
    
    // qDebug()<<"ledgerslist temp size"<<LedgersListTemp.size();

    LedgerMasterDatabaseHelper ledHelper;
    //    // qDebug()<<"Ledgers to be auto added : "<<LedgersListTemp.size();
    QMapIterator<QString,float>iterator(LedgersListTemp);
    while (iterator.hasNext()) {
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.drAmount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        ledger.isInvoiceItem = true;
        voucher->ledgersList.append(ledger);
        // qDebug()<<"purchase append"<<voucher->ledgersList.size();
        //        // qDebug()<<iterator.key()<<iterator.value();
    }
    
    voucher->crTotal=0;
    voucher->drTotal=0;
    for(LedgerMasterDataModel obj:voucher->ledgersList){
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
        // qDebug()<<"Trans vals "<<obj.LedgerName<<obj.crAmount<<obj.drAmount;
    }
//    voucher->grandTotal = voucher->drTotal - voucher->crTotal;
    //    for(int i=0;i<voucher->ledgersList.size();i++){
    //        voucher->grandTotal += voucher->ledgersList[i].drAmount - voucher->ledgersList[i].crAmount;
    //    }
    
}

void TransactionCalculator::VoucherCalculateSalesReturnIND(GeneralVoucherDataObject *voucher)
{
    // qDebug()<<Q_FUNC_INFO<<voucher->ledgersList.size();
    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;
    
    QMap<QString,float> LedgersListTemp;
    voucher->quantityTotal = 0;
    
    for(int i=0;i<voucher->InventoryItems.length();i++){
        
        float vatAmt  = 0;
        float vatRate = 0;
        float subT = 0;
        float grandT = 0;
        
        voucher->quantityTotal+= voucher->InventoryItems[i].BaseItem.quantity;
        vatRate = voucher->InventoryItems[i].BaseItem.taxRate;
        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;
        
        grandT = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;
        vatAmt = grandT * vatRate/100;
        
        subTotal += subT;
        grossTotal += grandT;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;
        
        QString SalesRetLedger;
        QString CGSTLedger;
        QString SGSTLedger;
        
        
        if(vatRate == 0){
            SalesRetLedger = "0x7x3";
            
        }
        else if (vatRate == 5){
            SalesRetLedger = "0x7xSR5";
            CGSTLedger  = "0x2x14xOCG25";
            SGSTLedger  = "0x2x14xOSG25";
            
        }
        else if (vatRate == 12){
            SalesRetLedger = "0x7xSR12";
            CGSTLedger  = "0x2x14xOCG6";
            SGSTLedger  = "0x2x14xOSG6";
            
        }
        else if (vatRate == 18){
            SalesRetLedger = "0x7xSR18";
            CGSTLedger  = "0x2x14xOCG9";
            SGSTLedger  = "0x2x14xOSG9";
            
        }
        else if (vatRate == 28){
            SalesRetLedger = "0x7xSR28";
            CGSTLedger  = "0x2x14xOCG14";
            SGSTLedger  = "0x2x14xOSG14";
        }
        
        if(LedgersListTemp.keys().contains(SalesRetLedger)){
            LedgersListTemp[SalesRetLedger] += grandT;
        }
        else{
            // qDebug()<<"inserting ledgers temp"<<SalesRetLedger;
            LedgersListTemp.insert(SalesRetLedger,grandT);
        }
        
        
        //CGST
        if(LedgersListTemp.keys().contains(CGSTLedger)){
            LedgersListTemp[CGSTLedger] += QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(CGSTLedger.length()>0){
                // qDebug()<<"inserting ledgers temp"<<CGSTLedger;
                LedgersListTemp.insert(CGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }

        //SGST
        if(LedgersListTemp.keys().contains(SGSTLedger)){
            LedgersListTemp[SGSTLedger] +=  QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(SGSTLedger.length()>0){
                // qDebug()<<"inserting ledgers temp"<<SGSTLedger;
                LedgersListTemp.insert(SGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }

        voucher->InventoryItems[i].BaseItem.subTotal = subT;//+vatAmt;
        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;
        voucher->InventoryItems[i].BaseItem.grandTotal = grandT +vatAmt;
    }
    
    voucher->subTotal = subTotal;
    voucher->discountinAmount = discountTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;
    voucher->grandTotal = voucher->grossTotal +vatTotal;
        
//    for(int i=0;i<voucher->ledgersList.size();i++){
//        if(voucher->ledgersList[i].isInvoiceItem){
//            // qDebug()<<"removing from ledgerslist at 468 "<<Q_FUNC_INFO;
//            voucher->ledgersList.removeAt(i);
//            i--;
//        }
//    }
    
    float LedgersTotal=0;

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }

    for(int i=0;i<voucher->ledgersList.size();i++){
        LedgersTotal += voucher->ledgersList[i].drAmount - voucher->ledgersList[i].crAmount;
    }

    voucher->grandTotal += LedgersTotal;

    // qDebug()<<"ledgerslist temp size"<<LedgersListTemp.size();
    
    QMapIterator<QString,float>iterator(LedgersListTemp);
    
    LedgerMasterDatabaseHelper ledHelper;
    
    while (iterator.hasNext()) {
        // qDebug()<<"terate 1111";
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.drAmount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.isInvoiceItem = true;
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        // qDebug()<<"sales return"<<voucher->ledgersList.size();
        voucher->ledgersList.append(ledger);
        
        //        // qDebug()<<iterator.key()<<iterator.value();
    }

    // qDebug()<<"outside loop"<<voucher->ledgersList.size();
    voucher->crTotal=0;
    voucher->drTotal=0;

    for(LedgerMasterDataModel obj:voucher->ledgersList){
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
    }
    //    LedgerMasterDataModel roundledger ;
    //    if(voucher->roundOff>0){
    //        roundledger.crAmount = voucher->roundOff;
    //    }
    //    else{
    //        roundledger.drAmount = voucher->roundOff;
    //    }
    //    roundledger.LedgerID = ledHelper->getLedgerIDByName("Round Off");
    //    roundledger.isInvoiceItem = true;
    //    if(voucher->roundOff!=0)
    //        voucher->ledgersList.append(roundledger);
    
    // qDebug()<<"EOF *****************************************";
}

void TransactionCalculator::VoucherCalculatePurchaseReturnIND(GeneralVoucherDataObject *voucher)
{
    float subTotal =0;
    float vatTotal =0;
    float discountTotal =0;
    float grossTotal = 0;
    
    QMap<QString,float> LedgersListTemp;
    for(int i=0;i<voucher->InventoryItems.length();i++){
        
        float vatAmt = 0, vatRate =0, subT =0, grandT =0;
        voucher->quantityTotal+= voucher->InventoryItems[i].BaseItem.quantity;
        vatRate = voucher->InventoryItems[i].BaseItem.taxRate;
        subT = voucher->InventoryItems[i].BaseItem.price *voucher->InventoryItems[i].BaseItem.quantity ;
        
        grandT = subT - voucher->InventoryItems[i].BaseItem.discountinAmount;
        vatAmt = grandT * vatRate/100;
        
        subTotal += subT;
        grossTotal += grandT;

        discountTotal +=  voucher->InventoryItems[i].BaseItem.discountinAmount;
        
        QString PurchaseLedger;
        QString CGSTLedger;
        QString SGSTLedger;
        
        
        if(vatRate == 0){
            PurchaseLedger = "0x8x4";
            
        }
        else if (vatRate == 5){
            PurchaseLedger = "0x8xPR5";
            CGSTLedger  = "0x2x14xICG25";
            SGSTLedger  = "0x2x14xISG25";
            
        }
        else if (vatRate == 12){
            PurchaseLedger = "0x8xPR12";
            CGSTLedger  = "0x2x14xICG6";
            SGSTLedger  = "0x2x14xISG6";
            
        }
        else if (vatRate == 18){
            PurchaseLedger = "0x8xPR18";
            CGSTLedger  = "0x2x14xICG9";
            SGSTLedger  = "0x2x14xISG9";
            
        }
        else if (vatRate == 28){
            PurchaseLedger = "0x8xPR28";
            CGSTLedger  = "0x2x14xICG14";
            SGSTLedger  = "0x2x14xISG14";
        }
        
        
        if(LedgersListTemp.keys().contains(PurchaseLedger)){
            LedgersListTemp[PurchaseLedger] += grandT;
        }
        else{
            LedgersListTemp.insert(PurchaseLedger,grandT);
        }
        
        
        // CGST
        if(LedgersListTemp.keys().contains(CGSTLedger)){
            LedgersListTemp[CGSTLedger] += QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(CGSTLedger.length()>0){
                LedgersListTemp.insert(CGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }

        //SGST
        if(LedgersListTemp.keys().contains(SGSTLedger)){
            LedgersListTemp[SGSTLedger] +=  QString::number(vatAmt/2,'f',2).toFloat();
            vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
        }
        else{
            if(SGSTLedger.length()>0){
                LedgersListTemp.insert(SGSTLedger, QString::number(vatAmt/2,'f',2).toFloat());
                vatTotal += QString::number(vatAmt/2,'f',2).toFloat();
            }
        }
        
        
        voucher->InventoryItems[i].BaseItem.subTotal = subT;//+vatAmt;
        voucher->InventoryItems[i].BaseItem.taxAmount = vatAmt;
        voucher->InventoryItems[i].BaseItem.grandTotal = grandT +vatAmt;
    }
    voucher->discountinAmount = discountTotal;
    voucher->subTotal = subTotal;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->taxTotalAmount  = vatTotal;
    voucher->grandTotal = voucher->grossTotal +vatTotal;
    
    //    float rounded = getRoundOff(voucher->grandTotal,0);
    //    voucher->roundOff = rounded - voucher->grandTotal  ;
    //    voucher->grandTotal = rounded;
    
    //    ui->roundOff->setText(QString::number(voucher->roundOff,'f',2));
    
    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }

    float LedgersTotal=0;

    for(int i=0;i<voucher->ledgersList.size();i++){
        if(voucher->ledgersList[i].isInvoiceItem){
            // qDebug()<<"removing from ledgerslist at "<<Q_FUNC_INFO;
            voucher->ledgersList.removeAt(i);
            i--;
        }
    }

    for(int i=0;i<voucher->ledgersList.size();i++){
        LedgersTotal += voucher->ledgersList[i].crAmount - voucher->ledgersList[i].drAmount;
    }

    voucher->grandTotal += LedgersTotal;
    
    LedgerMasterDatabaseHelper ledHelper;
    QMapIterator<QString,float>iterator(LedgersListTemp);
    while (iterator.hasNext()) {
        iterator.next();
        LedgerMasterDataModel ledger;
        ledger.crAmount = iterator.value();
        ledger.LedgerID = iterator.key();
        ledger.LedgerName = ledHelper.getLedgerNameByID(ledger.LedgerID);
        ledger.isInvoiceItem = true;
        voucher->ledgersList.append(ledger);
        
        //        // qDebug()<<iterator.key()<<iterator.value();
    }

    voucher->crTotal=0;
    voucher->drTotal=0;
    for(LedgerMasterDataModel obj:voucher->ledgersList){
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
    }
}

void TransactionCalculator::VoucherCalculateLedgersTotal(GeneralVoucherDataObject *voucher)
{
    voucher->ledgersTotal = 0;
    voucher->crTotal=0;
    voucher->drTotal=0;
    
    for(LedgerMasterDataModel obj:voucher->ledgersList){
        voucher->crTotal += obj.crAmount;
        voucher->drTotal += obj.drAmount;
    }
    voucher->ledgersTotal += voucher->drTotal - voucher->crTotal;
    voucher->grandTotal =  voucher->ledgersTotal>0?voucher->ledgersTotal:-1*voucher->ledgersTotal;
}


