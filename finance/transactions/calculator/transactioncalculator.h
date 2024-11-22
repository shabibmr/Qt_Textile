#ifndef TRANSACTIONCALCULATOR_H
#define TRANSACTIONCALCULATOR_H

#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

class TransactionCalculator
{
public:
    TransactionCalculator();
    static void VoucherCalculateSales(GeneralVoucherDataObject* voucher);
    static void VoucherCalculatePurchase(GeneralVoucherDataObject* voucher);
    static void VoucherCalculateSalesReturn(GeneralVoucherDataObject* voucher);
    static void VoucherCalculatePurchaseReturn(GeneralVoucherDataObject* voucher);
    static void VoucherCalculateLedgersTotal(GeneralVoucherDataObject* voucher);

    static void VoucherCalculateSalesIND(GeneralVoucherDataObject* voucher);
    static void VoucherCalculatePurchaseIND(GeneralVoucherDataObject* voucher);
    static void VoucherCalculateSalesReturnIND(GeneralVoucherDataObject* voucher);
    static void VoucherCalculatePurchaseReturnIND(GeneralVoucherDataObject* voucher);


    static float getRoundOff(float r,int multiple=0){
        //    return r;
        int no = (int)r;

        double fraction = r-no;
        if(fraction <0.125){
            fraction = 0;
        }
        else if(fraction>=0.125&& fraction <0.375)
            fraction = .25;
        else if(fraction>=0.375&& fraction<0.625){
            fraction = .5;
        }
        else if(fraction>=0.625&& fraction<0.875){
            fraction = .75;
        }

        else if(fraction>=0.875){
            fraction = 1;
        }
        return fraction+no;
    }

    static float getRoundOffToOne(float r){

        int no = (int)r;

        double fraction = r-no;

        if(fraction <0.5){
            fraction = 0;
        }
        else {
            fraction = 1;
        }
        return fraction+no;
    }
};

#endif // TRANSACTIONCALCULATOR_H
