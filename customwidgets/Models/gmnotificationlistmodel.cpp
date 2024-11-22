#include "GMNotificationListModel.h"
#include <QFont>
#include <QIcon>
#include "datamodels/transaction/gmvouchertypes.h"
#include "datamodels/projects/Quotation/quotationstatus.h"

GMNotificationListModel::GMNotificationListModel(GeneralVoucherDataObject *voucher, int col, bool isData, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->isData = isData;

    if(isData)
        this->colCount = col+1;
    else
        this->colCount = col;

}

QVariant GMNotificationListModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    // FIXME: Implement me!
    if (role != Qt::DisplayRole && section !=8)
        return QVariant();


    if(section == 5) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));

    }


    else if (orientation == Qt::Horizontal && section !=5 ) {
        return headerStrings[section];
    }
    return QVariant();
}

//bool GMNotificationListModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
//{
//    if (value != headerData(section, orientation, role)) {
//        // FIXME: Implement me!
//        emit headerDataChanged(orientation, section, section);
//        return true;
//    }
//    return false;
//}


int GMNotificationListModel::rowCount(const QModelIndex &parent) const
{
    if(isData)
    return voucher->InventoryItems.size()+1;
    else
        return voucher->InventoryItems.size();
}

int GMNotificationListModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant GMNotificationListModel::data(const QModelIndex &index, int role) const
{

    //    if(index.column() == 1) qDebug()<<"At request data";

    if (!checkIndex(index))
        return QVariant();
    //    if(index.column() == 1) qDebug()<<"failed at check index";


    if (index.row() > voucher->InventoryItems.size() || index.row() < 0)
        return QVariant();

    if(index.row() == voucher->InventoryItems.size() &&
            ( role == Qt::DisplayRole || role == Qt::EditRole ))
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

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() == 1)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ItemName;
        }
        else if (index.column() == 2)
        {
            return voucher->InventoryItems[index.row()].BaseItem.uomObject.UomSymbol;
        }
        else if (index.column() == 3)
        {
            return voucher->InventoryItems[index.row()].BaseItem.quantity ;
        }
        else if (index.column() == 4)
        {
//            qDebug()<<"Status"<<voucher->InventoryItems[index.row()].BaseItem.itemProductionStatus<<voucher->InventoryItems[index.row()].BaseItem.ItemName;
            return  QuotationStatus::getStatusString(voucher->InventoryItems[index.row()].BaseItem.itemProductionStatus);
        }
        else if (index.column() == 5)
        {
            return "";
        }

    }

    return QVariant();
}

bool GMNotificationListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    bool newLineAddedFlag =false;


    if (index.isValid() && role == Qt::EditRole && isData) {
        int row = index.row();



        if(row == voucher->InventoryItems.size() && index.column() == 1){
            //Create New
            qDebug()<<"Append";
            CompoundItemDataObject item;
            //            QObject *object = qvariant_cast <QObject*>(value);
            //            item.BaseItem = qobject_cast<inventoryItemDataModel> (object);


            item.BaseItem = value.value<inventoryItemDataModel>();
            item.BaseItem.quantity = 1;
            item.BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;
            beginInsertRows(QModelIndex(),voucher->InventoryItems.size(),voucher->InventoryItems.size());
            voucher->InventoryItems.append(item);


            //            this->insertRow(voucher->InventoryItems.size(),QModelIndex());
            newLineAddedFlag = true;

            endInsertRows();

        }
        else if (row< voucher->InventoryItems.size() && row >=0 ){
            //Replace
            //            qDebug()<<"Replace ";
            if (index.column() == 1){
                CompoundItemDataObject item;
                item.BaseItem = value.value<inventoryItemDataModel>();
                item.BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;

                //                qDebug()<<"======================Set Item name "<<item.BaseItem.ItemName;
                voucher->InventoryItems[row] =  item;
            }

            else if (index.column() == 3)
            {
                voucher->InventoryItems[row].BaseItem.quantity =
                        value.toFloat();

                voucher->InventoryItems[row].BaseItem.crQty = value.toFloat();

            }
            else if(index.column() == 5){
                int rowNumber = value.value<int>();
                beginRemoveRows(QModelIndex(), row, row);
                voucher->InventoryItems.removeAt(row);
                endRemoveRows();
            }
            else
                return false;
        }


        emit dataChanged(index, index, {role});
        emit updateValues();

        if(newLineAddedFlag)
            emit newLineAdded(index.row());
        return true;
    }

    return false;
}

Qt::ItemFlags GMNotificationListModel::flags(const QModelIndex &index) const
{

    if (!index.isValid() && isData)
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

bool GMNotificationListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
}

bool GMNotificationListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug()<<"Begin remove Rows";
    beginRemoveRows(parent, row, row + count);
    voucher->InventoryItems.removeAt(row);
    endRemoveRows();
}

void GMNotificationListModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
}

void GMNotificationListModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
}


QModelIndex GMNotificationListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->InventoryItems.size() && column< colCount){
        if((row == voucher->InventoryItems.size() && column <= 1 )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();

}

QModelIndex GMNotificationListModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}


