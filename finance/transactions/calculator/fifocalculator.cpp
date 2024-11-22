#include "fifocalculator.h"

FifoCalculator::FifoCalculator()
{
    transHelper = new TransactionFifoDatabaseHelper();
    shelper = new SalesInventoryItemDatabaseHelper();
}

void FifoCalculator::setFifoData(GeneralVoucherDataObject *voucher,inventoryItemDataModel *item, QDate vDate, int index)
{

    qDebug()<<Q_FUNC_INFO<<index<<item->godownList[index].fifoList.size();

    qDebug()<<"fifo before";
    for(ItemFifoDataModel *fifo:item->godownList[index].fifoList)
        qDebug()<<fifo->VoucherID<<fifo->VoucherType<<fifo->VoucherPrefix<<fifo->crQty<<fifo->Price<<fifo->updated;

    QSqlQueryModel* model = transHelper->getItemQtyListFifo(voucher, item->ItemID, vDate, item->godownList[index].fromGodown);

    float balance = item->godownList[index].crQty;
    qDebug()<<"line27 balance"<<balance<<item->godownList[index].fifoList.size();
    if(item->godownList[index].fifoList.size()>0){
        for(ItemFifoDataModel *fifo:item->godownList[index].fifoList)
            balance -= fifo->crQty;
    }
    qDebug()<<Q_FUNC_INFO<<"Setting fifo for "<<balance;
    for(int i=0;i<model->rowCount();i++){
        if(balance == 0)
            return;

        ItemFifoDataModel *obj = new ItemFifoDataModel();

        obj->VoucherID = model->record(i).value(0).toString();
        obj->VoucherType = model->record(i).value(1).toString();
        obj->VoucherPrefix = model->record(i).value(2).toString();
        obj->ItemID = model->record(i).value(3).toString();
        obj->ItemReqId = model->record(i).value(4).toString();
        obj->drQty = model->record(i).value(5).toFloat();
        obj->Price = model->record(i).value(6).toFloat();

        if(balance > 0){
            qDebug()<<"BALANCE"<<balance<<"next possible fifo qty"<<obj->drQty<<obj->Price;

            // check if there is an existing fifo entry for this
            for(int j=0; j<item->godownList[index].fifoList.size(); j++){
                if(item->godownList[index].fifoList[j]->updated &&
                        compareFifo(item->godownList[index].fifoList[j], obj)){
                    obj->drQty -= item->godownList[index].fifoList[j]->drQty;
                    break;
                }
            }

            if(obj->drQty < balance){
                obj->crQty = obj->drQty ;
                balance -= obj->drQty ;
            }
            else{
                obj->crQty = balance;
                balance = 0;
            }
            obj->updated=true;
            obj->drQty =0;

            item->godownList[index].fifoList.append(obj);
            qDebug()<<"added in fifo: cons, cr, bal"<<obj->consumedQty<<obj->crQty<<balance;
        }
    }

    qDebug()<<"65 fifolist"<<item->godownList[index].fifoList.size();

    if(balance > 0)
    {
        qDebug()<<"hanging entry";
        ItemFifoDataModel *fifo = new ItemFifoDataModel();
        fifo->ItemID =item->ItemID;
        fifo->crQty = balance;
        fifo->Price = item->price;

        item->godownList[index].fifoList.append(fifo);
    }

    if(balance < 0)
    {
        qDebug()<<"negative bal"<<balance;
        for(int i=item->godownList[index].fifoList.size()-1; i>=0; i--){
            if(balance == 0)
                return;
            if(item->godownList[index].fifoList[i]->crQty <= qFabs(balance)){
                balance += item->godownList[index].fifoList[i]->crQty;
                item->godownList[index].fifoList.removeAt(i);
                qDebug()<<"removed fifo row";
            }
            else if(item->godownList[index].fifoList[i]->crQty > qFabs(balance)){
                qDebug()<<"vlues are"<<item->godownList[index].fifoList[i]->crQty<<qFabs(balance);
                item->godownList[index].fifoList[i]->crQty -= qFabs(balance); // since bal is neqative
                qDebug()<<"changed to"<<item->godownList[index].fifoList[i]->crQty;
                balance = 0;
            }
        }
    }

    return;

}

void FifoCalculator::setItemQtyByFifo(GeneralVoucherDataObject *voucher, int row, int godownIndex, bool showPopup)
{



    return;
    qDebug()<<Q_FUNC_INFO<<godownIndex<<voucher->voucherType<<voucher->InventoryItems[row].BaseItem.godownList.size();

    showPopup = false; // remove to show popup

    if(!voucher->InventoryItems[row].BaseItem.isStockItem &&
            !QString::compare(voucher->voucherType,
                              GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder),
                              Qt::CaseInsensitive) &&
            !QString::compare(voucher->voucherType,
                              GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder),
                              Qt::CaseInsensitive)){
        qDebug()<<"no fifo entry for purchase/sales order or if item is not stockItem";
        return;
    }
    qDebug()<<"cr dr"<<voucher->InventoryItems[row].BaseItem.godownList[godownIndex].crQty
           <<voucher->InventoryItems[row].BaseItem.godownList[godownIndex].drQty;

    qDebug()<<"check for cr/dr qty ";

    if (voucher->InventoryItems[row].BaseItem.godownList[godownIndex].crQty >0) {
        qDebug()<<"CR QTY for FIFO ";
        float currentStock = shelper->getClsStkOfItemTillDate(voucher->InventoryItems[row].BaseItem.ItemID,
                                                              voucher->VoucherDate,
                                                              voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fromGodown);


        if(voucher->status != QuotationStatus::Started){
            qDebug()<<"QUANTITY EDIT";
            // If qty increased, check stock
            if(voucher->InventoryItems[row].BaseItem.quantity >
                    voucher->InventoryItems[row].BaseItem.prevQty){
                if(voucher->InventoryItems[row].BaseItem.quantity -
                        voucher->InventoryItems[row].BaseItem.prevQty
                        > currentStock ){
                    //                    qDebug()<<"New qty greater than stock"<<currentStock<<voucher->InventoryItems[row].BaseItem.quantity<<voucher->InventoryItems[row].BaseItem.prevQty;
                    //                    voucher->InventoryItems[row].BaseItem.quantity = voucher->InventoryItems[row].BaseItem.prevQty>0?voucher->InventoryItems[row].BaseItem.prevQty:0;
                    //                    QMessageBox box; box.setText("Quantity cannot be increased more than stock. Available stock is " +
                    //                                                 QString::number(currentStock,'f',3)); box.exec();
                    //                    return;
                }

            }
            else if(voucher->InventoryItems[row].BaseItem.quantity <
                    voucher->InventoryItems[row].BaseItem.prevQty){
                //12/02 - uncomment if issue when quantity reduced
                //                inventoryItemDataModel item = voucher->InventoryItems[row].BaseItem;
                //                item.quantity = voucher->InventoryItems[row].BaseItem.quantity - voucher->InventoryItems[row].BaseItem.prevQty;

                //                voucher->deletedItems.append(item);
            }

        }

        else{
            if(currentStock - voucher->InventoryItems[row].BaseItem.quantity < 0
                    && !GMVoucherTypes::compareVoucherType( voucher->voucherType , GMVoucherTypes::SalesOrder)
                    && !voucher->InventoryItems[row].BaseItem.isStockItem){
                // voucher->InventoryItems[row].BaseItem.quantity = currentStock>0?currentStock:0;
                //QMessageBox box; box.setText("#Quantity cannot be more than current stock. Available stock is " +
                // QString::number(currentStock,'f',3)); box.exec();
                // return;
            }
        }

        setFifoData(voucher, &voucher->InventoryItems[row].BaseItem, voucher->VoucherDate, godownIndex);

        qDebug()<<"Fifo set";
        int i=0;
        for(ItemFifoDataModel *fifo:voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fifoList){
            qDebug()<<fifo->VoucherID<<fifo->VoucherType<<fifo->VoucherPrefix<<fifo->crQty<<fifo->Price;
            //            qDebug()<<voucher->voucherNumber<<voucher->voucherType<<voucher->VoucherPrefix;
            // if(fifo->VoucherID == voucher->voucherNumber &&
            //  fifo->VoucherType == voucher->voucherType &&
            //  fifo->VoucherPrefix == voucher->VoucherPrefix){
            // qDebug()<<"Fifo from same voucher";
            //  voucher->InventoryItems[row].BaseItem->godownList[index].fifoList.removeAt(i);
            // }

            if(voucher->voucherType == "Stock Request")
                fifo->drQty = fifo->crQty;
            i++;
        }

        if(showPopup){
            FifoPopupDialog *fifoDialog = new FifoPopupDialog(&voucher->InventoryItems[row].BaseItem);
            fifoDialog->show();
        }

    }

    if(voucher->InventoryItems[row].BaseItem.drQty >0){

        qDebug()<<"DR"<<voucher->InventoryItems[row].BaseItem.godownList[godownIndex].drQty;

        if(voucher->status != QuotationStatus::Started &&
                ! QString::compare(voucher->voucherType,
                                   GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder),
                                   Qt::CaseInsensitive)){

            qDebug()<<"item edit"<<voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fifoList.size();
            // If qty increased, check stock
            if(voucher->InventoryItems[row].BaseItem.prevQty == voucher->InventoryItems[row].BaseItem.consumedQty
                    && voucher->InventoryItems[row].BaseItem.prevQty >0 ){
                //                QMessageBox box; box.setText("Item fully consumed, cannot edit " ); box.exec();
                //                voucher->InventoryItems[row].BaseItem.quantity = voucher->InventoryItems[row].BaseItem.prevQty;
                //                return;
            }

            else if(voucher->InventoryItems[row].BaseItem.quantity - voucher->InventoryItems[row].BaseItem.consumedQty <0){

                //                QMessageBox box; box.setText("Quantity cannot be reduced more than current consumed quantity" +
                //                                             QString::number(voucher->InventoryItems[row].BaseItem.consumedQty,'f',2)); box.exec();
                //                voucher->InventoryItems[row].BaseItem.quantity = voucher->InventoryItems[row].BaseItem.prevQty;
                //                return;
            }

        }
        else if (QString::compare(voucher->voucherType,
                                  GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder),
                                  Qt::CaseInsensitive)) {
            qDebug()<<"111111111111111 this is work order ";
            if(voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fifoList.size() == 0){
                ItemFifoDataModel *fifo = new ItemFifoDataModel();
                fifo->ItemID = voucher->InventoryItems[row].BaseItem.ItemID;
                fifo->VoucherID = voucher->voucherNumber;
                fifo->VoucherType = voucher->voucherType;
                fifo->VoucherPrefix = voucher->VoucherPrefix;
                fifo->VoucherDate = voucher->VoucherDate;
                fifo->ItemReqId = voucher->InventoryItems[row].BaseItem.ItemReqUuid /*+ QString::number(godownIndex)*/;
                fifo->drQty = voucher->InventoryItems[row].BaseItem.godownList[godownIndex].drQty;
                fifo->Price = voucher->InventoryItems[row].BaseItem.price;

                qDebug()<<"adding entry for price & qty"<<fifo->Price<<fifo->drQty;

                voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fifoList.append(fifo);
            }
            else{
                voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fifoList[0]->drQty =
                        voucher->InventoryItems[row].BaseItem.godownList[godownIndex].drQty;
            }
        }
        //added on 11/30 - to be removed
        //        else if(voucher->status == QuotationStatus::Started){

        //            qDebug()<<"stock in";

        //            ItemFifoDataModel *fifo = new ItemFifoDataModel();
        //            fifo->ItemID = voucher->InventoryItems[row].BaseItem.ItemID;
        //            fifo->VoucherID = voucher->voucherNumber;
        //            fifo->VoucherType = voucher->voucherType;
        //            fifo->VoucherPrefix = voucher->VoucherPrefix;
        //            fifo->VoucherDate = voucher->VoucherDate;
        //            fifo->ItemReqId = voucher->InventoryItems[row].BaseItem.ItemReqUuid;
        //            fifo->drQty = voucher->InventoryItems[row].BaseItem.drQty - voucher->InventoryItems[row].BaseItem.prevQty;
        //            fifo->Price = voucher->InventoryItems[row].BaseItem.price;

        //            voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fifoList.append(fifo);
        //            qDebug()<<godownIndex<<voucher->InventoryItems[row].BaseItem.godownList[godownIndex].fifoList.size();
        //        }
        else{
            // do nothing
            qDebug()<<"diff";
            qDebug()<<QString::compare(voucher->voucherType,
                                       GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder),
                                       Qt::CaseInsensitive);
        }

    }
}

void FifoCalculator::setPriceInFifo(GeneralVoucherDataObject *voucher, int row)
{
    for(int i=0;i<voucher->InventoryItems[row].BaseItem.godownList.size();i++){
        for(int j=0;j<voucher->InventoryItems[row].BaseItem.godownList[i].fifoList.size();j++){
            voucher->InventoryItems[row].BaseItem.godownList[i].fifoList[j]->Price = voucher->InventoryItems[row].BaseItem.price;
        }
    }
}

bool FifoCalculator::compareFifo(ItemFifoDataModel *fifo1, ItemFifoDataModel *fifo2)
{
    return (fifo1->VoucherID == fifo2->VoucherID &&
            fifo1->VoucherType == fifo2->VoucherType &&
            fifo1->VoucherPrefix == fifo2->VoucherPrefix &&
            fifo1->ItemReqId == fifo2->ItemReqId );
}
