#include "gminventoryitemmodel.h"
#include <QFont>
#include <QIcon>
#include "datamodels/transaction/gmvouchertypes.h"
#include "finance/transactions/calculator/fifocalculator.h"
#include  "datamodels/projects/Quotation/quotationstatus.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include "counter/MainScreen/itemdetail.h"
//#include "inventory/transactions/batchentryeditor.h"

GMInventoryItemModel::GMInventoryItemModel(GeneralVoucherDataObject *voucher,

                                           int transactionType,
                                           QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;

    this->transactionType = transactionType;

    pricehelper = new PricelistDatabaseHelper();
    shelper = new SalesInventoryItemDatabaseHelper();

    fifoCalc = new FifoCalculator();

    useSectionFlag = ConfigurationSettingsDatabaseHelper::getValue(useDefaultItemSection,false).toBool();

    allowItemMultipleEntryFlag = ConfigurationSettingsDatabaseHelper::getValue(allowItemMultipleEntry,true).toBool();

    headerStrings<<"#"<<"Particulars"<<"Item Code"<<"UOM"<<"Ordered \n Qty"<<"Quantity"<<"  Price"<<"Nett Price"<<"   MRP   "<<"Disc %"<<"Disc. Amt"<<"  Tax  "<<"Tax Amt"<<"Amount";
    this->colCount = headerStrings.size()+2;

    switch (transactionType) {
    case 1:{
        TransactionCalculator::VoucherCalculateSales(voucher);
        break;
    }
    case 2:{
        TransactionCalculator::VoucherCalculatePurchase(voucher);
        break;
    }
    case 3:{
        qDebug()<<Q_FUNC_INFO;
        TransactionCalculator::VoucherCalculateSalesReturn(voucher);
        break;
    }
    case 4:{
        TransactionCalculator::VoucherCalculatePurchaseReturn(voucher);
        break;
    }
    }
}

void GMInventoryItemModel::setAllowNegativeStock(bool newAllowNegativeStock)
{
    allowNegativeStock = newAllowNegativeStock;
}

QVariant GMInventoryItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (role != Qt::DisplayRole && section < colCount-2)
        return QVariant();


    if(section == colCount-2) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));

    }
    if(section == colCount-1) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QtMaterialTheme::icon("navigation", "more_vert"));

    }

    else if (orientation == Qt::Horizontal && section <colCount-2 ) {
        return headerStrings[section];
    }
    return QVariant();
}


int GMInventoryItemModel::rowCount(const QModelIndex &parent) const
{
    return voucher->InventoryItems.size()+1;
}

int GMInventoryItemModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant GMInventoryItemModel::data(const QModelIndex &index, int role) const
{


    if (!checkIndex(index))
        return QVariant();


    if (index.row() > voucher->InventoryItems.size() || index.row() < 0)
        return QVariant();

    if(index.row() == voucher->InventoryItems.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        if(index.column()==0){
            return  index.row()+1;
        }
        else if(index.column()==1){
            return "";
        }
        return "";
    }
    if (index.column() >2  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignRight|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::FontRole && index.column() == 1 &&
            index.row() == voucher->InventoryItems.size()) { // First column items are bold.
        QFont font;
        font.setItalic(true);
        return font;
    }
    if(role == Qt::UserRole){
        return voucher->InventoryItems[index.row()].BaseItem.ItemID;
    }

    if(role == Qt::UserRole+1){
        //        return QVariant::fromValue(voucher->InventoryItems[index.row()].BaseItem.uomObject);
        return  voucher->InventoryItems[index.row()].BaseItem.uomObject._id;
    }

    if(role == Qt::UserRole+2){

        //return ledger ID
        return voucher->ledgerObject.LedgerID;
    }

    if(role == Qt::UserRole+4){

        //return ledger ID
        return voucher->ledgerObject.defaultPriceListID;
    }

    if(role == Qt::UserRole+5){

        //return ledger ID
        return voucher->InventoryItems[index.row()].BaseItem.itemVoucherStatus == QuotationStatus::SalesOrderCreated;
    }
    if(role == Qt::UserRole +10){
        return "del";
    }

    if(role == Qt::UserRole+9){
        return QVariant::fromValue(voucher->InventoryItems[index.row()].BaseItem);
    }

    if(role == Qt::BackgroundColorRole){
        if(voucher->InventoryItems[index.row()].BaseItem.itemVoucherStatus != QuotationStatus::SalesOrderCreated)
        {return QColor(255,0,0,80);}
        else{
            return QColor(Qt::transparent);
        }
    }



    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) //qDebug()<<"Inside if for col "<<index.column();
        int x =0;
        if (index.column() == x++)
        {
            return  index.row()+1;
        }

        else if (index.column() == x++)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ItemName;
        }
        else if (index.column() == x++)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ItemCode;
        }

        else if (index.column() == x++)
        {   if(voucher->InventoryItems[index.row()].BaseItem.uomObject._id!="0")
                return voucher->InventoryItems[index.row()].BaseItem.uomObject.UomSymbol;
            else
                return "N A";
        }
        else if (index.column() == x++)
        {
            return voucher->InventoryItems[index.row()].BaseItem.consumedQty;
        }
        else if (index.column() == x++)
        {
//            if(voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate == 0)
//                voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate = 1;

            return QString::number(voucher->InventoryItems[index.row()].BaseItem.quantity
                    / voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate,'f',
                    voucher->InventoryItems[index.row()].BaseItem.uomObject.decimalPoints);
        }
        else if (index.column() == x++)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.price
                    * voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate,'f',2);
        }
        else if (index.column() == x++)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.price
                    * (1 + voucher->InventoryItems[index.row()].BaseItem.taxRate/100)
                    * (1 - voucher->InventoryItems[index.row()].BaseItem.discountPercentage/100)
                    * voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate,'f',2);
        }
        else if (index.column() == x++)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.price_2
                    * voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate,'f',2);
        }
        else if (index.column() == x++)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.discountPercentage,'f',2);
        }

        else if (index.column() == x++)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.discountinAmount,'f',2);
        }

        else if (index.column() == x++)//9
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.taxRate,'f',2);
        }
        else if (index.column() == x++)//10
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.taxAmount,'f',2);
        }

        else if (index.column() == x++)//11
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.grandTotal , 'f',2);
            //            return QString::number(voucher->InventoryItems[index.row()].BaseItem.subTotal
            //                    + voucher->InventoryItems[index.row()].BaseItem.taxAmount
            //                    - voucher->InventoryItems[index.row()].BaseItem.discountinAmount,'f',2);
        }
        else if (index.column() == colCount-2)
        {
            return "";
        }

        else if (index.column() == colCount-1)
        {
            //qDebug()<<"more icon";
            return "";
        }
    }

    return QVariant();
}

bool GMInventoryItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //    //qDebug()<<"set data called";

    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        if(row == voucher->InventoryItems.size() && index.column() == 1){
            CompoundItemDataObject item;
            item.BaseItem = value.value<inventoryItemDataModel>();
            item.BaseItem.PriceLevel = voucher->priceListId;
            item.BaseItem.ItemReqUuid = DatabaseHelper::getUUID();
            for(int i=0;i<voucher->InventoryItems.length();i++){
                if(voucher->InventoryItems[i].BaseItem.ItemID == item.BaseItem.ItemID){
                    emit itemRepeated(i);
                    if(!allowItemMultipleEntryFlag && transactionType == 1)
                    return false;
                }
            }
            if(!useSectionFlag){
                item.BaseItem.fromGodownID = voucher->fromGodownID;
                item.BaseItem.toGodownID = voucher->toGodownID;
            }

            else {
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                switch (transactionType) {
                case 1: case 3: {
                    item.BaseItem.fromGodownID = item.BaseItem.section;
                    item.BaseItem.toGodownID = "OUT";
                    break;
                }
                case 2: case 4: {
                    item.BaseItem.toGodownID = item.BaseItem.section;
                    item.BaseItem.fromGodownID = "OUT";
                    break;
                }

                }
                qDebug()<<item.BaseItem.fromGodownID<<item.BaseItem.toGodownID<<item.BaseItem.ItemName
                       <<item.BaseItem.section;
            }


            GodownTransactionDataModel godownEntry;
            godownEntry.fromGodown = item.BaseItem.fromGodownID;
            godownEntry.toGodown = item.BaseItem.toGodownID;
            item.BaseItem.godownList.append(godownEntry);
            //            item.BaseItem.price = pricehelper->getPriceByItemIDAndUOM(item.BaseItem.ItemID,
            //            voucher->priceListId,
            //            item.BaseItem.uomObject._id
            //            );

            if(item.BaseItem.defaultUOMID == "0"){
                item.BaseItem.quantity = 1;
            }
            beginInsertRows(QModelIndex(),voucher->InventoryItems.size(),
                            voucher->InventoryItems.size());
            voucher->InventoryItems.append(item);
            switch (transactionType) {
            case 1:
            case 3:{
                //qDebug()<<"ITEM MODEL SETTING";
                float price =
                        pricehelper->getPriceByItemIDAndUOM(item.BaseItem.ItemID,
                                                            voucher->priceListId,
                                                            item.BaseItem.uomObject._id
                                                            );
                float discount = 0;
//                        pricehelper->getDiscountByItemIDAndUOM(item.BaseItem.ItemID,
//                                                            voucher->priceListId,
//                                                            item.BaseItem.uomObject._id
//                                                            );


                if(price != -1){
                    voucher->InventoryItems[row].BaseItem.price = price;
                    voucher->InventoryItems[row].BaseItem.uomObject.convRate = 1;
                    voucher->InventoryItems[row].BaseItem.discountPercentage = discount;
//                    voucher->InventoryItems[row].BaseItem.discountinAmount = price * discount/100;


                }
                else {
                    voucher->InventoryItems[row].BaseItem.price =
                            voucher->InventoryItems[row].BaseItem.price_1;
                }
                setItemRateforSales(row);

                qDebug()<<Q_FUNC_INFO<<__LINE__<<item.BaseItem.fromGodownID;

                voucher->InventoryItems[row].BaseItem.ClosingStock = itemHelper.getClsStkOfItemTillDate(item.BaseItem.ItemID, voucher->VoucherDate, item.BaseItem.fromGodownID);
                break;
            }
            case 2:
            case 4:{
                voucher->InventoryItems[row].BaseItem.ClosingStock = itemHelper.getClsStkOfItemTillDate(item.BaseItem.ItemID, voucher->VoucherDate, item.BaseItem.toGodownID);
                setItemRateforPurchase(row);
                break;
            }
            }
            newLineAddedFlag = true;
            endInsertRows();
            if(voucher->InventoryItems[row].BaseItem.isBatchProcessed){
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                emit qtyEdited(row);
            }
        }
        // edit any inserted row
        else if (row< voucher->InventoryItems.size() && row >=0 ){
            int x =1;
            if (index.column() == x++)
            {
                CompoundItemDataObject item;
                item.BaseItem = value.value<inventoryItemDataModel>();
                item.BaseItem.PriceLevel = voucher->priceListId;
                item.BaseItem.ItemReqUuid = DatabaseHelper::getUUID();

                if(!useSectionFlag){
                    item.BaseItem.fromGodownID = voucher->fromGodownID;
                    item.BaseItem.toGodownID = voucher->toGodownID;
                }
                else {
                    qDebug()<<Q_FUNC_INFO<<__LINE__;
                    switch (transactionType) {
                    case 1: case 3: {
                        item.BaseItem.fromGodownID = item.BaseItem.section;
                        item.BaseItem.toGodownID = "OUT";

                        break;

                    }
                    case 2: case 4: {
                        item.BaseItem.toGodownID = item.BaseItem.section;
                        item.BaseItem.fromGodownID = "OUT";

                        break;

                    }

                    }
                    //                item.BaseItem.toGodownID = ;
                }
                qDebug()<<item.BaseItem.fromGodownID<<item.BaseItem.toGodownID<<item.BaseItem.ItemName<<item.BaseItem.section;


                //qDebug()<<"ITEM MODEL SETTING ITEM TAX : "<<item.BaseItem.taxRate;

                GodownTransactionDataModel godownEntry;
                godownEntry.fromGodown = item.BaseItem.fromGodownID;
                godownEntry.toGodown = item.BaseItem.toGodownID;
                item.BaseItem.godownList.append(godownEntry);
                voucher->InventoryItems[row] =  item;
                switch (transactionType) {
                case 1:{


                    float price =
                            pricehelper->getPriceByItemIDAndUOM(item.BaseItem.ItemID,
                                                                voucher->priceListId,
                                                                item.BaseItem.uomObject._id
                                                                );
                    float discount = 0;
//                            pricehelper->getDiscountByItemIDAndUOM(item.BaseItem.ItemID,
//                                                                voucher->priceListId,
//                                                                item.BaseItem.uomObject._id
//                                                                );
                    if(price != -1){
                        voucher->InventoryItems[row].BaseItem.price = price;
                        voucher->InventoryItems[row].BaseItem.uomObject.convRate = 1;
                        voucher->InventoryItems[row].BaseItem.discountPercentage = discount;
                        voucher->InventoryItems[row].BaseItem.discountinAmount = price * discount/100;

                    }
                    else {
                        voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.price_1;
                    }
                    voucher->InventoryItems[row].BaseItem.ClosingStock =
                            itemHelper.getClsStkOfItemTillDate(item.BaseItem.ItemID, voucher->VoucherDate, item.BaseItem.fromGodownID);
                    setItemRateforSales(row);
                    break;
                }
                case 2:{
                    item.BaseItem.toGodownID = voucher->toGodownID;
                    voucher->InventoryItems[row].BaseItem.ClosingStock =
                            itemHelper.getClsStkOfItemTillDate(item.BaseItem.ItemID, voucher->VoucherDate, item.BaseItem.toGodownID);
                    setItemRateforPurchase(row);
                    break;
                }
                case 3:{
                    item.BaseItem.fromGodownID = voucher->fromGodownID;
                    voucher->InventoryItems[row].BaseItem.ClosingStock =
                            itemHelper.getClsStkOfItemTillDate(item.BaseItem.ItemID, voucher->VoucherDate, item.BaseItem.fromGodownID);
                    setItemRateforSales(row);
                    break;
                }
                case 4:{
                    voucher->InventoryItems[row].BaseItem.ClosingStock =
                            itemHelper.getClsStkOfItemTillDate(item.BaseItem.ItemID, voucher->VoucherDate, item.BaseItem.toGodownID);
                    item.BaseItem.toGodownID = voucher->toGodownID;
                    setItemRateforPurchase(row);
                    break;
                }
                }


                if(voucher->InventoryItems[row].BaseItem.isBatchProcessed){
                    qDebug()<<Q_FUNC_INFO<<__LINE__;
                    emit qtyEdited(row);
                }
            }
            else if (index.column() == x++)
            {
            }

            else if (index.column() == x++){ // x=3
                voucher->InventoryItems[row].BaseItem.uomObject = value.value<UomDataModel>();
                switch (transactionType) {
                case 1:{

                    float price = pricehelper->getPriceByItemIDAndUOM(voucher->InventoryItems[row].BaseItem.ItemID,
                                                                      voucher->priceListId,
                                                                      voucher->InventoryItems[row].BaseItem.uomObject._id);
                    float discount = 0;
//                            pricehelper->getDiscountByItemIDAndUOM(voucher->InventoryItems[row].BaseItem.ItemID,
//                                                                   voucher->priceListId,
//                                                                   voucher->InventoryItems[row].BaseItem.uomObject._id
//                                                                );
                    if(price != -1){
                        voucher->InventoryItems[row].BaseItem.price = price;
                        voucher->InventoryItems[row].BaseItem.uomObject.convRate = 1;
                        voucher->InventoryItems[row].BaseItem.discountPercentage = discount;
                        voucher->InventoryItems[row].BaseItem.discountinAmount = price * discount/100;
                    }
                    else {
                        voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.price_1;
                    }
                    break;
                }
                case 2:{
                    break;
                }
                }




            }
            else if (index.column() == x++)
            {
                // item code
            }
            else if (index.column() == x++){ //x=4  change quantity

                voucher->InventoryItems[row].BaseItem.quantity =
                        value.toFloat() * voucher->InventoryItems[row].BaseItem.uomObject.convRate;
                voucher->InventoryItems[row].BaseItem.discountinAmount =
                        voucher->InventoryItems[row].BaseItem.price *
                        voucher->InventoryItems[row].BaseItem.quantity *
                        voucher->InventoryItems[row].BaseItem.discountPercentage / 100;

                setItemQtyByFifo(row);
//                qDebug()<<Q_FUNC_INFO<<__LINE__;

            }
            else if (index.column() == x++){ //x=5 price
                voucher->InventoryItems[row].BaseItem.price =
                        value.toFloat() / voucher->InventoryItems[row].BaseItem.uomObject.convRate ;
                qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->InventoryItems[row].BaseItem.price << value.toFloat();
                voucher->InventoryItems[row].BaseItem.discountinAmount =
                        voucher->InventoryItems[row].BaseItem.price *
                        voucher->InventoryItems[row].BaseItem.quantity *
                        voucher->InventoryItems[row].BaseItem.discountPercentage /100;
//                if(transactionType == 2 &&
//                        voucher->InventoryItems[row].BaseItem.isBatchProcessed ){
//                    for(int k=0; k<voucher->InventoryItems[row].BaseItem.godownList.size(); k++){
//                        if(voucher->InventoryItems[row].BaseItem.godownList[k].batchList.size() >0)
//                                voucher->InventoryItems[row].BaseItem.godownList[k].batchList[0].purchaseCost = value.toFloat();
//                    }
//                }
                fifoCalc->setPriceInFifo(voucher, row);
                voucher->InventoryItems[row].BaseItem.discountinAmount =
                        voucher->InventoryItems[row].BaseItem.price *
                        voucher->InventoryItems[row].BaseItem.quantity *
                        voucher->InventoryItems[row].BaseItem.discountPercentage /100;
            }
            else if (index.column() == x++) { //x =6
                voucher->InventoryItems[row].BaseItem.price = ((10000*value.toFloat())/
                        ((100+voucher->InventoryItems[row].BaseItem.taxRate)*((100-voucher->InventoryItems[row].BaseItem.discountPercentage))))
                        / voucher->InventoryItems[row].BaseItem.uomObject.convRate ;
                voucher->InventoryItems[row].BaseItem.discountinAmount =
                        voucher->InventoryItems[row].BaseItem.price *
                        voucher->InventoryItems[row].BaseItem.quantity *
                        voucher->InventoryItems[row].BaseItem.discountPercentage /100;
            }
            else if( index.column() == x++){

            }
            else if (index.column() == x++) { //x=7

                voucher->InventoryItems[row].BaseItem.discountPercentage = value.toFloat();
                voucher->InventoryItems[row].BaseItem.discountinAmount =
                        voucher->InventoryItems[row].BaseItem.price *
                        voucher->InventoryItems[row].BaseItem.quantity *
                        voucher->InventoryItems[row].BaseItem.discountPercentage /100;
                qDebug()<<"disc amt"<<voucher->InventoryItems[row].BaseItem.discountinAmount;
                //                shelper->updateGST(voucher->InventoryItems[row].BaseItem.ItemID,voucher->InventoryItems[row].BaseItem.taxRate);

            }
            else if (index.column() == x++) { //x=8
                voucher->InventoryItems[row].BaseItem.discountinAmount = value.toFloat();
                if(value.toFloat() > 0){
                    voucher->InventoryItems[row].BaseItem.discountPercentage =
                            voucher->InventoryItems[row].BaseItem.discountinAmount * 100
                            / (voucher->InventoryItems[row].BaseItem.price *
                               voucher->InventoryItems[row].BaseItem.quantity);
                    qDebug()<<"disc %"<<voucher->InventoryItems[row].BaseItem.discountPercentage;
                }
                //                shelper->updateGST(voucher->InventoryItems[row].BaseItem.ItemID,voucher->InventoryItems[row].BaseItem.taxRate);

            }
            else if (index.column() == x++) {  //x=9
                voucher->InventoryItems[row].BaseItem.taxRate = value.toFloat();
                qDebug()<<"tax rate model"<<voucher->InventoryItems[row].BaseItem.taxRate<<voucher->InventoryItems[row].BaseItem.taxAmount;
                //                shelper->updateGST(voucher->InventoryItems[row].BaseItem.ItemID,voucher->InventoryItems[row].BaseItem.taxRate);

            }
            else if(index.column() ==x++){ //x=10

            }
            else if (index.column() == x++){
                voucher->InventoryItems[row].BaseItem.subTotal =
                        value.toFloat();
//                voucher->InventoryItems[row].BaseItem.price =
//                        value.toFloat() /
//                        voucher->InventoryItems[row].BaseItem.quantity * 100 ;
//                voucher->InventoryItems[row].BaseItem.price /=
//                        (voucher->InventoryItems[row].BaseItem.taxRate + 100 );
                voucher->InventoryItems[row].BaseItem.price = ((100*voucher->InventoryItems[row].BaseItem.subTotal)/
                        ((100+voucher->InventoryItems[row].BaseItem.taxRate)*(voucher->InventoryItems[row].BaseItem.quantity*(100-voucher->InventoryItems[row].BaseItem.discountPercentage))))*100;
                voucher->InventoryItems[row].BaseItem.discountinAmount =
                        voucher->InventoryItems[row].BaseItem.price *
                        voucher->InventoryItems[row].BaseItem.quantity *
                        voucher->InventoryItems[row].BaseItem.discountPercentage /100;

            }
            else if(index.column()==colCount-2){
                int rowNumber = value.value<int>();
            }

            else if(index.column()==colCount-1){
                int rowNumber = value.value<int>();
                //                beginRemoveRows(QModelIndex(), rowNumber, rowNumber);
                //                voucher->InventoryItems.removeAt(rowNumber);
                //                endRemoveRows();

                //                beginResetModel();
                //                voucher->InventoryItems.removeAt(rowNumber);
                //                endResetModel();
            }
            else
            {
                return false;
            }
        }
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<transactionType;
        switch (transactionType) {
        case 1:{
            TransactionCalculator::VoucherCalculateSales(voucher);
//            qDebug()<<"&&&&&&&&&&&&&"<<voucher->grandTotal;
            break;
        }
        case 2:{
            TransactionCalculator::VoucherCalculatePurchase(voucher);
            break;
        }
        case 3:{
            qDebug()<<Q_FUNC_INFO;
            TransactionCalculator::VoucherCalculateSalesReturn(voucher);
            break;
        }
        case 4:{
            TransactionCalculator::VoucherCalculatePurchaseReturn(voucher);
            break;
        }
        }
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->grandTotal;
        emit dataChanged(index, index, {role});
        emit updateValues();

        if(newLineAddedFlag)
            emit newLineAdded(index.row());
        return true;
    }

    return false;
}

Qt::ItemFlags GMInventoryItemModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

bool GMInventoryItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
}

bool GMInventoryItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + 0);
    voucher->InventoryItems.removeAt(row);
    endRemoveRows();

//    emit updateValues();

}

//void GMInventoryItemModel::setHeaderStrings(const QStringList &value)
//{
//    //    headerStrings = value;
//}

void GMInventoryItemModel::setVoucher(GeneralVoucherDataObject *value)
{
    //    //qDebug()<<"## setVoucher Price:"<<voucher->InventoryItems[0].BaseItem.price<<value->InventoryItems[0].BaseItem.price;
    beginResetModel();
    voucher = value;
    //    //qDebug()<<"%% setVoucher Price:"<<voucher->InventoryItems[0].BaseItem.price<<value->InventoryItems[0].BaseItem.price;
    endResetModel();
    //    //qDebug()<<"Model Reset";
}

//void GMInventoryItemModel::setItemBatchEntry(int row)
//{
//    BatchEntryEditor *batchEditor = new BatchEntryEditor();
//    connect(batchEditor, &BatchEntryEditor::batchEntered, this, [=](InventoryItemBatchModel *batch1){
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<batch1->batchName;
//        if(voucher->InventoryItems[row].BaseItem.batchList.size() == 0){
//            batch1->itemID = voucher->InventoryItems[row].BaseItem.ItemID;
//            batch1->VoucherNo = voucher->voucherNumber;
//            batch1->VoucherType = voucher->voucherType;
//            batch1->VoucherPrefix = voucher->VoucherPrefix;
//            batch1->RefNo = voucher->voucherNumber;
//            batch1->RefType = voucher->voucherType;
//            batch1->RefPrefix = voucher->VoucherPrefix;
//            batch1->voucherDate = voucher->VoucherDate;
////            batch1->uom =  voucher->InventoryItems[row].BaseItem.uomObject._id;
//            voucher->InventoryItems[row].BaseItem.batchList.append(batch1);
//        }
//        else
//            voucher->InventoryItems[row].BaseItem.batchList[0] = batch1;
//    });
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    batchEditor->show();
//    qDebug()<<Q_FUNC_INFO<<__LINE__;

//}

void GMInventoryItemModel::setItemQtyByFifo( int row)
{
    qDebug()<<"start of "<<Q_FUNC_INFO;
    if((!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher),
                          Qt::CaseInsensitive)
        || !QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote),
                                                                   Qt::CaseInsensitive))
            && !allowNegativeStock){
        qDebug()<<__LINE__<<Q_FUNC_INFO;
        voucher->InventoryItems[row].BaseItem.ClosingStock =
                itemHelper.getClsStkOfItemTillDate(voucher->InventoryItems[row].BaseItem.ItemID, voucher->VoucherDate,
                                                   voucher->InventoryItems[row].BaseItem.fromGodownID);
        qDebug()<<voucher->InventoryItems[row].BaseItem.ClosingStock << voucher->InventoryItems[row].BaseItem.quantity;
        if(voucher->InventoryItems[row].BaseItem.isStockItem &&
                voucher->InventoryItems[row].BaseItem.ClosingStock +voucher->InventoryItems[row].BaseItem.prevQty < voucher->InventoryItems[row].BaseItem.quantity ){
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<"EMIT NO STOCK";
            voucher->InventoryItems[row].BaseItem.quantity = 0;
            voucher->InventoryItems[row].BaseItem.discountinAmount = 0;
            emit noStock(row);
//            return;
        }
    }
        if(voucher->InventoryItems[row].BaseItem.isBatchProcessed){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->InventoryItems[row].BaseItem.quantity;
            emit qtyEdited(row);
        }

    //   return;
    //   qDebug()<<Q_FUNC_INFO<<row<<voucher->InventoryItems[row].BaseItem.godownList.size();
    if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder),
                         Qt::CaseInsensitive))
        return;

    else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder),
                              Qt::CaseInsensitive))
        return;

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(voucher->InventoryItems[row].BaseItem.godownList.size() > 0){
        for( int i=1; i<voucher->InventoryItems[row].BaseItem.godownList.size();i++ )
            voucher->InventoryItems[row].BaseItem.godownList.removeAt(i);
    }
    else{
        GodownTransactionDataModel godownEntry;
        voucher->InventoryItems[row].BaseItem.godownList.append(godownEntry);
    }
    switch(transactionType){
    case 1:{
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        voucher->InventoryItems[row].BaseItem.crQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].crQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].toGodown = "OUT";
        voucher->InventoryItems[row].BaseItem.toGodownID = "OUT";
        if(voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown == "OUT")
            voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown =
                    voucher->fromGodownID;
        break;
    }
    case 2:
    {
        voucher->InventoryItems[row].BaseItem.drQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].drQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown = "OUT";
        voucher->InventoryItems[row].BaseItem.fromGodownID = "OUT";
        if(voucher->InventoryItems[row].BaseItem.godownList[0].toGodown == "OUT")
            voucher->InventoryItems[row].BaseItem.godownList[0].toGodown =
                    voucher->toGodownID;
        break;
    }
    case 3:
    {
        voucher->InventoryItems[row].BaseItem.drQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].drQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown = "OUT";
        voucher->InventoryItems[row].BaseItem.fromGodownID = "OUT";
        if(voucher->InventoryItems[row].BaseItem.godownList[0].toGodown == "OUT")
            voucher->InventoryItems[row].BaseItem.godownList[0].toGodown =
                    voucher->toGodownID;
        break;
    }
    case 4:
    {
        voucher->InventoryItems[row].BaseItem.crQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].crQty = voucher->InventoryItems[row].BaseItem.quantity;
        voucher->InventoryItems[row].BaseItem.godownList[0].toGodown = "OUT";
        voucher->InventoryItems[row].BaseItem.toGodownID = "OUT";
        if(voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown == "OUT")
            voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown =
                    voucher->fromGodownID;
        break;
    }
    }

    qDebug()<<"call fifo: "<<voucher->voucherType<<voucher->InventoryItems[row].BaseItem.godownList[0].toGodown<<voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown;
    fifoCalc->setItemQtyByFifo(voucher, row, 0);
    qDebug()<<"end of "<<Q_FUNC_INFO;

}

void GMInventoryItemModel::setItemRateforPurchase(int row)
{
    qDebug()<<Q_FUNC_INFO;
    // get last purchase price;
    //    voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.priceLastPurchase;
    voucher->InventoryItems[row].BaseItem.price =
            voucher->InventoryItems[row].BaseItem.priceLastPurchase > 0 ?
                voucher->InventoryItems[row].BaseItem.priceLastPurchase :
                voucher->InventoryItems[row].BaseItem.stdCost;
    fifoCalc->setPriceInFifo(voucher, row);
    return;
    if(voucher->InventoryItems[row].BaseItem.section.contains("Godown",Qt::CaseInsensitive))
    {
        //        voucher->InventoryItems[row].BaseItem.price = last purchase Price;
        voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.stdCost +
                voucher->InventoryItems[row].BaseItem.stdCost * 0.1;
    }
    else {
        voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.price * 0.7;
    }

    //    //qDebug()<<"Price set : "<< voucher->InventoryItems[row].BaseItem.price;
}

void GMInventoryItemModel::setItemRateforSales(int row)
{
    //  Not doing anything for now
    return;
    if(voucher->InventoryItems[row].BaseItem.section.contains("Godown",Qt::CaseInsensitive))
    {
        voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.stdCost +
                voucher->InventoryItems[row].BaseItem.stdCost * 0.1;
    }
    else {
        voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.price * 0.7;
    }
    //    //qDebug()<<"Price set : "<< voucher->InventoryItems[row].BaseItem.price;


    // get price from pricelist
    if(voucher->ledgerObject.defaultPriceListID.length()>0){

    }
    else {

    }
}

QModelIndex GMInventoryItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->InventoryItems.size() && column< colCount){
        if((row == voucher->InventoryItems.size() && column <= 1 )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();

}

QModelIndex GMInventoryItemModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

//bool GMInventoryItemModel::insertColumns(int column, int count, const QModelIndex &parent)
//{
//    beginInsertColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endInsertColumns();
//}

//bool GMInventoryItemModel::removeColumns(int column, int count, const QModelIndex &parent)
//{
//    beginRemoveColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endRemoveColumns();
//}
