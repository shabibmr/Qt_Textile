#include "gmwastageentrymodel.h"
#include <QFont>
#include <QIcon>
#include "datamodels/transaction/gmvouchertypes.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"

GMWastageEntryModel::GMWastageEntryModel(GeneralVoucherDataObject *voucher, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->colCount = col+2;

    fifoCalc = new FifoCalculator();
}



QVariant GMWastageEntryModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    // FIXME: Implement me!
    if (role != Qt::DisplayRole && section < this->colCount-2)
        return QVariant();

    if(section == 5) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));
    }

    if(section == 6) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QtMaterialTheme::icon("navigation", "more_vert"));

    }

    else if (orientation == Qt::Horizontal && section < 5 ) {
        return headerStrings[section];
    }
    return QVariant();
}

int GMWastageEntryModel::rowCount(const QModelIndex &parent) const
{
    return voucher->InventoryItems.size()+1;


}

int GMWastageEntryModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant GMWastageEntryModel::data(const QModelIndex &index, int role) const
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
            return voucher->InventoryItems[index.row()].BaseItem.price *
                    voucher->InventoryItems[index.row()].BaseItem.quantity;
        }
        else if (index.column() == 5)
        {
            return "";
        }
        else if (index.column() == 6)
        {
            return "";
        }

    }

    return QVariant();
}

bool GMWastageEntryModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
            item.BaseItem.fromGodownID = voucher->fromGodownID;
            item.BaseItem.toGodownID = "OUT";

            beginInsertRows(QModelIndex(),voucher->InventoryItems.size(),voucher->InventoryItems.size());
            voucher->InventoryItems.append(item);
            GodownTransactionDataModel godownEntry;
            godownEntry.fromGodown = voucher->InventoryItems[row].BaseItem.fromGodownID;
            godownEntry.toGodown = "OUT";
            voucher->InventoryItems[row].BaseItem.godownList.append(godownEntry);
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
                GodownTransactionDataModel godownEntry;
                godownEntry.fromGodown = voucher->InventoryItems[row].BaseItem.fromGodownID;
                godownEntry.toGodown = "OUT";
                voucher->InventoryItems[row].BaseItem.godownList.append(godownEntry);
            }

            else if (index.column() == 3)
            {
                voucher->InventoryItems[row].BaseItem.quantity =
                        value.toFloat();
                float diff = voucher->InventoryItems[row].BaseItem.quantity;
                if(diff >0){
                    voucher->InventoryItems[row].BaseItem.crQty = diff;
                    voucher->InventoryItems[row].BaseItem.drQty =0;
                }
                else{
                    voucher->InventoryItems[row].BaseItem.drQty = -diff;
                    voucher->InventoryItems[row].BaseItem.crQty = 0;
                }

                setItemQtyByFifo(row);

            }
            else if(index.column()==5){
                int rowNumber = value.value<int>();
                //                qDebug()<<"At model "<<rowNumber;
            }
            else if(index.column()==6){
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

Qt::ItemFlags GMWastageEntryModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

bool GMWastageEntryModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //    qDebug()<<"insert called";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMWastageEntryModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + 0);
    for(int i =0;i<count;i++)
        voucher->InventoryItems.removeAt(i);
    endRemoveRows();
}

void GMWastageEntryModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
    qDebug()<<"Header Set";
}

void GMWastageEntryModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
    //    qDebug()<<"Model Reset";
}


QModelIndex GMWastageEntryModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->InventoryItems.size() && column< colCount){
        if((row == voucher->InventoryItems.size() && column <= 1 )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();


}

QModelIndex GMWastageEntryModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}


//bool GMWastageEntryModel::insertColumns(int column, int count, const QModelIndex &parent)
//{
//    beginInsertColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endInsertColumns();
//}

//bool GMWastageEntryModel::removeColumns(int column, int count, const QModelIndex &parent)
//{
//    beginRemoveColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endRemoveColumns();
//}



void GMWastageEntryModel::setItemQtyByFifo( int row)
{
    //   return;
    //   qDebug()<<Q_FUNC_INFO<<row<<voucher->InventoryItems[row].BaseItem.godownList.size();
    if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder),
                         Qt::CaseInsensitive))
        return;

    else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder),
                              Qt::CaseInsensitive))
        return;

    if(voucher->InventoryItems[row].BaseItem.godownList.size() > 0){
        for( int i=1; i<voucher->InventoryItems[row].BaseItem.godownList.size();i++ )
            voucher->InventoryItems[row].BaseItem.godownList.removeAt(i);
    }
    else{
        GodownTransactionDataModel godownEntry;
        voucher->InventoryItems[row].BaseItem.godownList.append(godownEntry);
    }

    voucher->InventoryItems[row].BaseItem.crQty = voucher->InventoryItems[row].BaseItem.quantity;
    voucher->InventoryItems[row].BaseItem.godownList[0].crQty = voucher->InventoryItems[row].BaseItem.quantity;
    voucher->InventoryItems[row].BaseItem.godownList[0].toGodown = "OUT";
    if(voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown == "OUT")
        voucher->InventoryItems[row].BaseItem.godownList[0].fromGodown =
                voucher->fromGodownID;


    qDebug()<<"call fifo: "<<voucher->voucherType;
    fifoCalc->setItemQtyByFifo(voucher, row, 0);
    qDebug()<<"end of "<<Q_FUNC_INFO;

}
