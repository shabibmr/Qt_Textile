#include "gmgodowntransfermodel.h"


#include <QFont>
#include <QIcon>
#include "datamodels/transaction/gmvouchertypes.h"

GMGodownTransferModel::GMGodownTransferModel(GeneralVoucherDataObject *voucher, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->colCount = col+1;

    fifoCalc = new FifoCalculator();
}

GMGodownTransferModel::~GMGodownTransferModel()
{

}

QVariant GMGodownTransferModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    // FIXME: Implement me!
    if (role != Qt::DisplayRole && section != 5)
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

int GMGodownTransferModel::rowCount(const QModelIndex &parent) const
{
    return voucher->InventoryItems.size()+1;


}

int GMGodownTransferModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant GMGodownTransferModel::data(const QModelIndex &index, int role) const
{

    //    if(index.column() == 1) qDebug()<<"At request data";

    if (!checkIndex(index))
        return QVariant();
    //    if(index.column() == 1) qDebug()<<"failed at check index";


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
            return voucher->InventoryItems[index.row()].BaseItem.price;
        }
        else if (index.column() == 5)
        {
            return "";
        }

    }

    return QVariant();
}

bool GMGodownTransferModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    bool newLineAddedFlag =false;


    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();



        if(row == voucher->InventoryItems.size() && index.column() == 1){
            //Create New
            qDebug()<<"Append";
            CompoundItemDataObject item;
            //            QObject *object = qvariant_cast <QObject*>(value);
            //            item.BaseItem = qobject_cast<inventoryItemDataModel> (object);


            item.BaseItem = value.value<inventoryItemDataModel>();
            beginInsertRows(QModelIndex(),voucher->InventoryItems.size(),voucher->InventoryItems.size());
            voucher->InventoryItems.append(item);
            qDebug()<<"New Item at "<<voucher->InventoryItems.size()<< " is "<<
                      voucher->InventoryItems[voucher->InventoryItems.size()-1].BaseItem.ItemName;

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
                //                qDebug()<<"======================Set Item name "<<item.BaseItem.ItemName;
                voucher->InventoryItems[row] =  item;
            }

            else if (index.column() == 3)
            {
//                voucher->InventoryItems[row].BaseItem.quantity = value.toFloat();


                voucher->InventoryItems[row].BaseItem.quantity =
                        value.toFloat();
                voucher->InventoryItems[row].BaseItem.crQty =  value.toFloat();
                voucher->InventoryItems[row].BaseItem.drQty =  value.toFloat();



            }
            else if(index.column()==5){
                int rowNumber = value.value<int>();
                //                qDebug()<<"At model "<<rowNumber;
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

Qt::ItemFlags GMGodownTransferModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

bool GMGodownTransferModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //    qDebug()<<"insert called";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMGodownTransferModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + 0);
    voucher->InventoryItems.removeAt(row);
    endRemoveRows();
}

void GMGodownTransferModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
    qDebug()<<"Header Set";
}

void GMGodownTransferModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
    //    qDebug()<<"Model Reset";
}


QModelIndex GMGodownTransferModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->InventoryItems.size() && column< colCount){
        if((row == voucher->InventoryItems.size() && column <= 1 )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();


}

QModelIndex GMGodownTransferModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}


//bool GMGodownTransferModel::insertColumns(int column, int count, const QModelIndex &parent)
//{
//    beginInsertColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endInsertColumns();
//}

//bool GMGodownTransferModel::removeColumns(int column, int count, const QModelIndex &parent)
//{
//    beginRemoveColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endRemoveColumns();
//}
