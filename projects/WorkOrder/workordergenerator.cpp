#include "workordergenerator.h"

WorkOrderGenerator::WorkOrderGenerator(QObject *parent)
    : QObject(parent)
{
    dbHelper = new WorkOrderDatabaseHelper();
    printer = new CounterPrinterHelper();
    godown = new GodownDatabaseHelper();
}

void WorkOrderGenerator::generateWorkOrders()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QString section="";
    QStringList sections;
    QList<GeneralVoucherDataObject*> vouchers;

    for(int i=0;i<voucher->InventoryItems.size();i++){
        //        qDebug()<<!voucher->InventoryItems[i].BaseItem.isPurchaseItem;
        //        qDebug()<<voucher->InventoryItems[i].BaseItem.prevQty<<voucher->InventoryItems[i].BaseItem.quantity;
        //        qDebug()<< (voucher->InventoryItems[i].BaseItem.listId == 0 ||
        //                      voucher->InventoryItems[i].BaseItem.ItemReqUuid == "X");
        section = voucher->InventoryItems[i].BaseItem.section;
        bool isProd = godown->getGodownById(section)->isProductionCentre;
        if(isProd && !voucher->InventoryItems[i].BaseItem.isPurchaseItem
//                &&
//                voucher->InventoryItems[i].BaseItem.prevQty != voucher->InventoryItems[i].BaseItem.quantity
                && (voucher->InventoryItems[i].BaseItem.listId == 0 ||
                    voucher->InventoryItems[i].BaseItem.ItemReqUuid == "X")){

            if(!sections.contains(section)){
                sections.append(section);
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                qDebug()<<"Creating  new Work ORder";
                GeneralVoucherDataObject *workVoucher= new GeneralVoucherDataObject();
                workVoucher->InventoryItems.append(voucher->InventoryItems[i]);
                workVoucher->SalesmanID = voucher->SalesmanID;
                workVoucher->VoucherDate = QDate::currentDate();
                workVoucher->toGodownID = section;

                //                workVoucher->TransactionId = voucher->TransactionId;
                workVoucher->RequirementVoucherNo = voucher->TransactionId;
                workVoucher->VoucherPrefix = LoginValues::voucherPrefix;
                workVoucher->reference = voucher->reference;
                workVoucher->status = QuotationStatus::SalesOrderCreated;
                workVoucher->ModeOfService = voucher->ModeOfService;
                vouchers.append(workVoucher);
            }
            else{
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                int x = sections.indexOf(section);
                vouchers[x]->InventoryItems.append(voucher->InventoryItems[i]);
            }
        }
    }
    qDebug()<<"Work Orders : "<<vouchers.size();
    for(int i=0;i<vouchers.size();i++){
        qDebug()<<"inserting for section "<<vouchers[i]->toGodownID;
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        dbHelper->insertVoucher(vouchers[i]);

    }
}

void WorkOrderGenerator::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
}

void WorkOrderGenerator::printKot()
{
    printer->printKOT(voucher);
}
