#include "gmitemwiseordermodel.h"
#include <QFont>
#include <QIcon>
GMItemwiseOrderModel::GMItemwiseOrderModel(GeneralVoucherDataObject *voucher,
                                           PricelistDatabaseHelper *phelp,
                                           QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;

    QStringList lis;
    lis<<"#"<<"Item"<<"Purch Date"<<"Ledger"<<"UOM"<<"Total Qty"<<"Container"<<"Qty"<<"WtDis"<<"Rate"<<"Amount";
    this->colCount = lis.length()+1;
    TransactionCalculator::VoucherCalculateSales(voucher);
    this->phelper = phelp;
    headerStrings = lis;
}

QVariant GMItemwiseOrderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(section == colCount-1) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));
    }
    else if (orientation == Qt::Horizontal && section < colCount-1 ) {
        return headerStrings[section];
    }
    return QVariant();
}


int GMItemwiseOrderModel::rowCount(const QModelIndex &parent) const
{
    return voucher->InventoryItems.size()+1;


}

int GMItemwiseOrderModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant GMItemwiseOrderModel::data(const QModelIndex &index, int role) const
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

    if(index.row() < voucher->InventoryItems.size() && role == Qt::BackgroundRole ){
        if(voucher->InventoryItems[index.row()].BaseItem.itemVoucherStatus == QuotationStatus::SalesInvoiceRaised){
            return QVariant(QColor(Qt::red));
        }
    }

    if(role == Qt::UserRole){
        return voucher->InventoryItems[index.row()].BaseItem.ItemID;
    }

    if(role == Qt::UserRole+1){
        return  voucher->InventoryItems[index.row()].BaseItem.uomObject._id;
    }

    if(role == Qt::UserRole+5){
        //        qDebug()<<"From MOdel Bom Size : "<<voucher->InventoryItems[index.row()].BaseItem.bomList.length();
        if(voucher->InventoryItems[index.row()].BaseItem.bomList.length()>0){
            return QVariant(voucher->InventoryItems[index.row()].BaseItem.bomList[0].PurchaseItem);
        }
        else{
            return "";
        }
    }


    if (index.column() >4  && role == Qt::TextAlignmentRole) {
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

    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        if       (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if  (index.column() == 1)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ItemName;
        }
        else if  (index.column() == 2)
        {   if(role == Qt::DisplayRole)
                return voucher->InventoryItems[index.row()].BaseItem.manufactureDate.toString("dd-MM-yyyy");
            else
                return voucher->InventoryItems[index.row()].BaseItem.manufactureDate;
        }
        else if  (index.column() == 3)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ItemAlias;
        }
        else if (index.column() == 4)
        {
            return voucher->InventoryItems[index.row()].BaseItem.brandName;
        }
        else if (index.column() == 5)
        {
            return voucher->InventoryItems[index.row()].BaseItem.quantityFull;
        }
        else if (index.column() == 6)
        {
            return voucher->InventoryItems[index.row()].BaseItem.requestQty;
        }

        else if (index.column() == 7)
        {
            return voucher->InventoryItems[index.row()].BaseItem.quantity;
        }
        else if (index.column() == 8)
        {
            return voucher->InventoryItems[index.row()].BaseItem.discQuantity;
        }

        else if (index.column() == 9)
        {
            return QString::number(
                        voucher->InventoryItems[index.row()].BaseItem.price,
                    'f',2);
        }
        else if (index.column() == 10)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.subTotal,'f',2);
        }
        else if (index.column() == 11)
        {
            return "";
        }

    }

    return QVariant();
}

bool GMItemwiseOrderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    bool newLineAddedFlag = false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        if(row == voucher->InventoryItems.size() && index.column() == 1 ){
            CompoundItemDataObject item;
            item.BaseItem = value.value<inventoryItemDataModel>();
            item.BaseItem.Dimension = voucher->voucherNumber;
            item.BaseItem.length = row + 1;
            item.BaseItem.itemVoucherStatus = QuotationStatus::SalesOrderCreated;
            item.BaseItem.manufactureDate = voucher->VoucherDate;
            item.BaseItem.ProjectID = item.BaseItem.uomObject._id;
            item.BaseItem.brandName = item.BaseItem.uomObject.UomSymbol;
            item.BaseItem.defaultSalesLedgerID = "";
            //            qDebug()<<"New Item Found "<< item.BaseItem.ItemName;
            beginInsertRows(QModelIndex(),voucher->InventoryItems.size(),voucher->InventoryItems.size());
            voucher->InventoryItems.append(item);
            //            qDebug()<<"UOM IS : "<<item.BaseItem.uomObject.UomSymbol;
            newLineAddedFlag = true;
            endInsertRows();
        }
        else if (row< voucher->InventoryItems.size() && row >=0 && voucher->InventoryItems[row].BaseItem.itemVoucherStatus ==  QuotationStatus::SalesOrderCreated){
            //Replace

            if(index.column()==1){
                CompoundItemDataObject item;
                item.BaseItem = value.value<inventoryItemDataModel>();
                item.BaseItem.PriceLevel = voucher->priceListId;
                item.BaseItem.ItemAlias = voucher->InventoryItems[row].BaseItem.ItemAlias;
                item.BaseItem.defaultSalesLedgerID=voucher->InventoryItems[row].BaseItem.defaultSalesLedgerID;
                item.BaseItem.manufactureDate = voucher->VoucherDate;
                item.BaseItem.fromGodownID = voucher->fromGodownID;
                item.BaseItem.ProjectID = item.BaseItem.uomObject._id;
                item.BaseItem.brandName = item.BaseItem.uomObject.UomSymbol;
                item.BaseItem.manufactureDate = voucher->InventoryItems[row].BaseItem.manufactureDate;
                item.BaseItem.itemVoucherStatus = QuotationStatus::SalesOrderCreated;
                item.BaseItem.length = voucher->InventoryItems[row].BaseItem.length;
                item.BaseItem.Dimension = voucher->InventoryItems[row].BaseItem.Dimension;

                voucher->InventoryItems[row] =  item;
                float price =
                        phelper->getPriceByItemIDAndUOM(item.BaseItem.ItemID,
                                                        voucher->priceListId,
                                                        item.BaseItem.uomObject._id
                                                        );
                if(price != -1){
                    voucher->InventoryItems[row].BaseItem.price = price;
                    voucher->InventoryItems[row].BaseItem.uomObject.convRate = 1;
                }
                else {
                    voucher->InventoryItems[row].BaseItem.price = voucher->InventoryItems[row].BaseItem.price_1;
                }
            }
            if(index.column()==2){
                voucher->InventoryItems[row].BaseItem.manufactureDate = value.toDate();
            }

            if (index.column() == 3){
                LedgerMasterDataModel ledger = value.value<LedgerMasterDataModel>();
                qDebug()<<"Ledger Set as "<<ledger.LedgerName;
                voucher->InventoryItems[row].BaseItem.ItemAlias = ledger.LedgerName;
                voucher->InventoryItems[row].BaseItem.defaultSalesLedgerID = ledger.LedgerID;
            }
            else if (index.column() == 4)
            {
                UomDataModel uomObject = value.value<UomDataModel>();
                voucher->InventoryItems[row].BaseItem.ProjectID = uomObject._id;
                voucher->InventoryItems[row].BaseItem.brandName = uomObject.UomSymbol;

            }
            else if (index.column() == 5){
                qDebug()<<"Full qty :"<< value.toFloat();
                voucher->InventoryItems[row].BaseItem.quantityFull = value.toFloat();

                voucher->InventoryItems[row].BaseItem.quantity = voucher->InventoryItems[row].BaseItem.quantityFull -
                        voucher->InventoryItems[row].BaseItem.discQuantity;
            }

            else if (index.column() == 6){
                voucher->InventoryItems[row].BaseItem.requestQty = value.toFloat();
            }
            else if (index.column() == 8){
                voucher->InventoryItems[row].BaseItem.discQuantity = value.toFloat();
                voucher->InventoryItems[row].BaseItem.quantity = voucher->InventoryItems[row].BaseItem.quantityFull -
                        voucher->InventoryItems[row].BaseItem.discQuantity;
            }
            else if (index.column() == 9)
            {
                voucher->InventoryItems[row].BaseItem.price = value.toFloat();
            }
            else if(index.column() == 11){
                int rowNumber = value.value<int>();
            }
            else
                return false;
        }
        else{
            qDebug()<<"cant edit cos Status : "<<voucher->InventoryItems[row].BaseItem.itemVoucherStatus;
        }
        TransactionCalculator::VoucherCalculateSales(voucher);
        emit dataChanged(index, index, {role});
        emit updateValues();

        if(newLineAddedFlag)
            emit newLineAdded(index.row());
        return true;
    }

    return false;
}

Qt::ItemFlags GMItemwiseOrderModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;
    if(index.column()==0 || index.column()==11)
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
}

bool GMItemwiseOrderModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //    qDebug()<<"insert called";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMItemwiseOrderModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + 0);
    voucher->InventoryItems.removeAt(row);
    endRemoveRows();
}

void GMItemwiseOrderModel::setHeaderStrings(const QStringList &value)
{

    headerStrings = value;
}

void GMItemwiseOrderModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
}

void GMItemwiseOrderModel::setCurrentItem(inventoryItemDataModel value)
{
    CurrentItem = value;
}


QModelIndex GMItemwiseOrderModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->InventoryItems.size() && column < colCount){
        if((row == voucher->InventoryItems.size() && (column == 0 || column ==1) )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMItemwiseOrderModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

