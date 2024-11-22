#include "pricelistmodel.h"

PriceListModel::PriceListModel(PriceListDataObject *obj, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->priceList = obj;
    this->colCount = col;
}

QVariant PriceListModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    // FIXME: Implement me!
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return headerStrings[section];
    }
    return QVariant();
}

//bool PriceListModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
//{
//    if (value != headerData(section, orientation, role)) {
//        // FIXME: Implement me!
//        emit headerDataChanged(orientation, section, section);
//        return true;
//    }
//    return false;
//}


int PriceListModel::rowCount(const QModelIndex &parent) const
{
    return priceList->items.size()+1;
}

int PriceListModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant PriceListModel::data(const QModelIndex &index, int role) const
{

    //    if(index.column() == 1) qDebug()<<"At request data";

    if (!checkIndex(index))
        return QVariant();
    //    if(index.column() == 1) qDebug()<<"failed at check index";


    if (index.row() > priceList->items.size() || index.row() < 0)
        return QVariant();

    if(index.row() == priceList->items.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
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
            index.row() == priceList->items.size()) { // First column items are bold.
            QFont font;
            font.setItalic(true);
            return font;
    }

    if(role == Qt::UserRole){
        return priceList->items[index.row()].ItemID;
    }

    if(role == Qt::UserRole+1){
//        return QVariant::fromValue(voucher->InventoryItems[index.row()].BaseItem.uomObject);
        return  priceList->items[index.row()].uomObject._id;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() == 1)
        {
            if(priceList->items[index.row()].ItemName.length()>0)
            return priceList->items[index.row()].ItemName;
            else
                return "Item Name not found for : "+priceList->items[index.row()].ItemID;
        }
        else if (index.column() == 2)
        {
            return priceList->items[index.row()].uomObject.UomSymbol;
        }
        else if (index.column() == 3)
        {
            return priceList->items[index.row()].price;
        }
    }

    return QVariant();
}

bool PriceListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool newLineAddedFlag =false;

    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        if(row == priceList->items.size() && index.column() == 1){
            CompoundItemDataObject item;
            item.BaseItem = value.value<inventoryItemDataModel>();
            beginInsertRows(QModelIndex(),priceList->items.size(),priceList->items.size());
            priceList->items.append(item.BaseItem);
            newLineAddedFlag = true;
            endInsertRows();

        }
        else if (row< priceList->items.size() && row >=0 ){

            if (index.column() == 1){
                CompoundItemDataObject item;
                item.BaseItem = value.value<inventoryItemDataModel>();
                priceList->items[row] =  item.BaseItem;
            }
            else if (index.column() == 2){
                priceList->items[row].uomObject = value.value<UomDataModel>();
           }

            else if (index.column() == 3)
                priceList->items[row].price = value.toFloat();
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

Qt::ItemFlags PriceListModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

bool PriceListModel::insertRows(int row, int count, const QModelIndex &parent)
{
//    qDebug()<<"insert called";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool PriceListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return QAbstractItemModel::removeRows(row,count,parent);
}

void PriceListModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
}

void PriceListModel::setPriceList(PriceListDataObject *value)
{
    beginResetModel();
    priceList = value;
    endResetModel();
}


QModelIndex PriceListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=priceList->items.size() && column< colCount){
        if((row == priceList->items.size() && column <= 1 )|| row < priceList->items.size())
            return this->createIndex(row,column);
    }
    return QModelIndex();
}

QModelIndex PriceListModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

